/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#include "freertos/semphr.h"

#include "esp_at.h"
#include "esp_log.h"
#include "esp_system.h"

#ifdef CONFIG_AT_BASE_ON_SDIO
#include "driver/sdio_slave.h"

#include "soc/slc_struct.h"
#include "soc/slc_reg.h"
#include "soc/host_struct.h"
#include "soc/soc.h"

#include "nvs_flash.h"

#define ESP_AT_SDIO_BUFFER_SIZE      CONFIG_AT_SDIO_BLOCK_SIZE
#define ESP_AT_SDIO_BUFFER_NUM       CONFIG_AT_SDIO_BUFFER_NUM
#define ESP_AT_SDIO_QUEUE_SIZE       CONFIG_AT_SDIO_QUEUE_SIZE

#define container_of(ptr, type, member) ({      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - ((size_t) &((type *)0)->member));})

typedef struct sdio_list {
    uint8_t pbuf[ESP_AT_SDIO_BUFFER_SIZE];
    struct sdio_list* next;
    sdio_slave_buf_handle_t handle;
    uint32_t left_len;
    uint32_t pos;
} esp_at_sdio_list_t;

static const char* TAG = "SDIO-AT";

static esp_at_sdio_list_t WORD_ALIGNED_ATTR sdio_buffer_list[ESP_AT_SDIO_BUFFER_NUM];
static esp_at_sdio_list_t* pHead;
static esp_at_sdio_list_t* pTail;
static xSemaphoreHandle semahandle;

// AT response data , send them to SDIO
static int32_t at_sdio_write_data(uint8_t* data, int32_t len)
{
    esp_err_t ret = ESP_OK;
    int32_t length = len;
    uint8_t* sendbuf = NULL;
    if (len < 0 || data == NULL) {
        ESP_LOGE(TAG , "Write data error, len:%d", len);
        return -1;
    }

    sendbuf = heap_caps_malloc(len, MALLOC_CAP_DMA);
    if (sendbuf == NULL) {
        ESP_LOGE(TAG , "Malloc send buffer fail!");
        return 0;
    }

    memcpy(sendbuf, data, length);

    ret = sdio_slave_transmit(sendbuf, len);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG , "sdio slave transmit error, ret : 0x%x\r\n", ret);
        length = 0;
    }

    free(sendbuf);
    return length;
}

static int32_t at_sdio_read_data(uint8_t* data, int32_t len)
{
    uint32_t copy_len = 0;
    if (data == NULL || len < 0) {
        ESP_LOGI(TAG , "Cannot get read data address.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG , "Empty read data.");
        return 0;
    }

    if (pHead == NULL) {
        return 0;
    }

    while (copy_len < len) {
        if (!pHead) {
            break;
        }

        esp_at_sdio_list_t* p_list = pHead;

        if (len < p_list->left_len) {
            memcpy(data, p_list->pbuf + p_list->pos, len);
            p_list->pos += len;
            p_list->left_len -= len;
            copy_len += len;
        } else {
            memcpy(data, p_list->pbuf + p_list->pos, p_list->left_len);
            p_list->pos += p_list->left_len;
            copy_len += p_list->left_len;
            p_list->left_len = 0;
            xSemaphoreTake(semahandle, portMAX_DELAY);
            pHead = p_list->next;
            p_list->next = NULL;

            if (!pHead) {
                pTail = NULL;
            }

            xSemaphoreGive(semahandle);

            sdio_slave_recv_load_buf(p_list->handle);
        }
    }

    return copy_len;
}

static void esp_at_sdio_slave_init()
{
    sdio_slave_config_t config = {
        .sending_mode       = SDIO_SLAVE_SEND_STREAM,
        .send_queue_size    = ESP_AT_SDIO_QUEUE_SIZE,
        .recv_buffer_size   = ESP_AT_SDIO_BUFFER_SIZE,
    };
    sdio_slave_buf_handle_t handle;

    semahandle = xSemaphoreCreateMutex();
    esp_err_t ret = sdio_slave_initialize(&config);
    assert(ret == ESP_OK);

    for (int loop = 0; loop < ESP_AT_SDIO_BUFFER_NUM; loop++) {
        handle = sdio_slave_recv_register_buf(sdio_buffer_list[loop].pbuf);
        assert(handle != NULL);

        ret = sdio_slave_recv_load_buf(handle);
        assert(ret == ESP_OK);
    }

    sdio_slave_set_host_intena(SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET | SDIO_SLAVE_HOSTINT_BIT0);

    sdio_slave_start();

    ESP_LOGI(TAG, "slave ready");
}

static void at_sdio_recv_task(void* pvParameters)
{
    sdio_slave_buf_handle_t handle;
    size_t length = 0;
    uint8_t* ptr = NULL;

    at_sdio_write_data((uint8_t *) "\r\nready\r\n" , strlen("\r\nready\r\n"));

    for (;;) {

        // receive data from SDIO host
        esp_err_t ret = sdio_slave_recv(&handle, &ptr, &length, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Recv error,ret:%x", ret);
            continue;
        }

        esp_at_sdio_list_t* p_list = container_of(ptr, esp_at_sdio_list_t, pbuf); // get struct list pointer

        p_list->handle = handle;
        p_list->left_len = length;
        p_list->pos = 0;
        p_list->next = NULL;
        xSemaphoreTake(semahandle, portMAX_DELAY);

        if (!pTail) {
            pTail = p_list;
            pHead = pTail;
        } else {
            pTail->next = p_list;
            pTail = p_list;
        }

        xSemaphoreGive(semahandle);

        // notify length to AT core
        esp_at_port_recv_data_notify(length, portMAX_DELAY);
    }
}

void at_interface_init(void)
{
    //If you want to use AT lib, make sure obey this function.
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_sdio_read_data,
        .write_data = at_sdio_write_data,
        .get_data_length = NULL,
        .wait_write_complete = NULL,
    };

    esp_at_device_ops_regist(&esp_at_device_ops);
}

void at_custom_init(void)
{
    // init slave driver
    esp_at_sdio_slave_init();

    xTaskCreate(at_sdio_recv_task , "at_sdio_recv_task" , 4096 , NULL , 2 , NULL);
}
#endif