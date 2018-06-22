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
//#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"

#include "driver/sdio_slave.h"

//#include "rom/lldesc.h"
#include "soc/slc_struct.h"
#include "soc/slc_reg.h"
#include "soc/host_struct.h"
#include "soc/soc.h"

#include "nvs_flash.h"

#define ESP_AT_RING_ESP_AT_BUFFER_SIZE 3*1024
#define BUFFER_SIZE     512
#define SDIO_SLAVE_QUEUE_SIZE 11


static bool at_transparent_transmition = false;
RingbufHandle_t at_read_ring_buf;

static const char* TAG = "SDIO-AT";


sdio_slave_buf_handle_t handle;

int32_t at_port_write_data(uint8_t* data, int32_t len)
{
    int32_t length = len;
    int32_t pos = 0;
    ESP_LOGI(TAG , "At write data len:%d:", len);

    uint8_t* sendbuf = heap_caps_malloc(len, MALLOC_CAP_DMA);
    memset(sendbuf, '\0', len);
    memcpy(sendbuf, data, length);

    while (len > 512) {
        esp_err_t ret = sdio_slave_transmit(sendbuf + pos, 512);
        assert(ret == ESP_OK);
        pos += 512;
        len -= 512;
    }


    esp_err_t ret = sdio_slave_transmit(sendbuf + pos, len);
    assert(ret == ESP_OK);

    free(sendbuf);
    return length;
}

int32_t at_port_read_data(uint8_t* data, int32_t len)
{
    if (data == NULL || len < 0) {
        ESP_LOGE(TAG , "Cannot get read data address.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG , "Empty read data.");
        return 0;
    }

    size_t ring_len = len;
    uint8_t* recv_data = (uint8_t*) xRingbufferReceive(at_read_ring_buf , &ring_len , (portTickType) 0);

    if (recv_data == NULL) {
        ESP_LOGE(TAG , "Cannot recieve socket data from ringbuf.");
        return -1;
    } else {
        memcpy(data , recv_data , len);
    }

    return len;
}

/*We can ignore it, -1 is a right value*/
int32_t at_port_get_data_length(void)
{
    return -1;
}

/*In socket mode, we can ignore it*/
bool at_port_wait_write_complete(int32_t timeout_msec)
{
    return true;
}


void esp_at_sdio_slave_init()
{
    WORD_ALIGNED_ATTR uint8_t    buffer[BUFFER_SIZE] = {};

    sdio_slave_config_t config = {
        .sending_mode       = SDIO_SLAVE_SEND_STREAM,
        .send_queue_size    = SDIO_SLAVE_QUEUE_SIZE,
        .recv_buffer_size   = BUFFER_SIZE,
    };

    esp_err_t ret = sdio_slave_initialize(&config);
    assert(ret == ESP_OK);

    sdio_slave_buf_handle_t handle;

    handle = sdio_slave_recv_register_buf(buffer);
    assert(handle != NULL);
    ret = sdio_slave_recv_load_buf(handle);
    assert(ret == ESP_OK);

    sdio_slave_set_host_intena(SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET | SDIO_SLAVE_HOSTINT_BIT0);

    sdio_slave_start();

    ESP_LOGI(TAG, "slave ready");
}

void sdio_recv_task(void* pvParameters)
{
    for (;;) {
        ESP_LOGD(TAG, "Host have some packets");
        size_t length;
        uint8_t* ptr;

        esp_err_t ret = sdio_slave_recv(&handle, &ptr, &length, portMAX_DELAY);

        if (ret == ESP_OK) {
            ESP_LOGD(TAG, "handle: %p, recv len: %d:", handle, length);
            ESP_LOGD(TAG, "data:%s", ptr);

        } else {
            ESP_LOGI(TAG, "Recv error,ret:%x", ret);
            continue;
        }

        if (xRingbufferSend(at_read_ring_buf, ptr, length, portMAX_DELAY) == pdFALSE) {     // a buffer is better
            ESP_LOGE(TAG, "Cannot write data to ringbuf.");
        }

        esp_at_port_recv_data_notify(length, portMAX_DELAY);
        sdio_slave_recv_load_buf(handle);
    }
}


void at_status_callback(esp_at_status_type status)
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


void at_task_init(void)
{
    nvs_flash_init();
    uint8_t* version = (uint8_t*) malloc(64);

    if (version == NULL) {
        ESP_LOGE(TAG , "malloc version space fail");
        return;
    }

    //If you want to use AT lib, make sure obey this function.
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_port_read_data,
        .write_data = at_port_write_data,
        .get_data_length = at_port_get_data_length,
        .wait_write_complete = at_port_wait_write_complete,
    };

    esp_at_custom_ops_struct esp_at_custom_ops = {
        .status_callback = at_status_callback,
    };

    at_read_ring_buf = xRingbufferCreate(ESP_AT_RING_ESP_AT_BUFFER_SIZE , RINGBUF_TYPE_ALLOWSPLIT);

    if (at_read_ring_buf == NULL) {
        ESP_LOGE(TAG , "Cannot create ringbuf");
        return;
    }

    sprintf((char*) version , "compile time:%s %s" , __DATE__ , __TIME__);
    esp_at_device_ops_regist(&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
    esp_at_module_init(CONFIG_LWIP_MAX_SOCKETS - 1, version);   // reserved one for server
    free(version);

#ifdef CONFIG_AT_BASE_COMMAND_SUPPORT

    if (esp_at_base_cmd_regist() == false) {
        printf("regist base cmd fail\r\n");
    }

#endif

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT

    if (esp_at_wifi_cmd_regist() == false) {
        printf("regist wifi cmd fail\r\n");
    }

#endif

#ifdef CONFIG_AT_NET_COMMAND_SUPPORT

    if (esp_at_net_cmd_regist() == false) {
        printf("regist net cmd fail\r\n");
    }

#endif


#ifdef CONFIG_AT_FS_COMMAND_SUPPORT

    if (esp_at_fs_cmd_regist() == false) {
        printf("regist ble cmd fail\r\n");
    }

#endif

    esp_at_sdio_slave_init();
    //Regist custom command
    xTaskCreate(&sdio_recv_task , "sdio_recv_task" , 4096 , NULL , 5 , NULL);
    // esp_at_port_write_data((uint8_t *) "\r\nready\r\n" , strlen("\r\nready\r\n"));
}


