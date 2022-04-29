/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019-2025 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_at_core.h"
#include "esp_at.h"

#ifdef CONFIG_AT_USER_COMMAND_SUPPORT

#define AT_USERRAM_READ_BUFFER_SIZE     1024
#define AT_USEROTA_URL_LEN_MAX          (8 * 1024)
#define AT_USERDOCS_BUFFER_LEN_MAX      (1024)
#define AT_DOCS_SERVER_HOSTNAME         "docs.espressif.com"
#define AT_DOCS_PROJECT_PATH            "projects/esp-at"
#define AT_DOCS_LANGUAGE_EN             "en"
#define AT_DOCS_LANGUAGE_CN             "zh_CN"
#define AT_DOCS_VERSION                 ESP_AT_DOCS_VERSION
#define AT_DOCS_HOME_WEB_PAGE           "index.html"

typedef enum {
    AT_USERRAM_FREE = 0,
    AT_USERRAM_MALLOC,
    AT_USERRAM_WRITE,
    AT_USERRAM_READ,
    AT_USERRAM_CLEAR,
    AT_USERRAM_MAX,
} at_userram_op_t;

static uint8_t *sp_user_ram = NULL;
static uint32_t s_user_ram_size = 0;
static int32_t s_user_ota_total_size = 0;
static int32_t s_user_ota_recv_size = 0;
static bool s_user_ota_is_chunked = true;
static xSemaphoreHandle s_at_user_sync_sema;

static void at_user_wait_data_cb(void)
{
    xSemaphoreGive(s_at_user_sync_sema);
}

static uint8_t at_setup_cmd_userram(uint8_t para_num)
{
#define HEAD_BUFFER_SIZE    32
    int32_t cnt = 0, operator = 0, length = 0, offset = 0;

    // operator
    if (esp_at_get_para_as_digit(cnt++, &operator) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (operator < AT_USERRAM_FREE || operator >= AT_USERRAM_MAX) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // length
    if (operator == AT_USERRAM_MALLOC || operator == AT_USERRAM_WRITE || operator == AT_USERRAM_READ) {
        if (esp_at_get_para_as_digit(cnt++, &length) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (length <= 0) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    // offset
    if (operator == AT_USERRAM_WRITE || operator == AT_USERRAM_READ) {
        if (cnt != para_num) {
            if (esp_at_get_para_as_digit(cnt++, &offset) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            if (offset < 0) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
    }

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    switch (operator) {
    // free
    case AT_USERRAM_FREE:
        if (sp_user_ram == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        free(sp_user_ram);
        sp_user_ram = NULL;
        s_user_ram_size = 0;
        break;

    // malloc
    case AT_USERRAM_MALLOC:
        if (sp_user_ram != NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        sp_user_ram = malloc(length);
        if (sp_user_ram == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        s_user_ram_size = length;
        break;

    // write
    case AT_USERRAM_WRITE: {
        if (sp_user_ram == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (offset + length > s_user_ram_size) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if (!s_at_user_sync_sema) {
            s_at_user_sync_sema = xSemaphoreCreateBinary();
            if (!s_at_user_sync_sema) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
        uint32_t had_written_len = 0;
        esp_at_port_enter_specific(at_user_wait_data_cb);
        esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);

        // receive at cmd port data to user ram
        while (xSemaphoreTake(s_at_user_sync_sema, portMAX_DELAY)) {
            had_written_len += esp_at_port_read_data(sp_user_ram + offset + had_written_len, length - had_written_len);
            if (had_written_len == length) {
                printf("Recv %d bytes\r\n", had_written_len);
                esp_at_port_exit_specific();
                esp_at_port_write_data((uint8_t *)"\r\nWRITE OK\r\n", strlen("\r\nWRITE OK\r\n"));
                had_written_len = esp_at_port_get_data_length();

                if (had_written_len > 0) {
                    esp_at_port_recv_data_notify(had_written_len, portMAX_DELAY);
                }
                break;
            }
        }
        vSemaphoreDelete(s_at_user_sync_sema);
        s_at_user_sync_sema = NULL;
        return ESP_AT_RESULT_CODE_PROCESS_DONE;
    }

    // read
    case AT_USERRAM_READ: {
        if (sp_user_ram == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (offset + length > s_user_ram_size) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        uint8_t *pbuffer = calloc(1, at_min(AT_USERRAM_READ_BUFFER_SIZE, length) + HEAD_BUFFER_SIZE);
        if (pbuffer == NULL) {
            printf("no mem %d\r\n", at_min(AT_USERRAM_READ_BUFFER_SIZE, length) + HEAD_BUFFER_SIZE);
            return ESP_AT_RESULT_CODE_ERROR;
        }
        printf("to read %d bytes\r\n", length);

        uint32_t head_len = 0, had_read_len = 0, to_read_len = 0;
        do {
            to_read_len = at_min(length - had_read_len, AT_USERRAM_READ_BUFFER_SIZE);
            head_len = snprintf((char *)pbuffer, HEAD_BUFFER_SIZE, "%s:%d,", esp_at_get_current_cmd_name(), to_read_len);
            memcpy((char *)pbuffer + head_len, (char *)sp_user_ram + offset + had_read_len, to_read_len);
            esp_at_port_write_data(pbuffer, head_len + to_read_len);
            had_read_len += to_read_len;
        } while (had_read_len < length);
        free(pbuffer);

        break;
    }

    // clear
    case AT_USERRAM_CLEAR:
        if (sp_user_ram == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        memset(sp_user_ram, 0x0, s_user_ram_size);
        break;

    default:
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_query_cmd_userram(uint8_t *cmd_name)
{
#define TEMP_BUFFER_SIZE    32
    uint8_t buffer[TEMP_BUFFER_SIZE] = {0};
    snprintf((char *)buffer, TEMP_BUFFER_SIZE, "%s:%d\r\n", cmd_name, s_user_ram_size);
    esp_at_port_write_data(buffer, strlen((char *)buffer));
    return ESP_AT_RESULT_CODE_OK;
}

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
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

        // get OTA image size
        if (strcmp(evt->header_key, "Content-Length") == 0) {
            s_user_ota_total_size = atoi(evt->header_value);
            s_user_ota_is_chunked = false;
        }

        break;
    case HTTP_EVENT_ON_DATA:
        s_user_ota_recv_size += evt->data_len;

        // chunked check
        if (s_user_ota_is_chunked) {
            printf("receive len=%d, receive total len=%d\r\n", evt->data_len, s_user_ota_recv_size);
        } else {
            printf("total_len=%d(%d), %0.1f%%!\r\n", s_user_ota_total_size, s_user_ota_recv_size, (s_user_ota_recv_size*1.0) * 100 / s_user_ota_total_size);
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        printf("http(https) finished\r\n");
        break;
    case HTTP_EVENT_DISCONNECTED:
        printf("http(https) disconnected\r\n");
        break;
    }

    return ESP_OK;
}

static uint8_t at_setup_cmd_userota(uint8_t para_num)
{
#define TEMP_BUFFER_SIZE    32
    uint8_t buffer[TEMP_BUFFER_SIZE] = {0};
    int32_t length = 0;
    int32_t cnt = 0;

    // length
    if (esp_at_get_para_as_digit(cnt++, &length) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((length <= 0) || (length > AT_USEROTA_URL_LEN_MAX)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    uint8_t *url = (uint8_t *)calloc(1, length + 1);
    if (url == NULL) {
        printf("no mem %d\r\n", length);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!s_at_user_sync_sema) {
        s_at_user_sync_sema = xSemaphoreCreateBinary();
        if (!s_at_user_sync_sema) {
            free(url);
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    int32_t had_received_len = 0;
    esp_at_port_enter_specific(at_user_wait_data_cb);
    esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);

    // receive at cmd port data
    while (xSemaphoreTake(s_at_user_sync_sema, portMAX_DELAY)) {
        had_received_len += esp_at_port_read_data(url + had_received_len, length - had_received_len);
        if (had_received_len == length) {
            esp_at_port_exit_specific();

            snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\nRecv %d bytes\r\n", length);
            esp_at_port_write_data(buffer, strlen((char *)buffer));

            had_received_len = esp_at_port_get_data_length();
            if (had_received_len > 0) {
                snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\nbusy p...\r\n");
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }

            break;
        }
    }

    printf("url is: %s\r\n", url);
    vSemaphoreDelete(s_at_user_sync_sema);
    s_at_user_sync_sema = NULL;

    s_user_ota_total_size = 0;
    s_user_ota_recv_size = 0;
    s_user_ota_is_chunked = true;

    esp_http_client_config_t config = {
        .url = (const char*)url,
        .event_handler = _http_event_handler,
        .keep_alive_enable = true,
        .buffer_size = 2048,
    };

    esp_err_t ret = esp_https_ota(&config);

    free(url);

    if (ret == ESP_OK) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(ESP_AT_PORT_TX_WAIT_MS_MAX);
        esp_restart();
        for(;;){
        }
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }
}

static uint8_t at_query_cmd_userdocs(uint8_t *cmd_name)
{
    int ret = 0;
    uint8_t *buffer = calloc(1, AT_USERDOCS_BUFFER_LEN_MAX);
    if (!buffer) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // https:<hostname>/<project>/<language>/<version>/<target>/<home_web_page>
    ret += snprintf((char *)buffer + ret, AT_USERDOCS_BUFFER_LEN_MAX - ret, "%s:\"https://%s/%s/%s/%s/%s/%s\"\r\n",
        cmd_name, AT_DOCS_SERVER_HOSTNAME, AT_DOCS_PROJECT_PATH, AT_DOCS_LANGUAGE_EN,
        AT_DOCS_VERSION, CONFIG_IDF_TARGET, AT_DOCS_HOME_WEB_PAGE);

    ret += snprintf((char *)buffer + ret, AT_USERDOCS_BUFFER_LEN_MAX - ret, "%s:\"https://%s/%s/%s/%s/%s/%s\"\r\n",
        cmd_name, AT_DOCS_SERVER_HOSTNAME, AT_DOCS_PROJECT_PATH, AT_DOCS_LANGUAGE_CN,
        AT_DOCS_VERSION, CONFIG_IDF_TARGET, AT_DOCS_HOME_WEB_PAGE);

    esp_at_port_write_data(buffer, ret);
    free(buffer);

    return ESP_AT_RESULT_CODE_OK;
}

static const esp_at_cmd_struct s_at_user_cmd[] = {
    {"+USERRAM", NULL, at_query_cmd_userram, at_setup_cmd_userram, NULL},
    {"+USEROTA", NULL, NULL, at_setup_cmd_userota, NULL},
    {"+USERDOCS", NULL, at_query_cmd_userdocs, NULL, NULL},
};

bool esp_at_user_cmd_regist(void)
{
    return esp_at_custom_cmd_array_regist(s_at_user_cmd, sizeof(s_at_user_cmd) / sizeof(s_at_user_cmd[0]));
}

#endif
