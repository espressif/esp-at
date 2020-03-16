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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/stream_buffer.h"

#include "esp8266/spi_struct.h"
#include "esp8266/gpio_struct.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_at.h"

#ifdef CONFIG_AT_BASE_ON_HSPI
#include "driver/gpio.h"
#include "driver/spi.h"

static const char* TAG = "HSPI-AT";

#define SPI_SLAVE_HANDSHARK_GPIO     CONFIG_AT_HSPI_HANDSHAKE_PIN
#define SPI_SLAVE_HANDSHARK_SEL      (1ULL<<SPI_SLAVE_HANDSHARK_GPIO)
#define AT_READ_STREAM_BUFFER_SIZE      1024*2
#define AT_WRITE_STREAM_BUFFER_SIZE      1024*4

static StreamBufferHandle_t at_spi_slave_rx_ring_buf = NULL;
static StreamBufferHandle_t at_spi_slave_tx_ring_buf = NULL;      // save slave -> master data

static bool at_spi_trans_done = true;
static bool at_wait_recv_data = false;

static uint32_t at_recv_total_len = 0;
static uint32_t at_notify_len = 0;
static uint32_t at_sendto_mcu_len = 0;

//#define LOG_LOCAL_LEVEL 4     //debug mode, it will print debug log

/*
*  SPI slave send data function 
*/
static int8_t IRAM_ATTR at_spi_load_data(uint8_t* buf, int32_t len)
{
    xStreamBufferSend(at_spi_slave_tx_ring_buf, (void*) buf, len, portMAX_DELAY);

    portENTER_CRITICAL();
    // "at_spi_trans_done = TRUE" means not send or receive data 
    if (at_spi_trans_done) {
        at_sendto_mcu_len = xStreamBufferBytesAvailable(at_spi_slave_tx_ring_buf);    // get remain data
        spi_slave_set_status(HSPI_HOST, &at_sendto_mcu_len);         /// Write status, MCU will read this register
        gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 1);              // Call MCU read data length
    }
    portEXIT_CRITICAL();

    return 0;
}

static void IRAM_ATTR at_spi_slave_send_data(uint32_t* data, uint32_t len)
{
    spi_trans_t trans;
    uint16_t cmd = 0;
    uint32_t send_len = (len + 3) >> 2;
    if (send_len > 16) {
        ESP_EARLY_LOGE(TAG, "ESP8266 only support transmit 64bytes(16 * sizeof(uint32_t)) one time");
        return;
    }
    memset(&trans, 0x0, sizeof(trans));
    trans.cmd = &cmd;
    trans.addr = NULL;
    trans.bits.val = 0;
    trans.bits.cmd = 8 * 1;
    trans.bits.addr = 8 * 1;
    trans.miso = data;
    trans.bits.miso = send_len * 32;               // 8 * sizeof(uint32_t)
    spi_trans(HSPI_HOST, &trans); 
}

void IRAM_ATTR at_spi_event_callback(int event, void* arg)
{
    BaseType_t xHigherPriorityTaskWoken;
    uint32_t trans_done;
    uint32_t send_data[16];
    uint32_t receive_data[16];
    size_t ringbuf_actual_Bytes, current_bytes;
    uint32_t read_len = 0;
    bool trigger_flag = false;
    uint32_t recv_filled_len  = 0;

    switch (event) {
        case SPI_TRANS_DONE_EVENT:
            gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 0);
            trans_done = *(uint32_t*)arg;

            if (trans_done & SPI_SLV_RD_BUF_DONE) { // slave -> master data
                if (at_sendto_mcu_len > 0) {             // Have some data send to MCU
                    read_len = at_sendto_mcu_len > 64 ? 64 : at_sendto_mcu_len;

                    ESP_EARLY_LOGD(TAG, "at_spi_event_callback(SPI_SLV_RD_BUF_DONE): Send data len: %d", read_len);

                    ringbuf_actual_Bytes = xStreamBufferReceiveFromISR(at_spi_slave_tx_ring_buf,
                                           (void*)send_data,
                                           read_len,
                                           &xHigherPriorityTaskWoken);
                    if (ringbuf_actual_Bytes == read_len) {
                        at_spi_slave_send_data(send_data, ringbuf_actual_Bytes);
                        trigger_flag = true;
                    } else {
                        ESP_EARLY_LOGE(TAG, "at_spi_event_callback(SPI_SLV_RD_BUF_DONE): stream buffer get len error");
                    }

                    at_sendto_mcu_len -= ringbuf_actual_Bytes;

                } else {        
                    at_spi_trans_done = true;            // send done flag

                    if (xStreamBufferIsEmpty(at_spi_slave_tx_ring_buf) == pdFALSE) {     // Still have some data need to send??
                        at_sendto_mcu_len = xStreamBufferBytesAvailable(at_spi_slave_tx_ring_buf);
                        SPI1.rd_status.val = at_sendto_mcu_len;
                        at_spi_trans_done = false;    // a new transmit
                        trigger_flag = true;
                    }

                    /* A full send cycle should not occur receive data while sending */
                    if (SPI1.wr_status != 0) {    
                        ESP_EARLY_LOGE(TAG, "at_spi_event_callback(SPI_SLV_RD_BUF_DONE): Some MCU send data missed???, len:%d", SPI1.wr_status);
                    }
                }
            }

            if (trans_done & SPI_SLV_WR_BUF_DONE) {    // master -> slave   data
            /* "at_recv_total_len == 0" means some data lost, write status length is at_recv_total_len*/
                if (at_recv_total_len == 0) {
                    ESP_EARLY_LOGE(TAG, "at_spi_event_callback(SPI_SLV_WR_BUF_DONE): remain len should not is 0");
                    break;
                }

                read_len = at_recv_total_len > 64 ? 64 : at_recv_total_len;
                for (int x = 0; x < 16; x++) {
                    receive_data[x] = SPI1.data_buf[x];
                }
                ESP_EARLY_LOGD(TAG, "at_spi_event_callback(SPI_SLV_WR_BUF_DONE): Receive data len: %d", read_len);
                at_recv_total_len -= read_len;

                ringbuf_actual_Bytes = xStreamBufferSendFromISR(at_spi_slave_rx_ring_buf,
                                       (void*)receive_data,
                                       read_len,    // data len
                                       &xHigherPriorityTaskWoken);
                if (ringbuf_actual_Bytes != read_len) {
                    ESP_EARLY_LOGE(TAG, "at_spi_event_callback(SPI_SLV_WR_BUF_DONE): send data error");
                    assert(0);
                }

                if (xStreamBufferSpacesAvailable(at_spi_slave_rx_ring_buf) >= 64) {   // Stream buffer not filled, can be read agian
                    trigger_flag = true;
                } else {
                    recv_filled_len = xStreamBufferBytesAvailable(at_spi_slave_rx_ring_buf);
                    at_notify_len = recv_filled_len;
                    esp_at_port_recv_data_notify_from_isr(recv_filled_len);
                    at_wait_recv_data = true;
                }
            }

            if (trans_done & SPI_SLV_WR_STA_DONE) {        // master -> slave status len  
                if (at_recv_total_len) {
                    ESP_EARLY_LOGE(TAG, "Receive repeat length %d", at_recv_total_len);
                    assert(0);
                }
                spi_slave_get_status(HSPI_HOST, &at_recv_total_len);
                ESP_EARLY_LOGD(TAG, "at_spi_event_callback(SPI_SLV_WR_STA_DONE): Write status, status value: %d", at_recv_total_len);

                if (at_recv_total_len == 0) {                  // MCU write done
                    current_bytes = xStreamBufferBytesAvailable(at_spi_slave_rx_ring_buf);
                    at_notify_len = current_bytes;
                    esp_at_port_recv_data_notify_from_isr(current_bytes);
                    at_spi_trans_done = true;

                    if (SPI1.rd_status.val != 0) {          // esp8266 have some data not send
                        at_spi_trans_done = false;
                        trigger_flag = true;
                    }
                } else if (at_recv_total_len <= 8 * 1024) {       
                    at_spi_trans_done = false;
                    trigger_flag = true;
                } else {        // too large data, 
                    ESP_EARLY_LOGE(TAG, "at_spi_event_callback(SPI_SLV_WR_STA_DONE): at_recv_total_len too large: %d, only support 8K one time", at_recv_total_len);
                    at_recv_total_len = 0;
                }
            }

            if (trans_done & SPI_SLV_RD_STA_DONE) {     // Slave -> Master status len
                if (at_sendto_mcu_len > 0) {   // We have some data need to send, and MCU have got it
                    at_spi_trans_done = false;   // Start a send cycle
                    read_len = at_sendto_mcu_len > 64 ? 64 : at_sendto_mcu_len;
                    ringbuf_actual_Bytes = xStreamBufferReceiveFromISR(at_spi_slave_tx_ring_buf,
                                           (void*)send_data,
                                           read_len,
                                           &xHigherPriorityTaskWoken);

                    if (ringbuf_actual_Bytes != read_len) {
                        ESP_EARLY_LOGE(TAG, "at_spi_event_callback(SPI_SLV_RD_STA_DONE): Expect to send %d bytes, but only %d bytes", read_len, ringbuf_actual_Bytes);
                        break;
                    }

                    at_spi_slave_send_data(send_data, ringbuf_actual_Bytes);
                    ESP_EARLY_LOGD(TAG, "at_spi_event_callback(SPI_SLV_RD_BUF_DONE): Send data len: %d", read_len);
                    at_sendto_mcu_len -= read_len;
                    trigger_flag = true;
                    SPI1.rd_status.val = 0;           // clear status register
                }
            }

            if (trigger_flag) {
                gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 1);    // tell MCU read/write data
                trigger_flag = false;
            }

            if (xHigherPriorityTaskWoken == pdTRUE) {
                taskYIELD();
            }
            break;

        default:
            ESP_LOGD(TAG, "SPI event : %d", event);
            break;
    }
}

/* Called when spi receive a normal AT command, make sure you have added \r\n in your spi data */
static int32_t IRAM_ATTR at_spi_read_data(uint8_t* data, int32_t len)
{
    uint32_t ring_len = 0;

    if (data == NULL || len < 0) {
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "Empty read data.");
        return 0;
    }

    ring_len = xStreamBufferReceive(at_spi_slave_rx_ring_buf, (void*) data, len, 0);

    if (ring_len == 0) {
        return 0;
    }

    ESP_LOGD(TAG, "at_spi_read_data read data len:%d", ring_len);

    if (at_wait_recv_data) {
        ESP_LOGD(TAG, "at_spi_read_data need to wait, len: %d", len);

        if (xStreamBufferSpacesAvailable(at_spi_slave_rx_ring_buf) >= 64) {
            ESP_LOGD(TAG, "Can receive agiain");
            at_wait_recv_data = false;
            gpio_set_level(SPI_SLAVE_HANDSHARK_GPIO, 1);
        }
    }

    return ring_len;
}

/* Result of AT command, auto call when read_data get data */
static int32_t IRAM_ATTR at_spi_write_data(uint8_t* buf, int32_t len)
{
    if (len < 0 || buf == NULL) {
        ESP_LOGE(TAG, "Cannot get write data.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGE(TAG, "Empty write data.");
        return 0;
    }

    ESP_LOGD(TAG, "at_spi_write_data write data len: %d\n", len);

    at_spi_load_data(buf, len);

    return len;
}

int32_t at_spi_get_data_length(void)
{
    return at_notify_len;
}

static void at_spi_slave_init(void)
{
    spi_config_t spi_config;
    at_spi_slave_rx_ring_buf = xStreamBufferCreate(AT_READ_STREAM_BUFFER_SIZE, 1024);
    at_spi_slave_tx_ring_buf = xStreamBufferCreate(AT_WRITE_STREAM_BUFFER_SIZE, 1);

    if (at_spi_slave_tx_ring_buf == NULL || at_spi_slave_rx_ring_buf == NULL) {
        // There was not enough heap memory space available to create
        ESP_LOGE(TAG, "creat StreamBuffer error, free heap heap: %d", esp_get_free_heap_size());
        assert(0);
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

    ESP_LOGI(TAG, "init spi");
    SPI1.rd_status.val = 0x0;
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:0, BYTE_RX_ORDER:0, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = 0x1C0;
    //spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    // Load default interrupt enable
    // TRANS_DONE: false, WRITE_STATUS: true, READ_STATUS: true, WRITE_BUFFER: true, READ_BUFFER: ture
    spi_config.intr_enable.val = SPI_SLAVE_DEFAULT_INTR_ENABLE;
    // Set SPI to slave mode
    spi_config.mode = SPI_SLAVE_MODE;
    // Register SPI event callback function
    spi_config.event_cb = at_spi_event_callback;
    spi_init(HSPI_HOST, &spi_config);
}

void at_interface_init(void)
{
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_spi_read_data,
        .write_data = at_spi_write_data,
        .get_data_length = at_spi_get_data_length

    };

    esp_at_device_ops_regist(&esp_at_device_ops);
    at_spi_slave_init();
}

void at_custom_init(void)
{

}
#endif
#endif
