/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

#ifdef CONFIG_AT_BASE_ON_SDIO
#include "driver/sdio_slave.h"
#include "soc/slc_struct.h"
#include "soc/slc_reg.h"
#include "soc/host_struct.h"
#include "soc/soc.h"
#include "esp_at.h"
#include "esp_at_interface.h"

#define AT_SDIO_DMA_SIZE                        4092
#define container_of(ptr, type, member) ({      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - ((size_t) &((type *)0)->member));})

typedef struct sdio_list {
    uint8_t pbuf[CONFIG_AT_SDIO_BLOCK_SIZE];
    struct sdio_list *next;
    sdio_slave_buf_handle_t handle;
    uint32_t left_len;
    uint32_t pos;
} esp_at_sdio_list_t;

// static variables
static esp_at_sdio_list_t *sp_head;
static esp_at_sdio_list_t *sp_tail;
static SemaphoreHandle_t s_sdio_rw_sema;
static esp_at_sdio_list_t WORD_ALIGNED_ATTR s_sdio_buffer_list[CONFIG_AT_SDIO_BUFFER_NUM];
static const char *TAG = "at-sdio";

static int32_t at_sdio_write_data(uint8_t *data, int32_t len)
{
    if (len < 0 || data == NULL) {
        ESP_LOGE(TAG, "invalid data:%p or len:%d", data, len);
        return -1;
    }

    xSemaphoreTake(s_sdio_rw_sema, portMAX_DELAY);

    uint32_t had_written_len = 0;
    do {
        int to_send_len = (len - had_written_len) > AT_SDIO_DMA_SIZE ? AT_SDIO_DMA_SIZE : (len - had_written_len);
        uint8_t *to_send_data = heap_caps_malloc(to_send_len, MALLOC_CAP_DMA);
        if (to_send_data == NULL) {
            ESP_LOGE(TAG, "malloc buffer fail");
            xSemaphoreGive(s_sdio_rw_sema);
            return 0;
        }

        memcpy(to_send_data, data + had_written_len, to_send_len);
        esp_err_t ret = sdio_slave_transmit(to_send_data, to_send_len);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "sdio slave transmit error");
            free(to_send_data);
            xSemaphoreGive(s_sdio_rw_sema);
            return 0;
        }

        free(to_send_data);
        had_written_len += to_send_len;
    } while (had_written_len != len);
    xSemaphoreGive(s_sdio_rw_sema);

    return len;
}

static int32_t at_sdio_read_data(uint8_t *data, int32_t len)
{
    if (data == NULL || len < 0) {
        ESP_LOGE(TAG, "invalid data:%p or len:%d", data, len);
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "read empty data");
        return 0;
    }

    if (sp_head == NULL) {
        return 0;
    }

    uint32_t copy_len = 0;
    while (copy_len < len) {
        if (!sp_head) {
            break;
        }

        esp_at_sdio_list_t *p_list = sp_head;
        if (len < p_list->left_len) {
            memcpy(data + copy_len, p_list->pbuf + p_list->pos, len);
            p_list->pos += len;
            p_list->left_len -= len;
            copy_len += len;
        } else {
            memcpy(data + copy_len, p_list->pbuf + p_list->pos, p_list->left_len);
            p_list->pos += p_list->left_len;
            copy_len += p_list->left_len;
            p_list->left_len = 0;
            xSemaphoreTake(s_sdio_rw_sema, portMAX_DELAY);
            sp_head = p_list->next;
            p_list->next = NULL;

            if (!sp_head) {
                sp_tail = NULL;
            }

            xSemaphoreGive(s_sdio_rw_sema);
            sdio_slave_recv_load_buf(p_list->handle);
        }
    }

    return copy_len;
}

static void at_sdio_task(void *params)
{
    size_t size = 0;

    for (;;) {
        // receive data from sdio host
        uint8_t *addr = NULL;
        sdio_slave_buf_handle_t handle;
        esp_err_t ret = sdio_slave_recv(&handle, &addr, &size, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "recv error, ret:%x", ret);
            continue;
        }

        esp_at_sdio_list_t *p_list = container_of(addr, esp_at_sdio_list_t, pbuf);
        p_list->handle = handle;
        p_list->left_len = size;
        p_list->pos = 0;
        p_list->next = NULL;
        xSemaphoreTake(s_sdio_rw_sema, portMAX_DELAY);

        if (!sp_tail) {
            sp_tail = p_list;
            sp_head = sp_tail;
        } else {
            sp_tail->next = p_list;
            sp_tail = p_list;
        }
        xSemaphoreGive(s_sdio_rw_sema);

        // notify esp-at core to receive data
        esp_at_port_recv_data_notify(size, portMAX_DELAY);
    }
}

static void at_sdio_init(void)
{
    // init sdio slave configuration
    sdio_slave_config_t config = {
        .sending_mode       = SDIO_SLAVE_SEND_STREAM,
        .send_queue_size    = CONFIG_AT_SDIO_QUEUE_SIZE,
        .recv_buffer_size   = CONFIG_AT_SDIO_BLOCK_SIZE,
    };
    ESP_ERROR_CHECK(sdio_slave_initialize(&config));

    // create read-write mutex
    s_sdio_rw_sema = xSemaphoreCreateMutex();

    // register and load receive buffer for sdio slave
    sdio_slave_buf_handle_t handle;
    for (int loop = 0; loop < CONFIG_AT_SDIO_BUFFER_NUM; loop++) {
        handle = sdio_slave_recv_register_buf(s_sdio_buffer_list[loop].pbuf);
        assert(handle != NULL);
        ESP_ERROR_CHECK(sdio_slave_recv_load_buf(handle));
    }

    // enable the interrupt
    sdio_slave_set_host_intena(SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET | SDIO_SLAVE_HOSTINT_BIT0);

    // start sdio slave
    sdio_slave_start();

    xTaskCreate(at_sdio_task, "at_sdio_task", 4096, NULL, 2, NULL);
}

void at_interface_init(void)
{
    // init interface driver
    at_sdio_init();

    // init interface operations
    esp_at_device_ops_struct sdio_ops = {
        .read_data = at_sdio_read_data,
        .write_data = at_sdio_write_data,
        .get_data_length = NULL,
        .wait_write_complete = NULL,
    };
    at_interface_ops_init(&sdio_ops);

    // init interface hooks
    at_interface_hooks(NULL);
}

#endif
