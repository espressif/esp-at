/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2021 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS only, in which case,
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "esp_at.h"

#if defined (CONFIG_IDF_TARGET_ESP32C3)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/stream_buffer.h"

#include "esp_system.h"
#include "esp_log.h"

#ifdef CONFIG_AT_BASE_ON_HSPI
#include "driver/gpio.h"
#include "driver/spi_slave_hd.h"

static const char* TAG = "HSPI-AT";
#define SPI_SLAVE_HANDSHARK_GPIO    CONFIG_SPI_HANDSHAKE_PIN
#define GPIO_MOSI                   CONFIG_SPI_MOSI_PIN
#define GPIO_MISO                   CONFIG_SPI_MISO_PIN
#define GPIO_SCLK                   CONFIG_SPI_SCLK_PIN
#define GPIO_CS                     CONFIG_SPI_CS_PIN

#ifdef CONFIG_SPI_QUAD_MODE
#define GPIO_WP                     CONFIG_SPI_WP_PIN
#define GPIO_HD                     CONFIG_SPI_HD_PIN
#endif

#define SLAVE_HOST                  SPI2_HOST
#define DMA_CHAN                    1

#define SPI_SLAVE_HANDSHARK_SEL     (1ULL<<SPI_SLAVE_HANDSHARK_GPIO)

#define SPI_DMA_MAX_LEN             4092
#define SPI_WRITE_STREAM_BUFFER     CONFIG_TX_STREAM_BUFFER_SIZE
#define SPI_READ_STREAM_BUFFER      CONFIG_RX_STREAM_BUFFER_SIZE
#define SLAVE_CONFIG_ADDR           4

typedef enum {
    SPI_NULL = 0,
    SPI_SLAVE_WR,         // slave -> master
    SPI_SLAVE_RD,         // maste -> slave
} spi_mode_t;

typedef struct {
    spi_mode_t direct;
} spi_msg_t;

typedef struct {
    uint32_t     direct : 8;
    uint32_t     seq_num : 8;
    uint32_t     transmit_len : 16;
} spi_rd_status_opt_t;

typedef struct {
    uint32_t     magic    : 8;
    uint32_t     send_seq : 8;
    uint32_t     send_len : 16;
} spi_wr_status_opt_t;

static uint8_t spi_slave_send_seq_num = 0;
static uint8_t spi_slave_recv_seq_num = 0;

static StreamBufferHandle_t spi_slave_rx_ring_buf = NULL;
static StreamBufferHandle_t spi_slave_tx_ring_buf = NULL;
static xQueueHandle msg_queue;
static uint8_t initiative_send_flag = 0;

static bool send_queue_error_flag = false;

static xSemaphoreHandle pxMutex;

void spi_mutex_lock(void)
{
    while (xSemaphoreTake(pxMutex, portMAX_DELAY) != pdPASS);
}

void spi_mutex_unlock(void)
{
    xSemaphoreGive(pxMutex);
}

bool cb_master_write_buffer(void* arg, spi_slave_hd_event_t* event, BaseType_t* awoken)
{
    //Give the semaphore.
    BaseType_t mustYield = false;
    spi_msg_t spi_msg = {
        .direct = SPI_SLAVE_RD,
    };
    if (xQueueSendFromISR(msg_queue, (void*)&spi_msg, &mustYield) != pdPASS) {
        ets_printf("send queue from isr error\n");
        send_queue_error_flag = true;
    }
    return true;
}

inline static void write_transmit_len(spi_mode_t spi_mode, uint16_t transmit_len)
{
    ESP_EARLY_LOGV(TAG, "write rd status: %d, %d", (uint32_t)spi_mode, transmit_len);
    if (transmit_len > SPI_WRITE_STREAM_BUFFER) {
        ESP_EARLY_LOGI(TAG, "Set error RD len: %d", transmit_len);
        return;
    }
    spi_rd_status_opt_t rd_status_opt;
    rd_status_opt.direct = spi_mode;
    rd_status_opt.transmit_len = transmit_len;
    if (spi_mode == SPI_SLAVE_WR) {    // slave -> master
        rd_status_opt.seq_num = ++spi_slave_send_seq_num;
    } else if (spi_mode == SPI_SLAVE_RD) {                             // SPI_SLAVE_RECV   maste -> slave
        rd_status_opt.seq_num = ++spi_slave_recv_seq_num;
    }

    spi_slave_hd_write_buffer(SLAVE_HOST, SLAVE_CONFIG_ADDR, (uint8_t*)&rd_status_opt, sizeof(spi_rd_status_opt_t));
}

/* Called when spi receive a normal AT command, make sure you have added \r\n in your spi data */
static int32_t at_spi_read_data(uint8_t* data, int32_t len)
{
    uint32_t ring_len = 0;
    if (data == NULL || len < 0) {
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "Empty read data.");
        return 0;
    }
    ESP_LOGD(TAG, "read len: %d", len);

    ring_len = xStreamBufferReceive(spi_slave_rx_ring_buf, (void*) data, len, 0);

    if (ring_len != len) {
        ESP_LOGD(TAG, "Read len expect %d, but actual read %d", len, ring_len);
    }

    return len;
}

/* Result of AT command, auto call when read_data get data */
static int32_t at_spi_write_data(uint8_t* buf, int32_t len)
{
    uint32_t length = 0;
    if (len < 0 || buf == NULL) {
        ESP_LOGE(TAG, "Cannot get write data.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGE(TAG, "Empty write data.");
        return 0;
    }
    
    if (buf == NULL  || len > SPI_WRITE_STREAM_BUFFER || len == 0) {
        ESP_LOGE(TAG, "Write data error, len:%d", len);
        return -1;
    }

    ESP_LOGI(TAG, "Write data len: %d", len);

    length = xStreamBufferSend(spi_slave_tx_ring_buf, buf, len, portMAX_DELAY);

    if(length != len) {
        ESP_LOGE(TAG, "Stream buffer send error");
        return -1;
    }

    spi_mutex_lock();
    if (initiative_send_flag == 0) {
        initiative_send_flag = 1;
        spi_msg_t spi_msg = {
            .direct = SPI_SLAVE_WR,
        };

        if (xQueueSend(msg_queue, (void*)&spi_msg, 0) != pdPASS) {
            ESP_LOGE(TAG, "send WR queue for spi_write error");
        }
    }
    spi_mutex_unlock();
    return len;
}

int32_t at_spi_get_data_length(void)
{
    if (spi_slave_rx_ring_buf ==  NULL) {
        return 0;
    }
    return xStreamBufferBytesAvailable(spi_slave_rx_ring_buf);
}

static void at_spi_slave_task(void* pvParameters)
{
    spi_slave_hd_data_t slave_trans;
    spi_slave_hd_data_t* ret_trans;
    spi_msg_t trans_msg = {0};
    uint32_t send_len = 0;
    uint32_t tmp_send_len = 0;
    uint32_t remain_len = 0;

    uint8_t* data_buf = (uint8_t*)malloc(SPI_DMA_MAX_LEN * sizeof(uint8_t));
    if (data_buf == NULL) {
        ESP_LOGE(TAG, "malloc fail");
        return;
    }

    while (1) {
        memset(data_buf, 0x0, SPI_DMA_MAX_LEN);
        memset(&trans_msg, 0x0, sizeof(spi_msg_t));

        xQueueReceive(msg_queue, (void*)&trans_msg, (TickType_t)portMAX_DELAY);
        ESP_LOGD(TAG, "Direct: %d", trans_msg.direct);
        if (trans_msg.direct == SPI_SLAVE_RD) {    // master -> slave

            // Tell master transmit mode is master send
            write_transmit_len(SPI_SLAVE_RD, SPI_DMA_MAX_LEN); 

            gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 0);
            
            memset(&slave_trans, 0x0, sizeof(spi_slave_hd_data_t));
            slave_trans.data = data_buf;
            slave_trans.len = SPI_DMA_MAX_LEN;
            ESP_ERROR_CHECK(spi_slave_hd_queue_trans(SLAVE_HOST, SPI_SLAVE_CHAN_RX, &slave_trans, portMAX_DELAY));

            gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 1); // it means slave has recv done, notify master to do next translation

            ESP_ERROR_CHECK(spi_slave_hd_get_trans_res(SLAVE_HOST, SPI_SLAVE_CHAN_RX, &ret_trans, portMAX_DELAY));
            if (ret_trans->trans_len > SPI_READ_STREAM_BUFFER || ret_trans->trans_len <= 0) {
                ESP_LOGE(TAG, "Recv error len: %d, %d, %x\n", ret_trans->trans_len, slave_trans.len, data_buf[0]);
                break;
            }

            xStreamBufferSend(spi_slave_rx_ring_buf, (void*) data_buf, ret_trans->trans_len, portMAX_DELAY);
            // notify length to AT core
            esp_at_port_recv_data_notify(ret_trans->trans_len, portMAX_DELAY);

        } else if (trans_msg.direct == SPI_SLAVE_WR) {     // slave -> master
            remain_len = xStreamBufferBytesAvailable(spi_slave_tx_ring_buf);
            if (remain_len > 0){
                send_len = remain_len > SPI_DMA_MAX_LEN ? SPI_DMA_MAX_LEN : remain_len;
                write_transmit_len(SPI_SLAVE_WR, send_len);

            } else {
                ESP_LOGD(TAG, "Receive send queue but no data");
                initiative_send_flag = 0;
                continue;
            }

            gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 0);
            tmp_send_len = xStreamBufferReceive(spi_slave_tx_ring_buf, (void*) data_buf, send_len, 0);

            if (send_len != tmp_send_len) {
                ESP_LOGE(TAG, "Read len expect %d, but actual read %d", send_len, tmp_send_len);
                break;
            }
            memset(&slave_trans, 0x0, sizeof(spi_slave_hd_data_t));
            slave_trans.data = (uint8_t*)data_buf;
            slave_trans.len = send_len;
            ESP_ERROR_CHECK(spi_slave_hd_queue_trans(SLAVE_HOST, SPI_SLAVE_CHAN_TX, &slave_trans, portMAX_DELAY));
            gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 1); // it means slave send done and notify master to recv
            ESP_ERROR_CHECK(spi_slave_hd_get_trans_res(SLAVE_HOST, SPI_SLAVE_CHAN_TX, &ret_trans, portMAX_DELAY));

            spi_mutex_lock();
            remain_len = xStreamBufferBytesAvailable(spi_slave_tx_ring_buf);
            if (remain_len > 0){
                spi_msg_t spi_msg = {
                    .direct = SPI_SLAVE_WR,
                };
                if (xQueueSend(msg_queue, (void*)&spi_msg, 0) != pdPASS) {
                    ESP_LOGE(TAG, "send WR queue error");
                    break;
                }
            } else {
                initiative_send_flag = 0;
            }
            spi_mutex_unlock();

        } else {
            ESP_LOGE(TAG, "Unknow direct: %d", trans_msg.direct);
            continue;
        }
    }

    free(data_buf);
    vTaskDelete(NULL);
}

void spi_bus_default_config(spi_bus_config_t* bus_cfg)
{
    bus_cfg->mosi_io_num = GPIO_MOSI;
    bus_cfg->miso_io_num = GPIO_MISO;
    bus_cfg->sclk_io_num = GPIO_SCLK;
#ifdef CONFIG_SPI_QUAD_MODE
    bus_cfg->quadwp_io_num = GPIO_WP;
    bus_cfg->quadhd_io_num = GPIO_HD;
    bus_cfg->flags = SPICOMMON_BUSFLAG_QUAD;
#else
    bus_cfg->quadwp_io_num = -1;
    bus_cfg->quadhd_io_num = -1;
#endif

#ifdef CONFIG_SPI_DUAL_MODE
    bus_cfg->flags = SPICOMMON_BUSFLAG_DUAL;
#endif
    bus_cfg->max_transfer_sz = 14000;
}

void spi_slot_default_config(spi_slave_hd_slot_config_t* slave_hd_cfg)
{
    slave_hd_cfg->spics_io_num = GPIO_CS;
    slave_hd_cfg->flags = 0;
    slave_hd_cfg->mode = CONFIG_SPI_MODE;
    slave_hd_cfg->command_bits = 8;
    slave_hd_cfg->address_bits = 8;
    slave_hd_cfg->dummy_bits = 8;
    slave_hd_cfg->queue_size = 4;
    slave_hd_cfg->dma_chan = SPI_DMA_CH_AUTO;

    // master writes to shared buffer
    slave_hd_cfg->cb_config.cb_buffer_rx = cb_master_write_buffer;
    slave_hd_cfg->cb_config.cb_recv = NULL;
}

static void init_slave_hd(void)
{
    spi_bus_config_t bus_cfg = {};
    spi_bus_default_config(&bus_cfg);

    spi_slave_hd_slot_config_t slave_hd_cfg = {};
    spi_slot_default_config(&slave_hd_cfg);

    ESP_ERROR_CHECK(spi_slave_hd_init(SLAVE_HOST, &bus_cfg, &slave_hd_cfg));
}

void at_pre_active_write_data_callback(at_write_data_fn_t fn)
{
    // Do something before active write data
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    at_wkmcu_if_config(fn);
#endif
}

void at_pre_sleep_callback(at_sleep_mode_t mode)
{
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    at_set_mcu_state_if_sleep(mode);
#endif
}

void at_interface_init(void)
{
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_spi_read_data,
        .write_data = at_spi_write_data,
        .get_data_length = at_spi_get_data_length,
        .wait_write_complete = NULL

    };

    esp_at_custom_ops_struct esp_at_custom_ops = {
        .status_callback = NULL,
        .pre_sleep_callback = at_pre_sleep_callback,
        .pre_deepsleep_callback = NULL,
        .pre_restart_callback = NULL,
        .pre_active_write_data_callback = at_pre_active_write_data_callback,
    };

    esp_at_device_ops_regist(&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
}

void at_custom_init(void)
{
    pxMutex = xSemaphoreCreateMutex();
    spi_slave_rx_ring_buf = xStreamBufferCreate(SPI_READ_STREAM_BUFFER, 1024);
    spi_slave_tx_ring_buf = xStreamBufferCreate(SPI_WRITE_STREAM_BUFFER, 1024);
    if (pxMutex == NULL || spi_slave_rx_ring_buf == NULL || spi_slave_tx_ring_buf == NULL) {
        // There was not enough heap memory space available to create
        ESP_LOGE(TAG, "creat StreamBuffer error, free heap heap: %d", esp_get_free_heap_size());
        return;
    }

    ESP_LOGI(TAG, "init gpio");
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = SPI_SLAVE_HANDSHARK_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 0);

    ESP_LOGI(TAG, "init SPI");
    // init slave driver
    init_slave_hd();

    msg_queue = xQueueCreate(10, sizeof(spi_msg_t));
    if (!msg_queue) {
        ESP_LOGE(TAG, "Semaphore create error");
        return;
    }
    xTaskCreate(at_spi_slave_task , "at_spi_task" , 4096 , NULL , 10 , NULL);
}
#endif
#endif
