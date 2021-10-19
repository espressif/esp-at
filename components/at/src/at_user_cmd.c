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

#include "esp_at_core.h"
#include "esp_at.h"

#ifdef CONFIG_AT_USER_COMMAND_SUPPORT

#define AT_USERRAM_READ_BUFFER_SIZE     1024
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
static xSemaphoreHandle s_at_userram_sync_sema;

static void at_userram_wait_data_cb(void)
{
    xSemaphoreGive(s_at_userram_sync_sema);
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
            if (esp_at_get_para_as_digit(cnt++, &offset) != ESP_AT_PARA_PARSE_RESULT_FAIL) {
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
        esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);

        if (!s_at_userram_sync_sema) {
            s_at_userram_sync_sema = xSemaphoreCreateBinary();
            if (!s_at_userram_sync_sema) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
        uint32_t had_written_len = 0;
        esp_at_port_enter_specific(at_userram_wait_data_cb);

        // receive at cmd port data to user ram
        while (xSemaphoreTake(s_at_userram_sync_sema, portMAX_DELAY)) {
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
        vSemaphoreDelete(s_at_userram_sync_sema);
        s_at_userram_sync_sema = NULL;
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

static const esp_at_cmd_struct s_at_user_cmd[] = {
    {"+USERRAM", NULL, at_query_cmd_userram, at_setup_cmd_userram, NULL},
};

bool esp_at_user_cmd_regist(void)
{
    return esp_at_custom_cmd_array_regist(s_at_user_cmd, sizeof(s_at_user_cmd) / sizeof(s_at_user_cmd[0]));
}

#endif
