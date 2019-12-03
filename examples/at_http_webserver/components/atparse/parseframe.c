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
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"

#include "esp_at.h"
#include "esp_log.h"
#include "esp_system.h"

#include "esp_event_loop.h"
#include "esp_wifi.h"

#include "atparse.h"
#include "parseframe.h"

#define ESP_AT_RING_ESP_AT_BUFFER_SIZE 6*1024
#define ESP_AT_RECEIVE_BUFF_SIZE 1500

static const char* TAG = "ParseFrame";

static RingbufHandle_t at_read_ring_buf;
static RingbufHandle_t at_write_ring_buf;

// AT massage queue
xQueueHandle at_msg_queue;

rsp_cb response_cb = NULL;

/*
* Call AT lib to process the received data.
*/
int8_t at_call_lib(uint8_t* data, int32_t len)
{
    if (len > 0) {
        if (xRingbufferSend(at_read_ring_buf, data, len, portMAX_DELAY) == pdFALSE) {
            ESP_LOGE(TAG, "Cannot add command to at ringbuf.");
            return -1;
        }
    } else {
        ESP_LOGE(TAG, "Command empty.");
        return -1;
    }

    esp_at_port_recv_data_notify(len, portMAX_DELAY);
    return 0;
}

/*Called when recieve a normal AT command, make sure have added \r\n */
static int32_t at_http_read_data(uint8_t* data, int32_t len)
{
    if (data == NULL || len < 0) {
        ESP_LOGW(TAG, "Cannot get read data address.");
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
static int32_t at_http_write_data(uint8_t* buf, int32_t len)
{
    if (len < 0 || buf == NULL) {
        ESP_LOGE(TAG, "Cannot get write data.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGE(TAG, "Empty write data.");
        return 0;
    }

    if (xRingbufferSend(at_write_ring_buf, buf, len, portMAX_DELAY) == pdFALSE) {
        ESP_LOGE(TAG, "Cannot add command to at ringbuf.");
        return -1;
    }

    return len;
}

/* Ignore it, -1 is a right value*/
static int32_t at_port_get_data_length(void)
{
    return -1;
}

/* Ignore it*/
static bool at_port_wait_write_complete(int32_t timeout_msec)
{
    return true;
}

static void check_reponse_task(void* pvParameters)
{
    size_t recv_len;
    char* esp_at_receive_data_buf = NULL;

    while (1) {
        char* at_rsp = (char*) xRingbufferReceive(at_write_ring_buf, &recv_len, (portTickType) portMAX_DELAY);

        if (at_rsp == NULL || recv_len == 0) {
            ESP_LOGI(TAG, "Cannot recieve socket data from write ringbuf.");
            continue;
        }

        if (recv_len >= ESP_AT_RECEIVE_BUFF_SIZE) {
            ESP_LOGE(TAG, "Too large data, skip it");
            continue;
        }

        esp_at_receive_data_buf = malloc(ESP_AT_RECEIVE_BUFF_SIZE * sizeof(char));
        memcpy(esp_at_receive_data_buf, at_rsp, recv_len);
        esp_at_receive_data_buf[recv_len] = '\0';
        ESP_LOGD(TAG, "Receive data:%s\n", esp_at_receive_data_buf);

        // Every added AT command will rewrite the response callback
        if (response_cb != NULL) {
            (*response_cb)(esp_at_receive_data_buf, recv_len);
        }

        vRingbufferReturnItem(at_write_ring_buf, at_rsp);
        free(esp_at_receive_data_buf);
    }
}

static esp_err_t at_wifi_event_handler(void *ctx, system_event_t *event)
{
    esp_err_t ret = esp_at_wifi_event_handler(ctx, event);

    return ret;
}

static void initialise_wifi(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK( esp_event_loop_init(at_wifi_event_handler, NULL) );
    
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void init_at_module(void)
{
    uint8_t* version = (uint8_t*)malloc(192);
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_http_read_data,
        .write_data = at_http_write_data,
        .get_data_length = at_port_get_data_length,
        .wait_write_complete = at_port_wait_write_complete,
    };

    sprintf((char*)version, "compile time:%s %s\r\n", __DATE__, __TIME__);
#ifdef CONFIG_ESP_AT_FW_VERSION

    if ((strlen(CONFIG_ESP_AT_FW_VERSION) > 0) && (strlen(CONFIG_ESP_AT_FW_VERSION) <= 128)) {
        ESP_LOGI(TAG, "%s\r\n", CONFIG_ESP_AT_FW_VERSION);
        strcat((char*)version, CONFIG_ESP_AT_FW_VERSION);
    }

#endif

    tcpip_adapter_init();
    initialise_wifi();

    esp_at_device_ops_regist(&esp_at_device_ops);

    esp_at_module_init(CONFIG_LWIP_MAX_SOCKETS - 1, version);   // reserved one for server
    free(version);

    if (esp_at_base_cmd_regist() == false) {
        ESP_LOGI(TAG, "regist base cmd fail\r\n");
    }

    if (esp_at_wifi_cmd_regist() == false) {
        ESP_LOGI(TAG, "regist wifi cmd fail\r\n");
    }

    if (esp_at_net_cmd_regist() == false) {
        ESP_LOGI(TAG, "regist net cmd fail\r\n");
    }

    if (esp_at_ble_cmd_regist() == false) {
        ESP_LOGI(TAG, "regist ble cmd fail\r\n");
    }

#if CONFIG_READ_WRITE_FILE_SYSTEM
    ESP_LOGI(TAG, "Use file function read write file system\r\n");
#else
    if (esp_at_fs_cmd_regist() == false) {
        ESP_LOGI(TAG, "regist FS cmd fail\r\n");
    }
    ESP_LOGI(TAG, "AT command regist OK\r\n");
#endif
}

void at_init_parse_frame(void)
{
    at_read_ring_buf = xRingbufferCreate(ESP_AT_RING_ESP_AT_BUFFER_SIZE, RINGBUF_TYPE_NOSPLIT);
    at_write_ring_buf = xRingbufferCreate(ESP_AT_RING_ESP_AT_BUFFER_SIZE, RINGBUF_TYPE_NOSPLIT);

    if (at_read_ring_buf == NULL || at_write_ring_buf == NULL) {
        ESP_LOGE(TAG, "Cannot create ringbuf");
        return;
    }

    init_at_module();

    at_msg_queue = xQueueCreate(1 , sizeof(at_response));

    xTaskCreate(&check_reponse_task, "custom_flag_task", 4 * 1024, NULL, 5, NULL);
}
