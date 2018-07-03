/* SPI Slave example, sender (uses SPI master driver)

 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
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

#include "esp_system.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_intr_alloc.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/spi_master.h"
/*
 SPI sender (master) example.

 This example is supposed to work together with the HSPI-AT. It uses the standard SPI pins (MISO, MOSI, SCLK, CS) to
 transmit data over in a full-duplex fashion, that is, while the master puts data on the MOSI pin, the slave puts its own
 data on the MISO pin.

 This example uses one extra pin: GPIO_HANDSHAKE is used as a handshake pin. The slave makes this pin high as soon as it is
 ready to receive/send data. This code connects this line to a GPIO interrupt which gives the rdySem semaphore. The main
 task waits for this semaphore to be given before queueing a transmission.

 Every AT data transmission need three steps. The first step transfer the flag to indicate read or write. The second step is
 to transfer the length of the data. The last step transfer the data.

 */

/*
 Pins in use. The SPI Master can use the GPIO mux, so feel free to change these if needed.
 */
#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

//Data buffer size
#define BUFF_SIZE 3*1024

static const char *TAG = "HSPI-AT";

//The semaphore indicating the slave is ready to receive stuff.
static xSemaphoreHandle rdySem;
//State flage , 1 to receive data, 2 to send data
int32_t at_flag = 0;

// data len
int32_t at_send_len, at_recv_len = 0;

/*
 This ISR is called when the handshake line goes high.
 */
static void IRAM_ATTR gpio_handshake_isr_handler(void* arg)
{
    //Give the semaphore.
    BaseType_t mustYield=false;

    //Slave have some data, mark it.
    at_flag = 1;
    xSemaphoreGiveFromISR(rdySem,&mustYield);

    if (mustYield) portYIELD_FROM_ISR();
}

QueueHandle_t esp_at_uart_queue = NULL;
void uart_task(void *pvParameters)
{
    uart_event_t event;
    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void *) &event,
                (portTickType) portMAX_DELAY)) {
            switch (event.type) {
                //Event of UART receving data
                case UART_DATA:
                    if (event.size) {
                        // Master want to send data, mark it.
                        at_flag = 2;
                        xSemaphoreGive(rdySem);
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
    vTaskDelete(NULL);
}

static void test_task(void *pvParameters)
{
    esp_err_t ret;
    spi_device_handle_t handle;
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
            .command_bits = 0,
            .address_bits = 0,
            .dummy_bits = 0,
            .clock_speed_hz = 8000000,
            .duty_cycle_pos = 128,        //50% duty cycle
            .mode = 0,
            .spics_io_num = GPIO_CS,
            .cs_ena_posttrans = 3, //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
            .queue_size = 3
    };

    //GPIO config for the handshake line.
    gpio_config_t io_conf = {
            .intr_type = GPIO_PIN_INTR_POSEDGE,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = 1,
            .pin_bit_mask = (1 << GPIO_HANDSHAKE)
    };

    char at_flag_buf[4] = "";

    char succ_tx_len_buf[4];
    char succ_rx_len_buf[4] = "";

    char sendlen_buf[4] = "";
    sendlen_buf[3] = 'A';
    int at_uart_len;
    char sendbuf[BUFF_SIZE] = "";
    char recvbuf[BUFF_SIZE] = "";

    char succ_flag[4];

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uart_driver_install(0, 2048, 8192, 10, &esp_at_uart_queue, 0);
    xTaskCreate(uart_task, "uTask", 2048, NULL, 4, NULL);

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
    //Assume the slave is ready for the first transmission: if the slave started up before us, we will not detect
    //positive edge on the handshake line.
    xSemaphoreTake(rdySem, 0);

    while (1) {
        memset(&t, 0, sizeof(t));
        memset(succ_flag, 0, 4);

        //  wait state flag (recv or send)
        xSemaphoreTake(rdySem, portMAX_DELAY);

        //recv flag
        if (at_flag == 1) {
            at_flag_buf[0] = at_flag;

            //Transmit flag, tell slave we are ready to receive data
            t.length = sizeof(at_flag_buf) * 8;        //flag bytes
            t.tx_buffer = at_flag_buf;
            t.rx_buffer = succ_flag;
            ret = spi_device_transmit(handle, &t);  //Transmit
            assert(ret == ESP_OK);
            if (succ_flag[0] != 'C') {
                xSemaphoreGive(rdySem);
                continue;
            }

            //Wait spi slave transmit data length
            memset(sendlen_buf, 0, 4);
            //Maybe transmit still is error,so don't block
            xSemaphoreTake(rdySem, 50 / portTICK_PERIOD_MS);
            //Transmit data length
            t.length = sizeof(sendlen_buf) * 8; //flag bytes
            t.tx_buffer = succ_tx_len_buf;
            t.rx_buffer = sendlen_buf;
            ret = spi_device_transmit(handle, &t);  //Transmit recv data len
            assert(ret == ESP_OK);

            //Check the data
            if (sendlen_buf[3] != 'B') {
                xSemaphoreGive(rdySem);
                continue;
            }

            at_recv_len = (sendlen_buf[1] << 7) + sendlen_buf[0];

            memset(&t, 0, sizeof(t));
            //Blocking....., spi slave transmit data will rouse the semaphone
            xSemaphoreTake(rdySem, portMAX_DELAY);
            t.length = (at_recv_len + sizeof(int32_t)) * 8; // recv len
            t.tx_buffer = NULL;
            t.rx_buffer = recvbuf;
            ret = spi_device_transmit(handle, &t);  //recv data
            assert(ret == ESP_OK);
            recvbuf[at_recv_len] = '\0';
            printf("%s", recvbuf);
            fflush(stdout);    //Force to print even if have not '\n'

            //send flag
        } else if (at_flag == 2) {
            at_flag_buf[0] = at_flag;

            //Transmit flag, tell slave we want to send data
            t.length = sizeof(at_flag_buf) * 8; //flag bytes
            t.tx_buffer = at_flag_buf;
            t.rx_buffer = succ_flag;
            ret = spi_device_transmit(handle, &t);  //Transmit!
            assert(ret == ESP_OK);

            if (succ_flag[0] != 'C') {
                xSemaphoreGive(rdySem);
                continue;
            }

            //Read data length
            if (ESP_OK != uart_get_buffered_data_len(0, (size_t*) &at_uart_len)) {
                ESP_LOGE(TAG, "Cannot get uart length");
                continue;
            }

            //Wait spi slave to read the length
            xSemaphoreTake(rdySem, portMAX_DELAY);
            if (at_uart_len <= BUFF_SIZE) {
                sendlen_buf[0] = at_uart_len & 127;
                sendlen_buf[1] = at_uart_len >> 7;
                sendlen_buf[3] = 'A';
            } else {
                ESP_LOGE(TAG, "Too large data");
                break;
            }

            //Transmit data length
            at_send_len = at_uart_len;
            t.length = sizeof(sendlen_buf) * 8; //128 bytes
            t.tx_buffer = sendlen_buf;
            t.rx_buffer = succ_rx_len_buf;
            ret = spi_device_transmit(handle, &t);  //Transmit!
            assert(ret == ESP_OK);
            if (succ_rx_len_buf[0] != 'b') {
                xSemaphoreGive(rdySem);
                continue;
            }
            memset(&t, 0, sizeof(t));
            if (at_send_len < BUFF_SIZE) {
                //Wait spi slave read the data
                xSemaphoreTake(rdySem, portMAX_DELAY);
                //Read data to a buffer
                at_uart_len = uart_read_bytes(0, (uint8_t*) sendbuf, at_send_len, 0);
                //Transmit data
                t.length = at_send_len * 8;
                t.tx_buffer = sendbuf;
                t.rx_buffer = NULL;
                ret = spi_device_transmit(handle, &t);  //Transmit!
                assert(ret == ESP_OK);
            } else {
                ESP_LOGE(TAG, "Too large data");
                break;
            }
        }

    }

    //Never reached.
    ret = spi_bus_remove_device(handle);
    assert(ret == ESP_OK);
    vTaskDelete(NULL);
}

//Main application
void app_main()
{
    xTaskCreate(test_task, "test_task", 16 * 1024, NULL, 5, NULL);
}
