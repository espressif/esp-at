/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_at.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "nvs_flash.h"

#include "driver/spi_slave.h"

#include "at_interface.h"

/*
 This example uses one extra pin: GPIO_HANDSHAKE is used as a handshake pin. The slave makes this pin high as soon as it is
 ready to receive/send data. This code connects this line to a GPIO interrupt which gives the rdySem semaphore. The main
 task waits for this semaphore to be given before queueing a transmission.

 Every AT data transmission need three steps. The first step wait Master send the flag to indicate read or write. The second step is
 to transfer the length of the data. The last step transfer the data.
 */

#define GPIO_HANDSHAKE          CONFIG_AT_HSPI_HANDSHAKE_PIN
#define GPIO_MOSI               CONFIG_AT_HSPI_MOSI_PIN
#define GPIO_MISO               CONFIG_AT_HSPI_MISO_PIN
#define GPIO_SCLK               CONFIG_AT_HSPI_SCLK_PIN
#define GPIO_CS                 CONFIG_AT_HSPI_CS_PIN

//Data buffer size
#define ESP_AT_BUFFER_SIZE 3*1024

//Max value, detect whether is a error len
#define MAX_COUNT 999999

#define ESP_AT_RING_ESP_AT_BUFFER_SIZE 12*1024

static const char* TAG = "HSPI-AT";
static bool at_transparent_transmition = false;
static RingbufHandle_t at_read_ring_buf;
static RingbufHandle_t at_send_ring_buf;

static volatile bool at_send_flag = false;
static volatile bool at_cb_flag = false;
static bool at_ring_write_flag = false;

//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
static void my_post_setup_cb(spi_slave_transaction_t* trans)
{
    if (trans->user) {
        WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1 << GPIO_HANDSHAKE));
    } else {
        at_send_flag = true;
    }
}

//Called after transaction is sent/received. We use this to set the handshake line low.
static void my_post_trans_cb(spi_slave_transaction_t* trans)
{
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1 << GPIO_HANDSHAKE));
    at_send_flag = false;
}

/*Called when spi receive a normal AT command, make sure you have added \r\n in your spi data*/
static int32_t at_spi_read_data(uint8_t* data, int32_t len)
{
    if (data == NULL || len < 0) {
        ESP_LOGE(TAG, "Cannot get read data address.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "Empty read data.");
        return 0;
    }

    size_t ring_len = len;
    uint8_t* recv_data = (uint8_t*) xRingbufferReceive(at_read_ring_buf, &ring_len, (portTickType) 0);

    if (recv_data == NULL) {
        ESP_LOGE(TAG, "Cannot recieve socket data from ringbuf.");
        return -1;
    } else {
        memcpy(data, recv_data, len);
        vRingbufferReturnItem(at_read_ring_buf, (void*) recv_data);
    }

    return len;
}

/*Result of AT command, auto call when read_data get data*/
static int32_t at_spi_write_data(uint8_t* buf, int32_t len)
{
    if (len < 0 || buf == NULL) {
        ESP_LOGE(TAG, "Cannot get write data.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGE(TAG, "Empty write data.");
        return 0;
    }

    int buf_size = ESP_AT_BUFFER_SIZE - 1;

    //3K is enough for network data
    if (len < ESP_AT_BUFFER_SIZE) {
        if (xRingbufferSend(at_send_ring_buf, buf, len, portMAX_DELAY) == pdFALSE) {
            ESP_LOGE(TAG, "Cannot write data to ringbuf.");
            return -1;
        }

        //Should not arrive here, if come, slice it.
    } else {
        int32_t len_tmp = len;
        int flag_num = 0;

        while (len_tmp > buf_size) {
            if (xRingbufferSend(at_send_ring_buf, (buf + flag_num * buf_size),
                                buf_size, portMAX_DELAY) == pdFALSE) {
                ESP_LOGE(TAG, "Cannot write data to ringbuf.");
                return -1;
            }

            flag_num++;
            len_tmp -= buf_size;
        }

        if (len_tmp > 0) {
            if (xRingbufferSend(at_send_ring_buf, (buf + flag_num * buf_size),
                                len_tmp, portMAX_DELAY) == pdFALSE) {
                ESP_LOGE(TAG, "Cannot write data to ringbuf.");
                return -1;
            }
        }
    }

    //Slave can send data even if blocking
    if (at_cb_flag && at_send_flag) {
        at_ring_write_flag = true;
        WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1 << GPIO_HANDSHAKE));
    }

    return len;
}

/*Ignore it, -1 is a right value*/
static int32_t at_port_get_data_length(void)
{
    return -1;
}

/*In SPI mode, we can ignore it*/
static bool at_port_wait_write_complete(int32_t timeout_msec)
{
    return true;
}

static void spi_task(void* pvParameters)
{
    esp_err_t ret;
    int32_t recv_len;
    int at_spi_flag = 0;
    int32_t send_len = 0;
    int byte_num;
    char recvbuf[ESP_AT_BUFFER_SIZE] = "";
    char sendbuf[ESP_AT_BUFFER_SIZE] = "";
    char succ_flag[4];
    memset(succ_flag, 0, 4);
    succ_flag[0] = 'C';

    char len_buf[4];
    memset(len_buf, 0, 4);

    //Transfer check info
    char succ_tx_len_buf[4] = "";
    succ_tx_len_buf[0] = 'b';

    //Receive check info
    char succ_rx_len_buf[4];

    char at_flag_buf[4];
    memset(at_flag_buf, 0, 4);
    spi_slave_transaction_t t;

    //Configuration for the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg = {
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .queue_size = 3,
        .flags = 0,
        .post_setup_cb = my_post_setup_cb,
        .post_trans_cb = my_post_trans_cb
    };

    //Configuration for the handshake line
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1 << GPIO_HANDSHAKE)
    };

    //Configure handshake line as output
    gpio_config(&io_conf);

    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Initialize SPI slave interface
    ret = spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, 1);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Cannot init spi.");
        vTaskDelete(NULL);
    }

    // "ready" is the initial message
    char power_on_buf[10] = "\r\nready\r\n";

    if (xRingbufferSend(at_send_ring_buf, power_on_buf, strlen(power_on_buf),
                        portMAX_DELAY) == pdFALSE) {
        ESP_LOGE(TAG, "Cannot write power on data to ringbuf.");
        vTaskDelete(NULL);
    }

    for (;;) {
        memset(&t, 0, sizeof(t));
        memset(at_flag_buf, 0, 4);

        //Waiting master tell us want to send or recv.
        t.length = sizeof(at_flag_buf) * 8;
        t.tx_buffer = succ_flag;
        t.rx_buffer = at_flag_buf;

        size_t size = MAX_COUNT;
        uint8_t* send_data = (uint8_t*) xRingbufferReceive(at_send_ring_buf,
                             &size, (portTickType) 0);

        //AT lib have not data
        if (size == MAX_COUNT) {
            t.user = NULL;
            //Allow AT lib send message even blocking
            at_cb_flag = true;
        } else {
            t.user = &at_spi_flag;
        }

        ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
        assert(ret == ESP_OK);
        //We want receive or send data, forbit to interrupt
        at_cb_flag = false;

        if (at_flag_buf[0] == 1) {  // send
            // If transmit is triggered by the AT lib
            if (at_ring_write_flag && (size == MAX_COUNT)) {
                send_data = (uint8_t*) xRingbufferReceive(at_send_ring_buf, &size, (portTickType) 0);
                at_ring_write_flag = false;
            }

            // Error transmit, abandon it
            if (size == MAX_COUNT) {
                continue;
            }

            send_len = size;

            memcpy(sendbuf, send_data, send_len);
            //We have saved the data, return buffer.
            vRingbufferReturnItem(at_send_ring_buf, (void*) send_data);

            //Slice the length if it over than 127
            if (send_len <= ESP_AT_BUFFER_SIZE) {
                len_buf[0] = send_len & 127;
                len_buf[1] = send_len >> 7;
                len_buf[3] = 'B'; // check info
            } else {
                ESP_LOGE(TAG, "Too large data");
                break;
            }

            //Send data length
            t.length = sizeof(len_buf) * 8;
            t.tx_buffer = len_buf;
            t.rx_buffer = succ_rx_len_buf;
            t.user = &at_spi_flag; //This option pull up the gpio, call spi master read the length
            ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
            assert(ret == ESP_OK);

            /* Send data */
            memset(&t, 0, sizeof(t));
            t.length = (send_len + sizeof(int32_t)) * 8;
            t.tx_buffer = sendbuf;
            t.rx_buffer = NULL;
            t.user = &at_spi_flag;
            ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
            assert(ret == ESP_OK);

        } else if (at_flag_buf[0] == 2) { // recv

            memset(len_buf, 0, 4);
            /* recv master data len  */
            t.length = sizeof(len_buf) * 8;
            t.tx_buffer = succ_tx_len_buf;
            t.rx_buffer = len_buf;
            t.user = &at_spi_flag;
            ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
            assert(ret == ESP_OK);

            if (len_buf[3] != 'A') {
                continue;
            }

            recv_len = (len_buf[1] << 7) + len_buf[0];

            /* recv master data */
            byte_num = recv_len;
            memset(&t, 0, sizeof(t));

            if (byte_num < ESP_AT_BUFFER_SIZE) {
                t.length = recv_len * 8;
                t.tx_buffer = NULL;
                t.rx_buffer = recvbuf;
                t.user = &at_spi_flag;
                ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
                assert(ret == ESP_OK);

                //if the data is "+++", exit transparent transmition mode
                if (at_transparent_transmition && (byte_num == 3)
                        && (memcmp(recvbuf, "+++", 3) == 0)) {
                    esp_at_transmit_terminal();
                    continue;
                }

                if (xRingbufferSend(at_read_ring_buf, recvbuf, byte_num,
                                    portMAX_DELAY) == pdFALSE) {     // a buffer is better
                    ESP_LOGE(TAG, "Cannot write data to ringbuf.");
                    break;
                }

            }

            esp_at_port_recv_data_notify(byte_num, portMAX_DELAY);
        }

    }

    spi_slave_free(HSPI_HOST);
    vTaskDelete(NULL);
}

/*This function let you handle some operation when using custom cmd and AT status change*/
static void at_status_callback(esp_at_status_type status)
{
    switch (status) {
        case ESP_AT_STATUS_NORMAL:
            at_transparent_transmition = false;
            break;

        case ESP_AT_STATUS_TRANSMIT:
            at_transparent_transmition = true;
            break;
    }
}

void at_interface_init(void)
{
    uint8_t* version = (uint8_t*)malloc(192);
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_spi_read_data,
        .write_data = at_spi_write_data,
        .get_data_length = at_port_get_data_length,
        .wait_write_complete = at_port_wait_write_complete,
    };

    esp_at_custom_ops_struct esp_at_custom_ops = {
        .status_callback = at_status_callback,
    };

    at_read_ring_buf = xRingbufferCreate(ESP_AT_RING_ESP_AT_BUFFER_SIZE, RINGBUF_TYPE_NOSPLIT);

    if (at_read_ring_buf == NULL) {
        ESP_LOGE(TAG, "Cannot create ringbuf");
        return;
    }

    at_send_ring_buf = xRingbufferCreate(ESP_AT_RING_ESP_AT_BUFFER_SIZE, RINGBUF_TYPE_NOSPLIT);

    if (at_send_ring_buf == NULL) {
        ESP_LOGE(TAG, "Cannot create ringbuf");
        return;
    }

    esp_at_device_ops_regist(&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
}

void at_custom_init(void)
{
    xTaskCreate(spi_task, "spi_task", 12 * 1024, NULL, 4, NULL);
}

