/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_vfs_fat.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_at.h"

#ifndef CONFIG_AT_FS_COMMAND_SUPPORT
#error "CONFIG_AT_FS_COMMAND_SUPPORT is undefined, please enable it first: python build.py menuconfig > Component config > AT > AT FS command support"
#endif

#define AT_NETWORK_TIMEOUT_MS       (10000)
#define AT_URL_LEN_MAX              (8 * 1024)
#define AT_HEAP_BUFFER_SIZE         4096
#define AT_RESP_PREFIX_LEN_MAX      64
#define AT_FATFS_MOUNT_POINT        "/fatfs"

typedef struct {
    bool fs_mounted;                /*!< File system mounted */
    char *path;                     /*!< File path */
    FILE *fp;                       /*!< File pointer */
    uint32_t total_size;            /*!< The total size of the file system */
    uint32_t had_read_size;         /*!< The file size that has been written to flash */
} at_read_fs_handle_t;

typedef struct {
    char *url;                      /*!< URL */
    int32_t post_size;              /*!< Total size of the file to post */
    SemaphoreHandle_t sync_sema;    /*!< Semaphore for synchronization */
    esp_http_client_handle_t client;    /*!< HTTP client handle */
    at_read_fs_handle_t *fs_handle;     /*!< File system handle */
} at_fs_to_http_server_t;

// static variables
static at_fs_to_http_server_t *sp_fs_to_http;
static const char *TAG = "at_fs_to_http";

static at_read_fs_handle_t *at_fs_to_http_begin(char *path)
{
    // mount file system
    if (!at_fatfs_mount()) {
        ESP_LOGE(TAG, "at_fatfs_mount failed");
        return NULL;
    }

    // init handle
    at_read_fs_handle_t *fs_handle = (at_read_fs_handle_t *)calloc(1, sizeof(at_read_fs_handle_t));
    if (!fs_handle) {
        ESP_LOGE(TAG, "calloc failed");
        return NULL;
    }
    fs_handle->fs_mounted = true;

    // init path
    fs_handle->path = (char *)calloc(1, strlen(AT_FATFS_MOUNT_POINT) + strlen(path) + 2);
    if (!fs_handle->path) {
        free(fs_handle);
        at_fatfs_unmount();
        ESP_LOGE(TAG, "calloc failed");
        return NULL;
    }
    sprintf(fs_handle->path, "%s/%s", AT_FATFS_MOUNT_POINT, path);

    // get file size
    struct stat st;
    memset(&st, 0, sizeof(st));
    if (stat(fs_handle->path, &st) == -1) {
        ESP_LOGE(TAG, "stat(%s) failed\n", fs_handle->path);
        free(fs_handle->path);
        free(fs_handle);
        at_fatfs_unmount();
        return NULL;
    }
    fs_handle->total_size = st.st_size;

    // open file
    fs_handle->fp = fopen(fs_handle->path, "rb");
    if (!fs_handle->fp) {
        ESP_LOGE(TAG, "fopen(%s) failed", fs_handle->path);
        free(fs_handle->path);
        free(fs_handle);
        at_fatfs_unmount();
        return NULL;
    }

    return fs_handle;
}

static int at_fs_read(at_read_fs_handle_t *fs_handle, uint8_t *data, size_t len)
{
    if (!fs_handle || !fs_handle->fp || !data) {
        ESP_LOGE(TAG, "invalid argument");
        return -ESP_ERR_INVALID_ARG;
    }

    if (fseek(fs_handle->fp, fs_handle->had_read_size, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "fseek failed");
        return ESP_FAIL;
    }

    size_t had_read_size = fread(data, 1, len, fs_handle->fp);
    fs_handle->had_read_size += had_read_size;
    return had_read_size;
}

static void at_fs_to_http_clean(void)
{
    if (sp_fs_to_http) {
        // http client
        if (sp_fs_to_http->sync_sema) {
            vSemaphoreDelete(sp_fs_to_http->sync_sema);
            sp_fs_to_http->sync_sema = NULL;
        }
        if (sp_fs_to_http->url) {
            free(sp_fs_to_http->url);
            sp_fs_to_http->url = NULL;
        }
        if (sp_fs_to_http->client) {
            esp_http_client_cleanup(sp_fs_to_http->client);
            sp_fs_to_http->client = NULL;
        }

        // file system
        if (sp_fs_to_http->fs_handle) {
            if (sp_fs_to_http->fs_handle->fp) {
                fclose(sp_fs_to_http->fs_handle->fp);
                sp_fs_to_http->fs_handle->fp = NULL;
            }
            if (sp_fs_to_http->fs_handle->path) {
                free(sp_fs_to_http->fs_handle->path);
                sp_fs_to_http->fs_handle->path = NULL;
            }
            if (sp_fs_to_http->fs_handle->fs_mounted) {
                at_fatfs_unmount();
                sp_fs_to_http->fs_handle->fs_mounted = false;
            }
            free(sp_fs_to_http->fs_handle);
            sp_fs_to_http->fs_handle = NULL;
        }

        // itself
        free(sp_fs_to_http);
        sp_fs_to_http = NULL;
    }
}

static void at_custom_wait_data_cb(void)
{
    xSemaphoreGive(sp_fs_to_http->sync_sema);
}

static esp_err_t at_http_event_handler(esp_http_client_event_t *evt)
{
    int header_len = 0;
    uint8_t *data = NULL;
    ESP_LOGI(TAG, "http event id=%d", evt->event_id);

    switch (evt->event_id) {
    case HTTP_EVENT_ON_DATA:
        data = malloc(evt->data_len + AT_RESP_PREFIX_LEN_MAX);
        if (!data) {
            ESP_LOGE(TAG, "malloc failed");
            return ESP_ERR_NO_MEM;
        }
        header_len = snprintf((char *)data, AT_RESP_PREFIX_LEN_MAX, "%s:%d,", esp_at_get_current_cmd_name(), evt->data_len);
        memcpy(data + header_len, evt->data, evt->data_len);
        memcpy(data + header_len + evt->data_len, "\r\n", 2);
        esp_at_port_write_data(data, header_len + evt->data_len + 2);
        free(data);
        break;
    default:
        break;
    }

    return ESP_OK;
}

static uint8_t at_setup_cmd_fs_to_http_server(uint8_t para_num)
{
    esp_err_t ret = ESP_OK;
    int32_t cnt = 0, url_len = 0;
    uint8_t *dst_path = NULL, *data = NULL;

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

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    sp_fs_to_http = (at_fs_to_http_server_t *)calloc(1, sizeof(at_fs_to_http_server_t));
    if (!sp_fs_to_http) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }

    // init resources
    sp_fs_to_http->fs_handle = at_fs_to_http_begin((char *)dst_path);
    sp_fs_to_http->url = (char *)calloc(1, url_len + 1);
    sp_fs_to_http->sync_sema = xSemaphoreCreateBinary();

    if (!sp_fs_to_http->fs_handle || !sp_fs_to_http->url || !sp_fs_to_http->sync_sema) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }

    // receive url from AT port
    int32_t had_recv_len = 0;
    esp_at_port_enter_specific(at_custom_wait_data_cb);
    esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);
    while (xSemaphoreTake(sp_fs_to_http->sync_sema, portMAX_DELAY)) {
        had_recv_len += esp_at_port_read_data((uint8_t *)(sp_fs_to_http->url) + had_recv_len, url_len - had_recv_len);
        if (had_recv_len == url_len) {
            printf("Recv %d bytes\r\n", had_recv_len);
            esp_at_port_exit_specific();

            int32_t remain_len = esp_at_port_get_data_length();
            if (remain_len > 0) {
                esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
            }
            break;
        }
    }
    printf("ready to post %s (size:%d) to %s\r\n", sp_fs_to_http->fs_handle->path, sp_fs_to_http->fs_handle->total_size, sp_fs_to_http->url);

    // init http client
    esp_http_client_config_t config = {
        .url = (const char*)sp_fs_to_http->url,
        .event_handler = at_http_event_handler,
        .timeout_ms = AT_NETWORK_TIMEOUT_MS,
        .buffer_size_tx = 4096,
    };
    sp_fs_to_http->client = esp_http_client_init(&config);
    if (!sp_fs_to_http->client) {
        ret = ESP_FAIL;
        goto cmd_exit;
    }
    esp_http_client_set_method(sp_fs_to_http->client, HTTP_METHOD_POST);
    esp_http_client_set_header(sp_fs_to_http->client, "Content-Type", "binary/octet-stream");

    // establish http connection
    ret = esp_http_client_open(sp_fs_to_http->client, sp_fs_to_http->fs_handle->total_size);
    if (ret != ESP_OK) {
        goto cmd_exit;
    }

    // post file to remote server
    data = (uint8_t *)malloc(AT_HEAP_BUFFER_SIZE);
    if (!data) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }
    do {
        int unposted_len = sp_fs_to_http->fs_handle->total_size - sp_fs_to_http->fs_handle->had_read_size;
        int to_post_len = AT_HEAP_BUFFER_SIZE < unposted_len ? AT_HEAP_BUFFER_SIZE : unposted_len;
        to_post_len = at_fs_read(sp_fs_to_http->fs_handle, data, to_post_len);
        if (to_post_len <= 0) {
            ret = ESP_FAIL;
            break;
        }
        int wlen = esp_http_client_write(sp_fs_to_http->client, (char *)data, to_post_len);
        if (wlen > 0) {
            sp_fs_to_http->post_size += wlen;
            if (sp_fs_to_http->post_size == sp_fs_to_http->fs_handle->total_size) {
                ret = ESP_OK;
                break;
            }
        } else if (wlen < 0) {
            ret = ESP_FAIL;
            ESP_LOGE(TAG, "Connection aborted!");
            break;
        } else {
            ret = ESP_FAIL;
            ESP_LOGE(TAG, "esp_http_client_write() timeout!");
            break;
        }
    } while (1);

    // post over
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "total expected len:%d, but total post size:%d", sp_fs_to_http->fs_handle->total_size, sp_fs_to_http->post_size);
        goto cmd_exit;
    }
    ESP_LOGI(TAG, "total post size matches expected size:%d", sp_fs_to_http->fs_handle->total_size);

    // fetch response header
    int header_ret = esp_http_client_fetch_headers(sp_fs_to_http->client);
    if (header_ret < 0) {
        ret = header_ret;
        goto cmd_exit;
    }
    // status code
    int status_code = esp_http_client_get_status_code(sp_fs_to_http->client);
    if (status_code != HttpStatus_Ok) {
        ESP_LOGE(TAG, "recv http status code: %d", status_code);
        ret = -status_code;
        goto cmd_exit;
    }
    // process the server response
    ret = esp_http_client_perform(sp_fs_to_http->client);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_http_client_perform failed: 0x%x", ret);
        goto cmd_exit;
    }

cmd_exit:
    if (data) {
        free(data);
    }
    // clean resources
    at_fs_to_http_clean();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "command ret: 0x%x", ret);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

static const esp_at_cmd_struct at_custom_cmd[] = {
    {"+FS_TO_HTTP_SERVER", NULL, NULL, at_setup_cmd_fs_to_http_server, NULL},
    /**
     * @brief You can define your own AT commands here.
     */
};

bool esp_at_fs_to_http_server_cmd_register(void)
{
    return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
}

ESP_AT_CMD_SET_INIT_FN(esp_at_fs_to_http_server_cmd_register, 1);
