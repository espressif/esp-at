/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_intr_alloc.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/spi_master.h"


/*
Pins in use. The SPI Master can use the GPIO mux, so feel free to change these if needed.
*/
#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

static const char* TAG = "ESP32-MCU";

static xQueueHandle rdySem;
static QueueHandle_t esp_at_uart_queue = NULL;
static RingbufHandle_t at_spi_master_receive_ring_buf = NULL;
static RingbufHandle_t at_spi_master_send_ring_buf = NULL;

#define RING_BUFFER_SIZE  4096

static uint32_t ring_buffer_filled_byte = 0;

/* SPI data cmd definition */
#define SPI_MASTER_WRITE_DATA_TO_SLAVE_CMD     2
#define SPI_MASTER_READ_DATA_FROM_SLAVE_CMD    3

/* SPI status cmd definition */
#define SPI_MASTER_WRITE_STATUS_TO_SLAVE_CMD   1
#define SPI_MASTER_READ_STATUS_FROM_SLAVE_CMD  4

spi_device_handle_t handle;

typedef enum {
    SPI_NULL = 0,
    SPI_READ,         // slave -> master
    SPI_WRITE             // maste -> slave
} at_spi_mode_t;

static at_spi_mode_t trans_mode = SPI_NULL;

/*
This ISR is called when the handshake line goes high.
There are two ways to trigger the GPIO interrupt:
1. Master sends data, slave has received successfully
2. Slave has data want to transmit
*/
static void IRAM_ATTR gpio_handshake_isr_handler(void* arg)
{
    //Give the semaphore.
    BaseType_t mustYield = false;
    xSemaphoreGiveFromISR(rdySem, &mustYield);

    if (mustYield) {
        portYIELD_FROM_ISR();
    }
}

// SPI data transmit function, address length is 8bit
static void IRAM_ATTR at_spi_master_tran_data(at_spi_mode_t mode, uint32_t* data, uint32_t len)
{
    spi_transaction_t t;
    uint32_t send_len = 0;
    memset(&t, 0x0, sizeof(t));

    if (len > 64) {  // ESP8266 does not have DMA, so it can only receive 64 bytes at a time
        ESP_LOGE(TAG, "send len:%d too large", len);
        return;
    }

    send_len = (len + 3) >> 2;

    spi_transaction_ext_t trans = (spi_transaction_ext_t) {
        .base = {
            .flags = SPI_TRANS_VARIABLE_ADDR,
            .length = send_len * 32,
        },
        .address_bits = 8,
    };

    if (mode == SPI_READ) {
        trans.base.cmd = SPI_MASTER_READ_DATA_FROM_SLAVE_CMD;    // slave -> master command, donnot change
        trans.base.rx_buffer = (void*)data;
    } else if (mode == SPI_WRITE) {
        trans.base.cmd = SPI_MASTER_WRITE_DATA_TO_SLAVE_CMD;    // master -> slave command, donnot change
        trans.base.tx_buffer = (void*)data;
    }

    spi_device_transmit(handle, (spi_transaction_t*)&trans);
}

//Read SPI status(Slave send to master data length) transmit function , address length is 0 bit(no address)
static int IRAM_ATTR at_spi_master_get_trans_len()
{
    uint32_t len;
    spi_transaction_ext_t trans = (spi_transaction_ext_t) {
        .base = {
            .flags = SPI_TRANS_VARIABLE_ADDR,
            .length = 4 * 8,
            .cmd = SPI_MASTER_READ_STATUS_FROM_SLAVE_CMD,
            .rx_buffer = &len
        },
        .address_bits = 0,
    };
    spi_device_transmit(handle, (spi_transaction_t*)&trans);

    return len;
}

// SPI status transmit function(Master sendto slave data length) , address length is 0 bit(no address)
static void IRAM_ATTR at_spi_master_set_trans_len(uint32_t len)
{
    spi_transaction_ext_t trans = (spi_transaction_ext_t) {
        .base = {
            .flags = SPI_TRANS_VARIABLE_ADDR,
            .length = 4 * 8,
            .cmd = SPI_MASTER_WRITE_STATUS_TO_SLAVE_CMD,
            .tx_buffer = &len
        },
        .address_bits = 0,
    };

    spi_device_transmit(handle, (spi_transaction_t*)&trans);
}

// SPI master sent to slave function
static int8_t IRAM_ATTR at_spi_load_data(uint8_t* buf, int32_t len)
{

    if (len > RING_BUFFER_SIZE) {
        ESP_LOGE(TAG, "Send length %d is too large", len);
        return -1;
    }

// ESP32 xRingbuffer lib have not API can read filled stream buffer bytes, just igone this global variable if your stream buffer have this API
    ring_buffer_filled_byte += len;
    xRingbufferSend(at_spi_master_send_ring_buf, (void*)buf, len, portMAX_DELAY);

    if (trans_mode == SPI_NULL) {         // it means SPI not send or receive
        xSemaphoreGive(rdySem);
    }

    return 0;
}

// Read send data from UART
void uart_task(void* pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(1024);

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void*) &event,
                          (portTickType) portMAX_DELAY)) {
            switch (event.type) {
                    //Event of UART receving data
                case UART_DATA:
                    if (event.size) {
                        // Master want to send data.
                        uart_read_bytes(0, dtmp, event.size, portMAX_DELAY);

                        // send data to slave
                        at_spi_load_data(dtmp, event.size);
                    }
                    break;

                case UART_PATTERN_DET:
                    break;

                    //Others
                default:
                    break;
            }
        }
    }

    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

void init_driver(void)
{
    esp_err_t ret;

    //Configuration for the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    //Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg = {
        .command_bits = 8,
        .dummy_bits = 0,
        .clock_speed_hz = 8000000,    // 8M SPI clock
        .duty_cycle_pos = 128,      //50% duty cycle
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .cs_ena_posttrans = 3,      //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
        .queue_size = 3
    };

    //GPIO config for the handshake line.
    gpio_config_t io_conf = {
        .intr_type = GPIO_PIN_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pin_bit_mask = (1 << GPIO_HANDSHAKE)
    };

    //Create the semaphore.
    rdySem = xSemaphoreCreateBinary();

    //Set up handshake line interrupt.
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_set_intr_type(GPIO_HANDSHAKE, GPIO_PIN_INTR_POSEDGE);
    gpio_isr_handler_add(GPIO_HANDSHAKE, gpio_handshake_isr_handler, NULL);

    //Initialize the SPI bus and add the device we want to send stuff to.
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret == ESP_OK);
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &handle);
    assert(ret == ESP_OK);

    uart_driver_install(0, 2048, 8192, 10, &esp_at_uart_queue, 1);
    xTaskCreate(uart_task, "uTask", 2048, NULL, 4, NULL);

    //Assume the slave is ready for the first transmission: if the slave started up before us, we will not detect
    //positive edge on the handshake line.
    xSemaphoreGive(rdySem);
}

/* The actual SPI transfer function
*  In order to improve the transfer efficiency, use two ring buffer to buffer the received or sent data
*/
static void IRAM_ATTR spi_master_transmit(void* arg)
{
    uint32_t transmit_data[16];
    size_t recv_actual_len = 0;
    uint8_t* transmit_point = NULL;

    uint32_t transmit_len = 0;
    uint32_t read_len = 0;

    while (1) {
        memset((uint8_t*)transmit_data, 0x0, 64);
        xSemaphoreTake(rdySem, portMAX_DELAY);

        if (trans_mode == SPI_NULL) {       // Some data need to read or write???
            // have some data need to send ???
            if (ring_buffer_filled_byte != 0) {
                trans_mode = SPI_WRITE;
                //transmit_len = RING_BUFFER_SIZE - send_buffer_remain_len;
                transmit_len = ring_buffer_filled_byte;
                ESP_LOGD(TAG, "Send len: %d\n", transmit_len);
                at_spi_master_set_trans_len(transmit_len);
                continue;
            }

            // Check if there is any data to receive
            transmit_len  = at_spi_master_get_trans_len();

            if (transmit_len > 0) {
                ESP_LOGD(TAG, "Receive data len: %d\n", transmit_len);
                trans_mode = SPI_READ;
                continue;
            } else {
                ESP_LOGD(TAG, "Nothing to do");
                continue;
            }
        }

        read_len =  transmit_len > 64 ? 64 : transmit_len;

        // ESP8266 have some data want to transimit, read it
        if (trans_mode == SPI_READ) {
            at_spi_master_tran_data(SPI_READ, transmit_data, read_len);
            xRingbufferSend(at_spi_master_receive_ring_buf, (void*)transmit_data, read_len, portMAX_DELAY);
            transmit_len -= read_len;

            if (transmit_len == 0) {
                trans_mode = SPI_NULL;
                /* When ESP8266 sending data , maybe MCU also have some date wait for send */
                if (ring_buffer_filled_byte > 0) {
                    xSemaphoreGive(rdySem);
                }
            }

            // MCU want to send data to ESP8266
        } else if (trans_mode == SPI_WRITE) {
            if (read_len > 0) {
                transmit_point = (uint8_t*)xRingbufferReceiveUpTo(at_spi_master_send_ring_buf, &recv_actual_len, 0, read_len);

                // When the data is at herd and tail of the buffer, ESP32 ring buffer cannot read it all at once, so we have to read it twice
                if (read_len != recv_actual_len) {
                    /* If use other stream buffer , ignore it */
                    memcpy((uint8_t*)transmit_data, transmit_point, recv_actual_len);
                    vRingbufferReturnItem(at_spi_master_send_ring_buf, (void*)transmit_point);
                    uint32_t remain_len = read_len - recv_actual_len;
                    uint32_t current_pos = recv_actual_len;
                    transmit_point = (uint8_t*)xRingbufferReceiveUpTo(at_spi_master_send_ring_buf, &recv_actual_len, 0, remain_len);
                    assert(recv_actual_len == remain_len);
                    memcpy(((uint8_t*)transmit_data) + current_pos, transmit_point, recv_actual_len);
                } else {
                    memcpy((uint8_t*)transmit_data, transmit_point, recv_actual_len);
                }

                at_spi_master_tran_data(SPI_WRITE, transmit_data, read_len);
                vRingbufferReturnItem(at_spi_master_send_ring_buf, (void*)transmit_point);
                transmit_len -= read_len;
                ring_buffer_filled_byte -= read_len;
            } else {
                trans_mode = SPI_NULL;

                if (ring_buffer_filled_byte > 0) {
                    xSemaphoreGive(rdySem);
                } else {
                    // if ring buffer is empty, send status=0 telle slave send done
                    at_spi_master_set_trans_len(0);
                }
            }

        }
    }
}

static void IRAM_ATTR spi_master_receive_task(void* arg)
{
    uint8_t* recv_data = NULL;
    uint8_t* print_data = malloc(1024 + 1);
    size_t recv_len = 0;

    while (1) {
        recv_data = (uint8_t*)xRingbufferReceiveUpTo(at_spi_master_receive_ring_buf, &recv_len, 50 / portTICK_RATE_MS, 1024);

        if (recv_data == NULL) {
            continue;
        } else {
            memcpy(print_data, recv_data, recv_len);
            print_data[recv_len] = '\0';
            printf("%s", print_data);
            fflush(stdout);    //Force to print even if have not '\n'

            vRingbufferReturnItem(at_spi_master_receive_ring_buf, (void*)recv_data);
        }
    }

    free(print_data);
    vTaskDelete(NULL);
}

//Main application
void app_main()
{
    at_spi_master_receive_ring_buf = xRingbufferCreate(RING_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);
    at_spi_master_send_ring_buf = xRingbufferCreate(RING_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);

    init_driver();
    xTaskCreate(spi_master_transmit, "spi_master_transmit", 1024 * 2, NULL, 8, NULL);
    xTaskCreate(spi_master_receive_task, "spi_master_receive_task", 1024 * 2, NULL, 4, NULL);
}
