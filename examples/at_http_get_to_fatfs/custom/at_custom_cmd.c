/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_vfs_fat.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_at.h"
#include "esp_timer.h"

// Add these variables to store time measurements
int64_t start_download_time, end_download_time;
int64_t start_write_time, end_write_time;


#ifndef CONFIG_AT_FS_COMMAND_SUPPORT
#error "CONFIG_AT_FS_COMMAND_SUPPORT is undefined, please enable it first: python build.py menuconfig > Component config > AT > AT FS command support"
#endif

#define AT_NETWORK_TIMEOUT_MS       (5000)
#define AT_URL_LEN_MAX              (8 * 1024)
#define AT_HEAP_BUFFER_SIZE         4096
#define AT_FATFS_MOUNT_POINT        "/fatfs"

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
        ESP_LOGE(TAG, "calloc failed");
        return NULL;
    }

    // mount file system
    if (!at_fatfs_mount()) {
        free(fs_handle);
        ESP_LOGE(TAG, "at_fatfs_mount failed");
        return NULL;
    }
    fs_handle->fs_mounted = true;

    // get available size
    uint64_t fs_total_size = 0, fs_free_size = 0;
    if (esp_vfs_fat_info(AT_FATFS_MOUNT_POINT, &fs_total_size, &fs_free_size) != ESP_OK) {
        free(fs_handle);
        at_fatfs_unmount();
        ESP_LOGE(TAG, "esp_vfs_fat_info failed");
        return NULL;
    }
    fs_handle->total_size = fs_total_size;
    fs_handle->available_size = fs_free_size;
    printf("fatfs available size:%u, total size:%u\r\n", fs_handle->available_size, fs_handle->total_size);

    // init path
    fs_handle->path = (char *)calloc(1, strlen(AT_FATFS_MOUNT_POINT) + strlen(path) + 2);
    if (!fs_handle->path) {
        free(fs_handle);
        at_fatfs_unmount();
        ESP_LOGE(TAG, "calloc failed");
        return NULL;
    }
    sprintf(fs_handle->path, "%s/%s", AT_FATFS_MOUNT_POINT, path);

    // open file
    remove(fs_handle->path);
    fs_handle->fp = fopen(fs_handle->path, "wb");
    if (!fs_handle->fp) {
        free(fs_handle);
        at_fatfs_unmount();
        ESP_LOGE(TAG, "fopen failed");
        return NULL;
    }

    return fs_handle;
}

esp_err_t at_http_to_fs_write(at_write_fs_handle_t *fs_handle, uint8_t *data, size_t len)
{
    if (!fs_handle || !fs_handle->fp) {
        //ESP_LOGE(TAG, "invalid argument");
        return ESP_ERR_INVALID_ARG;
    }

    if (fseek(fs_handle->fp, fs_handle->wrote_size, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "fseek failed");
        return ESP_FAIL;
    }

    size_t wrote_len = fwrite(data, 1, len, fs_handle->fp);
    if (wrote_len != len) {
        //ESP_LOGE(TAG, "fwrite failed, to write len=%d, wrote len=%d", len, wrote_len);
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
                fclose(sp_http_to_fs->fs_handle->fp);
                sp_http_to_fs->fs_handle->fp = NULL;
            }
            if (sp_http_to_fs->fs_handle->path) {
                free(sp_http_to_fs->fs_handle->path);
                sp_http_to_fs->fs_handle->path = NULL;
            }
            if (sp_http_to_fs->fs_handle->fs_mounted) {
                at_fatfs_unmount();
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
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        printf("http(https) error\r\n");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        printf("http(https) connected\r\n");
        break;
    case HTTP_EVENT_HEADER_SENT:
        printf("http(https) header sent\r\n");
        break;
    case HTTP_EVENT_ON_HEADER:
        printf("http(https) headed key=%s, value=%s\r\n", evt->header_key, evt->header_value);
        if (strcmp(evt->header_key, "Content-Length") == 0) {
            sp_http_to_fs->total_size = atoi(evt->header_value);
            printf("total_size=%d\r\n", sp_http_to_fs->total_size);
            sp_http_to_fs->is_chunked = false;
        }
        break;
    case HTTP_EVENT_ON_DATA:
        sp_http_to_fs->recv_size += evt->data_len;

        // chunked check
        if (sp_http_to_fs->is_chunked) {
            printf("received total len=%d\r\n", sp_http_to_fs->recv_size);
        } else {
            printf("total_len=%d(%d), %0.1f%%!\r\n", sp_http_to_fs->total_size,
                   sp_http_to_fs->recv_size, (sp_http_to_fs->recv_size * 1.0) * 100 / sp_http_to_fs->total_size);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        printf("http(https) finished\r\n");
        break;
    case HTTP_EVENT_DISCONNECTED:
        printf("http(https) disconnected\r\n");
        break;
    default:
        break;
    }

    return ESP_OK;
}

void check_heap_memory() {
    size_t free_heap_size = esp_get_free_heap_size();
    printf("Free heap size: %d bytes", free_heap_size);
}

int prepare_file(FILE* file, long desired_size) {
    // Move file pointer to desired size
    if (fseek(file, desired_size, SEEK_SET) != 0) {
        return -1; // Error
    }

    // Truncate file to this size
    if (ftruncate(fileno(file), desired_size) != 0) {
        return -1; // Error
    }

    // Reset file pointer to start
    if (fseek(file, 0, SEEK_SET) != 0) {
        return -1; // Error
    }

    return 0; // Success
}

static uint8_t at_setup_cmd_httpget_to_fs(uint8_t para_num)
{
    esp_err_t ret = ESP_OK;
    int32_t cnt = 0, url_len = 0, http_buffer_size = 0, file_write_buffer_size = 0;
    uint8_t *dst_path = NULL;
    bool is_write_file = true;

    // dst file path
    if (esp_at_get_para_as_str(cnt++, &dst_path) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (at_str_is_null(dst_path)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (strcmp((const char *)dst_path, "null") == 0) {
        is_write_file = false;
    }

    // url len
    if (esp_at_get_para_as_digit(cnt++, &url_len) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (url_len <= 0 || url_len > AT_URL_LEN_MAX) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // http buffer size
    if (esp_at_get_para_as_digit(cnt++, &http_buffer_size) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (http_buffer_size <= 0) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // file write buffer size
    if (esp_at_get_para_as_digit(cnt++, &file_write_buffer_size) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (file_write_buffer_size <= 0) {
        return ESP_AT_RESULT_CODE_ERROR;
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
            printf("Recv %d bytes\r\n", had_recv_len);
            esp_at_port_exit_specific();

            int32_t remain_len = esp_at_port_get_data_length();
            if (remain_len > 0) {
                esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
            }
            break;
        }
    }
    printf("ready to download %s to %s\r\n", sp_http_to_fs->url, sp_http_to_fs->fs_handle->path);

    check_heap_memory();

    // init http client
    esp_http_client_config_t config = {
            .url = (const char*)sp_http_to_fs->url,
            .event_handler = at_http_event_handler,
            .timeout_ms = AT_NETWORK_TIMEOUT_MS,
            .buffer_size = http_buffer_size,
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
        ESP_LOGE(TAG, "recv http status code: %d", status_code);
        ret = ESP_FAIL;
        goto cmd_exit;
    }
    if (sp_http_to_fs->fs_handle->available_size < sp_http_to_fs->total_size) {
        ESP_LOGE(TAG, "fatfs available size:%u, but res total size:%d", sp_http_to_fs->fs_handle->available_size, sp_http_to_fs->total_size);
        ret = ESP_FAIL;
        goto cmd_exit;
    }

    prepare_file(sp_http_to_fs->fs_handle->fp, sp_http_to_fs->total_size);

    // Start download time measurement
    start_download_time = esp_timer_get_time();

    // download data to file
    int data_len = 0;
    uint8_t *data = (uint8_t *)malloc(file_write_buffer_size);
    if (!data) {
        ret = ESP_ERR_NO_MEM;
        goto cmd_exit;
    }
    do {
        data_len = esp_http_client_read(sp_http_to_fs->client, (char *)data, file_write_buffer_size);
        if (data_len > 0) {
            // Start write time measurement
            start_write_time = esp_timer_get_time();

            if (is_write_file) {
                ret = at_http_to_fs_write(sp_http_to_fs->fs_handle, data, data_len);
            }
            // End write time measurement
            end_write_time = esp_timer_get_time();
            printf("Time to write %d bytes: %ld ms\r\n", data_len, (int32_t )((end_write_time - start_write_time)/1000));

            if (ret != ESP_OK) {
                break;
            }
        } else if (data_len < 0) {
            ESP_LOGE(TAG, "Connection aborted!");
            break;
        } else {
            printf("Connection closed\r\n");
            ret = ESP_OK;
            break;
        }
    } while (ret == ESP_OK && data_len > 0);
    free(data);

    // End download time measurement
    end_download_time = esp_timer_get_time();
    int64_t total_download_time = end_download_time - start_download_time;
    printf("Total download time: %ld ms\r\n", (int32_t )(total_download_time/1000));

    if (sp_http_to_fs->is_chunked) {
        printf("total received len:%d, total wrote size:%d\r\n", sp_http_to_fs->recv_size, sp_http_to_fs->fs_handle->wrote_size);
    } else {
        if (sp_http_to_fs->total_size != sp_http_to_fs->fs_handle->wrote_size) {
            ESP_LOGE(TAG, "total expected len:%d, but total wrote size:%d", sp_http_to_fs->total_size, sp_http_to_fs->fs_handle->wrote_size);
            ret = ESP_FAIL;
        } else {
            printf("total wrote size matches expected size:%d\r\n", sp_http_to_fs->fs_handle->wrote_size);
        }
    }

    // Calculate and print download speed
    double download_speed = (sp_http_to_fs->recv_size * 1.0) / (total_download_time / 1000000.0); // bytes per second
    printf("Download speed: %.2f bytes/second\r\n", download_speed);

    cmd_exit:
    // clean resources
    at_http_to_fs_clean();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "command ret: 0x%x", ret);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

#define HUGE_SCAN_SIMULATION

#ifdef HUGE_SCAN_SIMULATION
char *huge_scan_simulation_data[] = {
        "AT+CWLAP=,,,,0,0\r\n",
        "+CWLAP:(\"Skunks\",-30,\"30:57:8e:28:2f:06\",1)\r\n",
        "+CWLAP:(\"\",-31,\"30:57:8e:28:2f:09\",1)\r\n",
        "+CWLAP:(\"\",-31,\"30:57:8e:28:2f:03\",1)\r\n",
        "+CWLAP:(\"TMOBILE-D4A1\",-38,\"46:05:3f:54:d4:a6\",6)\r\n",  // Skip:ssid&LOCALLY_ADMINISTERED_MAC_ADDRESS
        "+CWLAP:(\"it’s something weird\",-55,\"d8:47:32:fc:7c:e9\",10)\r\n",
        "+CWLAP:(\"\",-55,\"de:47:32:fc:7c:e9\",10)\r\n",  // Skip:ssid&LOCALLY_ADMINISTERED_MAC_ADDRESS
        "+CWLAP:(\"Skunks_24\",-58,\"00:14:d1:df:b3:88\",1)\r\n",
        "+CWLAP:(\"TB Proprietary Channel. Vl\",-59,\"00:04:32:25:6b:d9\",11)\r\n",
        "+CWLAP:(\"The Wifi\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi10\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi11\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi12\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi13\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi14\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi15\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi16\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi17\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi18\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi19\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi20\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi21\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi22\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi23\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi24\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi25\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi26\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi27\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi28\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi29\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi30\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi31\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi32\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi33\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi34\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi35\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi36\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi37\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi38\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi39\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi40\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi41\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi42\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi43\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi44\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi45\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi46\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi47\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi48\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi49\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi50\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi51\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi52\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi53\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi54\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi55\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi56\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi57\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi58\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi59\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi60\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi61\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi62\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi63\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi64\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi65\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi66\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi67\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi68\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi69\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi70\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi71\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi72\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi73\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi74\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi75\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi76\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi77\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi78\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi79\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi80\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi81\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi82\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi83\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi84\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi85\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi86\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi87\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi88\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi89\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi90\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi91\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi92\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi93\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi94\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi95\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi96\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi97\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi98\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi99\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi100\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi101\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi102\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi103\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi104\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi105\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi106\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi107\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi108\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi109\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi110\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi111\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi112\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi113\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi114\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi115\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi116\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi117\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi118\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi119\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi120\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "+CWLAP:(\"Wifi121\",-60,\"bc:9b:68:be:63:1f\",1)\r\n",
        "OK\r\n"};

static uint8_t at_setup_cmd_huge_scan()
{
    printf("at_setup_cmd_huge_scan sending %lu bytes %lu lines\n", sizeof(huge_scan_simulation_data), sizeof(huge_scan_simulation_data) / sizeof(char *));

    for (int line = 0; line < sizeof(huge_scan_simulation_data) / sizeof(char *); line++) {
        int32_t length = esp_at_port_write_data((uint8_t *)huge_scan_simulation_data[line], strlen(huge_scan_simulation_data[line]));
        if (length == 0) {
            printf("esp_at_port_write_data failed\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }

        sys_msleep(10);
    }

    return ESP_AT_RESULT_CODE_OK;
}
#endif

#if 0
static uint8_t at_cmd_httpget_to_fs_test_cmd(uint8_t *cmd_name)
{
    uint8_t buffer[128] = {0};
    snprintf((char *)buffer, sizeof(buffer),
             "AT+HTTPGET_TO_FS=<\"dst_path\">,<url_len>,<http_buffer_size>,<file_write_buffer_size>\r\n");
    esp_at_port_write_data(buffer, strlen((char *)buffer));
    return ESP_AT_RESULT_CODE_OK;
}
#endif

static const esp_at_cmd_struct at_custom_cmd[] = {
    {"+HTTPGET_TO_FS", NULL, NULL, at_setup_cmd_httpget_to_fs, NULL},
#ifdef HUGE_SCAN_SIMULATION
    {"+HUGE_SCAN", NULL, NULL, NULL, at_setup_cmd_huge_scan},
#endif
/**
     * @brief You can define your own AT commands here.
     */
};

bool esp_at_custom_cmd_register(void)
{
    return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
}

ESP_AT_CMD_SET_INIT_FN(esp_at_custom_cmd_register, 1);
