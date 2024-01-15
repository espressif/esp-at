/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "freertos/stream_buffer.h"

#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_intr_alloc.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/spi_master.h"

/*
Pins in use. The SPI Master can use the GPIO mux, so feel free to change these if needed.
*/
#define GPIO_HANDSHAKE        CONFIG_SPI_HANDSHAKE_PIN
#define GPIO_MOSI             CONFIG_SPI_MOSI_PIN
#define GPIO_MISO             CONFIG_SPI_MISO_PIN
#define GPIO_SCLK             CONFIG_SPI_SCLK_PIN
#define GPIO_CS               CONFIG_SPI_CS_PIN

#ifdef CONFIG_SPI_QUAD_MODE
#define GPIO_WP               CONFIG_SPI_WP_PIN
#define GPIO_HD               CONFIG_SPI_HD_PIN
#endif

#ifdef CONFIG_IDF_TARGET_ESP32
#define MASTER_HOST           HSPI_HOST
#elif defined CONFIG_IDF_TARGET_ESP32C3
#define MASTER_HOST           SPI2_HOST
#endif

#define DMA_CHAN              SPI_DMA_CH_AUTO
#define ESP_SPI_DMA_MAX_LEN   4092
#define CMD_HD_WRBUF_REG      0x01
#define CMD_HD_RDBUF_REG      0x02
#define CMD_HD_WRDMA_REG      0x03
#define CMD_HD_RDDMA_REG      0x04
#define CMD_HD_WR_END_REG     0x07
#define CMD_HD_INT0_REG       0x08
#define WRBUF_START_ADDR      0x0
#define RDBUF_START_ADDR      0x4
#define STREAM_BUFFER_SIZE    1024 * 8

typedef enum {
    SPI_NULL = 0,
    SPI_READ,         // slave -> master
    SPI_WRITE,             // maste -> slave
} spi_mode_t;

typedef struct {
    bool slave_notify_flag; // when slave recv done or slave notify master to recv, it will be true
} spi_master_msg_t;

typedef struct {
    uint32_t     magic    : 8;    // 0xFE
    uint32_t     send_seq : 8;
    uint32_t     send_len : 16;
} spi_send_opt_t;

typedef struct {
    uint32_t     direct : 8;
    uint32_t     seq_num : 8;
    uint32_t     transmit_len : 16;
} spi_recv_opt_t;

typedef struct {
    spi_mode_t direct;
} spi_msg_t;

static QueueHandle_t msg_queue; // meaasge queue used for communicating read/write start
static QueueHandle_t esp_at_uart_queue = NULL;
static spi_device_handle_t handle;
static StreamBufferHandle_t spi_master_tx_ring_buf = NULL;
static const char* TAG = "SPI AT Master";
static SemaphoreHandle_t pxMutex;
static uint8_t initiative_send_flag = 0; // it means master has data to send to slave
static uint32_t plan_send_len = 0; // master plan to send data len

static uint8_t current_send_seq = 0;
static uint8_t current_recv_seq = 0;

static void spi_mutex_lock(void)
{
    while (xSemaphoreTake(pxMutex, portMAX_DELAY) != pdPASS);
}

static void spi_mutex_unlock(void)
{
    xSemaphoreGive(pxMutex);
}

/*
This isr handler is called when the handshake line goes high.
There are two ways to trigger the GPIO interrupt:
1. Master sends data, slave has received successfully
2. Slave has data want to transmit
*/
static void IRAM_ATTR gpio_handshake_isr_handler(void* arg)
{
    //Give the semaphore.
    BaseType_t mustYield = false;
    spi_master_msg_t spi_msg = {
        .slave_notify_flag = true,
    };
    xQueueSendFromISR(msg_queue, (void*)&spi_msg, &mustYield); // notify master to do next transaction

    if (mustYield) {
        portYIELD_FROM_ISR();
    }
}

static void at_spi_master_send_data(uint8_t* data, uint32_t len)
{
    spi_transaction_t trans = {
#if defined(CONFIG_SPI_QUAD_MODE)
        .flags = SPI_TRANS_MODE_QIO,
        .cmd = CMD_HD_WRDMA_REG | (0x2 << 4),    // master -> slave command, donnot change
#elif defined(CONFIG_SPI_DUAL_MODE)
        .flags = SPI_TRANS_MODE_DIO,
        .cmd = CMD_HD_WRDMA_REG | (0x1 << 4),
#else
        .cmd = CMD_HD_WRDMA_REG,    // master -> slave command, donnot change
#endif
        .length = len * 8,
        .tx_buffer = (void*)data
    };
    spi_device_polling_transmit(handle, &trans);
}

static void at_spi_master_recv_data(uint8_t* data, uint32_t len)
{
    spi_transaction_t trans = {
#if defined(CONFIG_SPI_QUAD_MODE)
        .flags = SPI_TRANS_MODE_QIO,
        .cmd = CMD_HD_RDDMA_REG | (0x2 << 4),    // master -> slave command, donnot change
#elif defined(CONFIG_SPI_DUAL_MODE)
        .flags = SPI_TRANS_MODE_DIO,
        .cmd = CMD_HD_RDDMA_REG | (0x1 << 4),
#else
        .cmd = CMD_HD_RDDMA_REG,    // master -> slave command, donnot change
#endif
        .rxlength = len * 8,
        .rx_buffer = (void*)data
    };
    spi_device_polling_transmit(handle, &trans);
}

// send a single to slave to tell slave that master has read DMA done
static void at_spi_rddma_done(void)
{
    spi_transaction_t end_t = {
        .cmd = CMD_HD_INT0_REG,
    };
    spi_device_polling_transmit(handle, &end_t);
}

// send a single to slave to tell slave that master has write DMA done
static void at_spi_wrdma_done(void)
{
    spi_transaction_t end_t = {
        .cmd = CMD_HD_WR_END_REG,
    };
    spi_device_polling_transmit(handle, &end_t);
}

// when spi slave ready to send/recv data from the spi master, the spi slave will a trigger GPIO interrupt,
// then spi master should query whether the slave will perform read or write operation.
static spi_recv_opt_t query_slave_data_trans_info()
{
    spi_recv_opt_t recv_opt;
    spi_transaction_t trans = {
        .cmd = CMD_HD_RDBUF_REG,
        .addr = RDBUF_START_ADDR,
        .rxlength = 4 * 8,
        .rx_buffer = &recv_opt,
    };
    spi_device_polling_transmit(handle, (spi_transaction_t*)&trans);
    return recv_opt;
}

// before spi master write to slave, the master should write WRBUF_REG register to notify slave,
// and then wait for handshark line trigger gpio interrupt to start the data transmission.
static void spi_master_request_to_write(uint8_t send_seq, uint16_t send_len)
{
    spi_send_opt_t send_opt;
    send_opt.magic = 0xFE;
    send_opt.send_seq = send_seq;
    send_opt.send_len = send_len;

    spi_transaction_t trans = {
        .cmd = CMD_HD_WRBUF_REG,
        .addr = WRBUF_START_ADDR,
        .length = 4 * 8,
        .tx_buffer = &send_opt,
    };
    spi_device_polling_transmit(handle, (spi_transaction_t*)&trans);
    // increment
    current_send_seq  = send_seq;
}

// spi master write data to slave
static int8_t spi_write_data(uint8_t* buf, int32_t len)
{
    if (len > ESP_SPI_DMA_MAX_LEN) {
        ESP_LOGE(TAG, "Send length errot, len:%ld", len);
        return -1;
    }
    at_spi_master_send_data(buf, len);
    at_spi_wrdma_done();
    return 0;
}

// write data to spi tx_ring_buf, this is just for test
static int32_t write_data_to_spi_task_tx_ring_buf(const void* data, size_t size)
{
    int32_t length = size;

    if (data == NULL  || length > STREAM_BUFFER_SIZE) {
        ESP_LOGE(TAG, "Write data error, len:%ld", length);
        return -1;
    }

    length = xStreamBufferSend(spi_master_tx_ring_buf, data, size, portMAX_DELAY);
    return length;
}

// notify slave to recv data
static void notify_slave_to_recv(void)
{
    if (initiative_send_flag == 0) {
        spi_mutex_lock();
        uint32_t tmp_send_len = xStreamBufferBytesAvailable(spi_master_tx_ring_buf);
        if (tmp_send_len > 0) {
            plan_send_len = tmp_send_len > ESP_SPI_DMA_MAX_LEN ? ESP_SPI_DMA_MAX_LEN : tmp_send_len;
            spi_master_request_to_write(current_send_seq + 1, plan_send_len); // to tell slave that the master want to write data
            initiative_send_flag = 1;
        }
        spi_mutex_unlock();
    }
}

static void IRAM_ATTR spi_trans_control_task(void* arg)
{
    esp_err_t ret;
    spi_master_msg_t trans_msg = {0};
    uint32_t send_len = 0;

    uint8_t* trans_data = (uint8_t*)malloc(ESP_SPI_DMA_MAX_LEN * sizeof(uint8_t));
    if (trans_data == NULL) {
        ESP_LOGE(TAG, "malloc fail");
        return;
    }

    while (1) {
        xQueueReceive(msg_queue, (void*)&trans_msg, (TickType_t)portMAX_DELAY);
        spi_mutex_lock();
        spi_recv_opt_t recv_opt = query_slave_data_trans_info();

        if (recv_opt.direct == SPI_WRITE) {
            if (plan_send_len == 0) {
                ESP_LOGE(TAG, "master want send data but length is 0");
                continue;
            }

            if (recv_opt.seq_num != current_send_seq) {
                ESP_LOGE(TAG, "SPI send seq error, %x, %x", recv_opt.seq_num, current_send_seq);
                if (recv_opt.seq_num == 1) {
                    ESP_LOGE(TAG, "Maybe SLAVE restart, ignore");
                    current_send_seq = recv_opt.seq_num;
                } else {
                    break;
                }
            }

            //initiative_send_flag = 0;
            send_len = xStreamBufferReceive(spi_master_tx_ring_buf, (void*) trans_data, plan_send_len, 0);

            if (send_len != plan_send_len) {
                ESP_LOGE(TAG, "Read len expect %lu, but actual read%lu", plan_send_len, send_len);
                break;
            }

            ret = spi_write_data(trans_data, plan_send_len);
            if (ret < 0) {
                ESP_LOGE(TAG, "Load data error");
                return;
            }

            // maybe streambuffer filled some data when SPI transimit, just consider it after send done, because send flag has already in SLAVE queue
            uint32_t tmp_send_len = xStreamBufferBytesAvailable(spi_master_tx_ring_buf);
            if (tmp_send_len > 0) {
                plan_send_len = tmp_send_len > ESP_SPI_DMA_MAX_LEN ? ESP_SPI_DMA_MAX_LEN : tmp_send_len;
                spi_master_request_to_write(current_send_seq + 1, plan_send_len);
            } else {
                initiative_send_flag = 0;
            }

        } else if (recv_opt.direct == SPI_READ) {
            if (recv_opt.seq_num != ((current_recv_seq + 1) & 0xFF)) {
                ESP_LOGE(TAG, "SPI recv seq error, %x, %x", recv_opt.seq_num, (current_recv_seq + 1));
                if (recv_opt.seq_num == 1) {
                    ESP_LOGE(TAG, "Maybe SLAVE restart, ignore");
                } else {
                    break;
                }
            }

            if (recv_opt.transmit_len > STREAM_BUFFER_SIZE || recv_opt.transmit_len == 0) {
                ESP_LOGE(TAG, "SPI read len error, %x", recv_opt.transmit_len);
                break;
            }

            current_recv_seq = recv_opt.seq_num;
            memset(trans_data, 0x0, recv_opt.transmit_len);
            at_spi_master_recv_data(trans_data, recv_opt.transmit_len);
            at_spi_rddma_done();
            trans_data[recv_opt.transmit_len] = '\0';
            printf("%s", trans_data);
            fflush(stdout);    //Force to print even if have not '\n'
        } else {
            ESP_LOGD(TAG, "Unknow direct: %d", recv_opt.direct);
            spi_mutex_unlock();
            continue;
        }

        spi_mutex_unlock();
    }

    free(trans_data);
    vTaskDelete(NULL);
}

// read data from uart, and then send data to spi_master_tx_ring_buf.
// this is just used to use uart port to send command.
void uart_task(void* pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(1024);

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void *) &event,
                          (TickType_t) portMAX_DELAY)) {
            switch (event.type) {
            //Event of UART receving data
            case UART_DATA:
                if (event.size) {
                    memset(dtmp, 0x0, 1024);
                    // read the data which spi master want to send
                    uart_read_bytes(0, dtmp, event.size, portMAX_DELAY);
                    // send data to spi task
                    write_data_to_spi_task_tx_ring_buf(dtmp, event.size);
                    notify_slave_to_recv();
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

inline void spi_bus_defalut_config(spi_bus_config_t* bus_cfg)
{
    bus_cfg->mosi_io_num = GPIO_MOSI;
    bus_cfg->miso_io_num = GPIO_MISO;
    bus_cfg->sclk_io_num = GPIO_SCLK;
#if defined(CONFIG_SPI_QUAD_MODE)
    bus_cfg->quadwp_io_num = GPIO_WP;
    bus_cfg->quadhd_io_num = GPIO_HD;
#else
    bus_cfg->quadwp_io_num = -1;
    bus_cfg->quadhd_io_num = -1;
#endif
    bus_cfg->max_transfer_sz = 14000;
}

inline void spi_device_default_config(spi_device_interface_config_t* dev_cfg)
{
    dev_cfg->clock_speed_hz = SPI_MASTER_FREQ_20M;
    dev_cfg->mode = 0;
    dev_cfg->spics_io_num = GPIO_CS;
    dev_cfg->cs_ena_pretrans = 8;
    dev_cfg->cs_ena_posttrans = 8;
    dev_cfg->command_bits = 8;
    dev_cfg->address_bits = 8;
    dev_cfg->dummy_bits = 8;
    dev_cfg->queue_size = 16;
    dev_cfg->flags = SPI_DEVICE_HALFDUPLEX;
    dev_cfg->input_delay_ns = 25;
}

static void init_master_hd(spi_device_handle_t* spi)
{
    // GPIO config for the handshake line.
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pin_bit_mask = (1 << GPIO_HANDSHAKE)
    };

    //Set up handshake line interrupt.
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_set_intr_type(GPIO_HANDSHAKE, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(GPIO_HANDSHAKE, gpio_handshake_isr_handler, NULL);

    // Create the meaasge queue.
    msg_queue = xQueueCreate(5, sizeof(spi_master_msg_t));
    // Create the tx_buf.
    spi_master_tx_ring_buf = xStreamBufferCreate(STREAM_BUFFER_SIZE, 1);
    // Create the semaphore.
    pxMutex = xSemaphoreCreateMutex();

    //init bus
    spi_bus_config_t bus_cfg = {};
    spi_bus_defalut_config(&bus_cfg);
    ESP_ERROR_CHECK(spi_bus_initialize(MASTER_HOST, &bus_cfg, DMA_CHAN));

    //add device
    spi_device_interface_config_t dev_cfg = {};
    spi_device_default_config(&dev_cfg);
    ESP_ERROR_CHECK(spi_bus_add_device(MASTER_HOST, &dev_cfg, spi));

    spi_mutex_lock();

    spi_recv_opt_t recv_opt = query_slave_data_trans_info();
    ESP_LOGI(TAG, "now direct:%u", recv_opt.direct);

    if (recv_opt.direct == SPI_READ) { // if slave in waiting response status, master need to give a read done single.
        if (recv_opt.seq_num != ((current_recv_seq + 1) & 0xFF)) {
            ESP_LOGE(TAG, "SPI recv seq error, %x, %x", recv_opt.seq_num, (current_recv_seq + 1));
            if (recv_opt.seq_num == 1) {
                ESP_LOGE(TAG, "Maybe SLAVE restart, ignore");
            }
        }

        current_recv_seq = recv_opt.seq_num;

        at_spi_rddma_done();
    }
    spi_mutex_unlock();
}

void app_main()
{
    init_master_hd(&handle);

    // init UART
    uart_driver_install(0, 2048, 8192, 10, &esp_at_uart_queue, 1);
    xTaskCreate(uart_task, "uTask", 2048, NULL, 4, NULL);

    xTaskCreate(spi_trans_control_task, "spi_trans_control_task", 1024 * 2, NULL, 8, NULL);
}
