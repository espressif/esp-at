/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_at.h"

#ifndef CONFIG_AT_FS_COMMAND_SUPPORT
#error "CONFIG_AT_FS_COMMAND_SUPPORT is undefined, please enable it first: python build.py menuconfig > Component config > AT > AT FS command support"
#endif

#define AT_NETWORK_TIMEOUT_MS       (5000)
#define AT_URL_LEN_MAX              (8 * 1024)
#define AT_HEAP_BUFFER_SIZE         4096

typedef struct {
    bool fs_mounted;                /*!< File system mounted */
    char *path;                     /*!< File path */
    FILE *fp;                       /*!< File pointer */
    uint32_t available_size;        /*!< The available size of the file system */
    uint32_t total_size;            /*!< The total size of the file system */
    uint32_t wrote_size;            /*!< The file size that has been written to flash */
} at_write_fs_handle_t;

typedef struct {
    char *url;                      /*!< URL */
    int32_t total_size;             /*!< Total size of the file */
    int32_t recv_size;              /*!< Received size of the file */
    bool is_chunked;                /*!< Chunked flag */
    SemaphoreHandle_t sync_sema;    /*!< Semaphore for synchronization */
    esp_http_client_handle_t client;    /*!< HTTP client handle */
    at_write_fs_handle_t *fs_handle;    /*!< File system handle */
} at_httpget_to_fs_t;

// static variables
static at_httpget_to_fs_t *sp_http_to_fs;
static const char *TAG = "at_http_to_fs";

at_write_fs_handle_t *at_http_to_fs_begin(char *path)
{
    at_write_fs_handle_t *fs_handle = (at_write_fs_handle_t *)calloc(1, sizeof(at_write_fs_handle_t));
    if (!fs_handle) {
        return NULL;
    }

    // mount file system
    if (!esp_at_fs_mount()) {
        free(fs_handle);
        ESP_AT_LOGE(TAG, "esp_at_fs_mount failed");
        return NULL;
    }
    fs_handle->fs_mounted = true;

    // get available size
    uint32_t fs_total_size = 0, fs_free_size = 0;
    if (esp_at_get_fs_info(&fs_total_size, &fs_free_size) != ESP_OK) {
        free(fs_handle);
        esp_at_fs_unmount();
        ESP_AT_LOGE(TAG, "esp_at_get_fs_info failed");
        return NULL;
    }
    fs_handle->total_size = fs_total_size;
    fs_handle->available_size = fs_free_size;
    ESP_AT_LOGI(TAG, "fs available size:%u, total size:%u", fs_handle->available_size, fs_handle->total_size);

    // init path
    fs_handle->path = (char *)calloc(1, strlen(esp_at_fs_get_mount_point()) + strlen(path) + 2);
    if (!fs_handle->path) {
        free(fs_handle);
        esp_at_fs_unmount();
        return NULL;
    }
    sprintf(fs_handle->path, "%s/%s", esp_at_fs_get_mount_point(), path);

    // open file
    remove(fs_handle->path);
    fs_handle->fp = fopen(fs_handle->path, "wb");
    if (!fs_handle->fp) {
        free(fs_handle->path);
        free(fs_handle);
        esp_at_fs_unmount();
        ESP_AT_LOGE(TAG, "fopen failed");
        return NULL;
    }

    return fs_handle;
}

esp_err_t at_http_to_fs_write(at_write_fs_handle_t *fs_handle, uint8_t *data, size_t len)
{
    if (!fs_handle || !fs_handle->fp) {
        ESP_AT_LOGE(TAG, "invalid argument");
        return ESP_ERR_INVALID_ARG;
    }

    if (fseek(fs_handle->fp, fs_handle->wrote_size, SEEK_SET) != 0) {
        ESP_AT_LOGE(TAG, "fseek failed");
        return ESP_FAIL;
    }

    size_t wrote_len = fwrite(data, 1, len, fs_handle->fp);
    if (wrote_len != len) {
        ESP_AT_LOGE(TAG, "fwrite failed, to write len=%d, wrote len=%d", len, wrote_len);
        return ESP_FAIL;
    }

    fs_handle->wrote_size += len;
    return ESP_OK;
}

static void at_http_to_fs_clean(void)
{
    if (sp_http_to_fs) {
        // http client
        if (sp_http_to_fs->sync_sema) {
            vSemaphoreDelete(sp_http_to_fs->sync_sema);
            sp_http_to_fs->sync_sema = NULL;
        }
        if (sp_http_to_fs->url) {
            free(sp_http_to_fs->url);
            sp_http_to_fs->url = NULL;
        }
        if (sp_http_to_fs->client) {
            esp_http_client_cleanup(sp_http_to_fs->client);
            sp_http_to_fs->client = NULL;
        }

        // file system
        if (sp_http_to_fs->fs_handle) {
            if (sp_http_to_fs->fs_handle->fp) {
                fflush(sp_http_to_fs->fs_handle->fp);
                fclose(sp_http_to_fs->fs_handle->fp);
                sp_http_to_fs->fs_handle->fp = NULL;
            }
            if (sp_http_to_fs->fs_handle->path) {
                free(sp_http_to_fs->fs_handle->path);
                sp_http_to_fs->fs_handle->path = NULL;
            }
            if (sp_http_to_fs->fs_handle->fs_mounted) {
                esp_at_fs_unmount();
                sp_http_to_fs->fs_handle->fs_mounted = false;
            }
            free(sp_http_to_fs->fs_handle);
            sp_http_to_fs->fs_handle = NULL;
        }

        // itself
        free(sp_http_to_fs);
        sp_http_to_fs = NULL;
    }
}

static void at_custom_wait_data_cb(void)
{
    xSemaphoreGive(sp_http_to_fs->sync_sema);
}

static esp_err_t at_http_event_handler(esp_http_client_event_t *evt)
{
    ESP_AT_LOGI(TAG, "http event id=%d", evt->event_id);
    switch (evt->event_id) {
    case HTTP_EVENT_ON_HEADER:
        if (strcmp(evt->header_key, "Content-Length") == 0) {
            sp_http_to_fs->total_size = atoi(evt->header_value);
            ESP_AT_LOGI(TAG, "total_size=%d", sp_http_to_fs->total_size);
            sp_http_to_fs->is_chunked = false;
        }
        break;
    case HTTP_EVENT_ON_DATA:
        sp_http_to_fs->recv_size += evt->data_len;
        // chunked check
        if (sp_http_to_fs->is_chunked) {
            ESP_AT_LOGI(TAG, "received total len=%d", sp_http_to_fs->recv_size);
        } else {
            ESP_AT_LOGI(TAG, "total_len=%d(%d), %0.1f%%!", sp_http_to_fs->total_size,
                        sp_http_to_fs->recv_size, (sp_http_to_fs->recv_size * 1.0) * 100 / sp_http_to_fs->total_size);
        }
        break;
    default:
        break;
    }

    return ESP_OK;
}

static uint8_t at_setup_cmd_httpget_to_fs(uint8_t para_num)
{
    esp_err_t ret = ESP_OK;
    int32_t cnt = 0, url_len = 0;
    uint8_t *dst_path = NULL;

    // dst file path
    if (esp_at_get_para_as_str(cnt++, &dst_path) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (at_str_is_null(dst_path)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // url len
    if (esp_at_get_para_as_digit(cnt++, &url_len) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (url_len <= 0 || url_len > AT_URL_LEN_MAX) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // cmd timeout
    int32_t elapsed_ms = 0, cmd_timeout_ms = 0;
    int64_t start_ms = esp_timer_get_time() / 1000;
    if (cnt < para_num) {
        if (esp_at_get_para_as_digit(cnt++, &cmd_timeout_ms) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (cmd_timeout_ms < 0) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    sp_http_to_fs = (at_httpget_to_fs_t *)calloc(1, sizeof(at_httpget_to_fs_t));
    if (!sp_http_to_fs) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }

    // init resources
    sp_http_to_fs->fs_handle = at_http_to_fs_begin((char *)dst_path);
    sp_http_to_fs->url = (char *)calloc(1, url_len + 1);
    sp_http_to_fs->sync_sema = xSemaphoreCreateBinary();

    if (!sp_http_to_fs->fs_handle || !sp_http_to_fs->url || !sp_http_to_fs->sync_sema) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }

    // receive url from AT port
    int32_t had_recv_len = 0;
    esp_at_port_enter_specific(at_custom_wait_data_cb);
    esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);
    while (xSemaphoreTake(sp_http_to_fs->sync_sema, portMAX_DELAY)) {
        had_recv_len += esp_at_port_read_data((uint8_t *)(sp_http_to_fs->url) + had_recv_len, url_len - had_recv_len);
        if (had_recv_len == url_len) {
            ESP_AT_LOGI(TAG, "recv %d bytes", had_recv_len);
            esp_at_port_exit_specific();

            int32_t remain_len = esp_at_port_get_data_length();
            if (remain_len > 0) {
                esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
            }
            break;
        }
    }
    ESP_AT_LOGI(TAG, "ready to download %s to %s", sp_http_to_fs->url, sp_http_to_fs->fs_handle->path);

    // init http client
    esp_http_client_config_t config = {
        .url = (const char*)sp_http_to_fs->url,
        .event_handler = at_http_event_handler,
        .timeout_ms = ((cmd_timeout_ms < AT_NETWORK_TIMEOUT_MS) ? cmd_timeout_ms : AT_NETWORK_TIMEOUT_MS),
        .buffer_size_tx = 4096,
    };
    sp_http_to_fs->is_chunked = true;
    sp_http_to_fs->client = esp_http_client_init(&config);
    if (!sp_http_to_fs->client) {
        ret = ESP_FAIL;
        goto cmd_exit;
    }
    esp_http_client_set_method(sp_http_to_fs->client, HTTP_METHOD_GET);

    // establish http connection
    ret = esp_http_client_open(sp_http_to_fs->client, 0);
    if (ret != ESP_OK) {
        goto cmd_exit;
    }
    esp_http_client_fetch_headers(sp_http_to_fs->client);
    int status_code = esp_http_client_get_status_code(sp_http_to_fs->client);
    if (status_code >= HttpStatus_BadRequest) {
        ESP_AT_LOGE(TAG, "recv http status code: %d", status_code);
        ret = ESP_FAIL;
        goto cmd_exit;
    }
    if (sp_http_to_fs->fs_handle->available_size < sp_http_to_fs->total_size) {
        ESP_AT_LOGE(TAG, "fs available size:%u, but res total size:%d", sp_http_to_fs->fs_handle->available_size, sp_http_to_fs->total_size);
        ret = ESP_FAIL;
        goto cmd_exit;
    }

    // download data to file
    int data_len = 0;
    uint8_t *data = (uint8_t *)malloc(AT_HEAP_BUFFER_SIZE);
    if (!data) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }
    do {
        // check timeout
        if (cmd_timeout_ms > 0) {
            elapsed_ms = esp_timer_get_time() / 1000 - start_ms;
            if (elapsed_ms >= cmd_timeout_ms) {
                ESP_AT_LOGE(TAG, "command timeout reached %u ms", cmd_timeout_ms);
                ret = ESP_ERR_TIMEOUT;
                break;
            }
        }

        // read data from http client and write to fs
        data_len = esp_http_client_read(sp_http_to_fs->client, (char *)data, AT_HEAP_BUFFER_SIZE);
        if (data_len > 0) {
            ret = at_http_to_fs_write(sp_http_to_fs->fs_handle, data, data_len);
            if (ret != ESP_OK) {
                break;
            }
        } else if (data_len < 0) {
            if (data_len == -ESP_ERR_HTTP_EAGAIN) {
                ESP_LOGD(TAG, "ESP_ERR_HTTP_EAGAIN invoked: Call timed out before data was ready");
                continue;
            }
            ESP_AT_LOGE(TAG, "Connection aborted!");
            break;
        } else {
            ret = ESP_OK;
            break;
        }
    } while (ret == ESP_OK && (data_len > 0 || data_len == -ESP_ERR_HTTP_EAGAIN));
    free(data);

    uint32_t expected_size = sp_http_to_fs->is_chunked ? sp_http_to_fs->recv_size : sp_http_to_fs->total_size;
    if (sp_http_to_fs->fs_handle->wrote_size != expected_size || expected_size == 0) {
        ESP_AT_LOGE(TAG, "expected to write size:%d, but actual wrote size:%d", expected_size, sp_http_to_fs->fs_handle->wrote_size);
        ret = ESP_FAIL;
    } else {
        ESP_AT_LOGI(TAG, "file download completed, total wrote size:%d", sp_http_to_fs->fs_handle->wrote_size);
    }

cmd_exit:
    // clean resources
    at_http_to_fs_clean();
    if (ret != ESP_OK) {
        ESP_AT_LOGE(TAG, "command ret: 0x%x", ret);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

static const esp_at_cmd_struct at_custom_cmd[] = {
    {"+HTTPGET_TO_FS", NULL, NULL, at_setup_cmd_httpget_to_fs, NULL},
    /**
     * @brief You can define your own AT commands here.
     */
};

bool esp_at_httpget_to_fs_cmd_register(void)
{
    return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
}

ESP_AT_CMD_SET_INIT_FN(esp_at_httpget_to_fs_cmd_register, 1);
