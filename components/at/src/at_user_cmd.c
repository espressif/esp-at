/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#endif

#ifdef CONFIG_BOOTLOADER_COMPRESSED_ENABLED
#include "at_compress_ota.h"
#endif

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

#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
typedef enum {
    CHECK_MCU_AWAKE_BY_MCU_SLP = 0,
    CHECK_MCU_AWAKE_BY_AT_SLP = 1,
    CHECK_MCU_AWAKE_BY_TIMEO = 2,
    CHECK_MCU_AWAKE_BY_GPIO = 3,
    CHECK_MCU_AWAKE_BY_MAX,
} at_check_mcu_awake_t;

typedef enum {
    WKMCU_MODE_MIN = 0,
    WKMCU_MODE_GPIO = 1,
    WKMCU_MODE_UART = 2,
    WKMCU_MODE_MAX,
} at_wkmcu_mode_t;

typedef struct {
    bool enable;
    at_wkmcu_mode_t wake_mode;
    uint8_t wake_number;
    uint8_t wake_signal;
    uint32_t delay_ms;
    uint32_t check_mcu_awake;
} at_wkmcu_cfg_t;

static bool s_mcu_sleep;
static at_wkmcu_cfg_t s_wkmcu_cfg;
static EventGroupHandle_t s_wkmcu_evt_group;

#define AT_MCU_AWAKE_ON_MCU_SLEEP       BIT(CHECK_MCU_AWAKE_BY_MCU_SLP)
#define AT_MCU_AWAKE_ON_AT_SLEEP        BIT(CHECK_MCU_AWAKE_BY_AT_SLP)
#define AT_MCU_AWAKE_ON_TIMEO           BIT(CHECK_MCU_AWAKE_BY_TIMEO)
#define AT_MCU_AWAKE_ON_GPIO            BIT(CHECK_MCU_AWAKE_BY_GPIO)    // unimplemented: check mcu awake state by gpio level
#define AT_MCU_AWAKE_BIT                (AT_MCU_AWAKE_ON_MCU_SLEEP | AT_MCU_AWAKE_ON_AT_SLEEP | AT_MCU_AWAKE_ON_TIMEO | AT_MCU_AWAKE_ON_GPIO)
#define AT_WKMCU_DELAY_MS_MAX           (60 * 1000) // 1 minute
#endif

static uint8_t *sp_user_ram = NULL;
static uint32_t s_user_ram_size = 0;
static int32_t s_user_ota_total_size = 0;
static int32_t s_user_ota_recv_size = 0;
static bool s_user_ota_is_chunked = true;
static SemaphoreHandle_t s_at_user_sync_sema;

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
            printf("total_len=%d(%d), %0.1f%%!\r\n", s_user_ota_total_size, s_user_ota_recv_size, (s_user_ota_recv_size * 1.0) * 100 / s_user_ota_total_size);
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
        .timeout_ms = 10000,
        .buffer_size = 2048,
    };

#ifdef CONFIG_BOOTLOADER_COMPRESSED_ENABLED
    esp_err_t ret = at_compress_https_ota(&config);
#else
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };

    esp_err_t ret = esp_https_ota(&ota_config);
#endif

    free(url);

    if (ret == ESP_OK) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(ESP_AT_PORT_TX_WAIT_MS_MAX);
        esp_restart();
        for (;;) {
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

#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
void at_set_mcu_state_if_sleep(at_sleep_mode_t mode)
{
    switch (mode) {
    case AT_DISABLE_SLEEP:
        xEventGroupSetBits(s_wkmcu_evt_group, AT_MCU_AWAKE_ON_AT_SLEEP);
        s_mcu_sleep = false;
        break;

    case AT_MIN_MODEM_SLEEP:
    case AT_LIGHT_SLEEP:
    case AT_MAX_MODEM_SLEEP:
        xEventGroupClearBits(s_wkmcu_evt_group, AT_MCU_AWAKE_BIT);
        s_mcu_sleep = true;
        break;

    default:
        break;
    }

    if (s_wkmcu_cfg.enable) {
        gpio_set_level(s_wkmcu_cfg.wake_number, !s_wkmcu_cfg.wake_signal);
    }

    return;
}

void at_wkmcu_if_config(at_write_data_fn_t write_data_fn)
{
    if (!s_wkmcu_cfg.enable || !s_mcu_sleep) {
        return;
    }

    switch (s_wkmcu_cfg.wake_mode) {
    case WKMCU_MODE_GPIO:
        gpio_set_level(s_wkmcu_cfg.wake_number, s_wkmcu_cfg.wake_signal);
        break;

    case WKMCU_MODE_UART:
        write_data_fn(&s_wkmcu_cfg.wake_signal, 1);
        break;

    default:
        break;
    }

    printf("wait %ums or wake-up signal\r\n", s_wkmcu_cfg.delay_ms);
    EventBits_t uxBits = xEventGroupWaitBits(s_wkmcu_evt_group, s_wkmcu_cfg.check_mcu_awake, pdFALSE, pdFALSE, s_wkmcu_cfg.delay_ms / portTICK_PERIOD_MS);

    if (!(uxBits & s_wkmcu_cfg.check_mcu_awake)) {
        // timeout
        xEventGroupSetBits(s_wkmcu_evt_group, AT_MCU_AWAKE_ON_TIMEO);
        s_mcu_sleep = true;
    }

    // reverse wake up signal
    if (s_wkmcu_cfg.wake_mode == WKMCU_MODE_GPIO) {
        gpio_set_level(s_wkmcu_cfg.wake_number, !s_wkmcu_cfg.wake_signal);
    }

    return;
}

static uint8_t at_setup_cmd_userwkmcucfg(uint8_t para_num)
{
    int32_t cnt = 0, enable = 0, wk_mode = 0, wk_number = 0, wk_signal = 0, delay_ms = 0, check_awake = 0;

    if (s_mcu_sleep == true) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // enable
    if (esp_at_get_para_as_digit(cnt++, &enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (enable < 0 || enable > 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (enable) {
        if (s_wkmcu_cfg.enable == 1) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        // wake mode
        if (esp_at_get_para_as_digit(cnt++, &wk_mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (wk_mode <= WKMCU_MODE_MIN || wk_mode >= WKMCU_MODE_MAX) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // wake number
        if (esp_at_get_para_as_digit(cnt++, &wk_number) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (wk_mode == WKMCU_MODE_GPIO) {
            if (!GPIO_IS_VALID_GPIO(wk_number)) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        } else if (wk_mode == WKMCU_MODE_UART) {
#ifdef CONFIG_IDF_TARGET_ESP8266
            if (wk_number != 0) {
#else
            if (wk_number != 1) {
#endif
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }

        // wake signal
        if (esp_at_get_para_as_digit(cnt++, &wk_signal) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (wk_mode == WKMCU_MODE_GPIO) {
            if (wk_signal < 0 || wk_signal > 1) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        } else if (wk_mode == WKMCU_MODE_UART) {
            if (wk_signal < 0 || wk_signal > 0xFF) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }

        // delay time
        if (esp_at_get_para_as_digit(cnt++, &delay_ms) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (delay_ms < 0 || delay_ms > AT_WKMCU_DELAY_MS_MAX) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // check mcu awake bit
        if (cnt < para_num) {
            if (esp_at_get_para_as_digit(cnt++, &check_awake) != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        } else {
            check_awake = BIT(CHECK_MCU_AWAKE_BY_MCU_SLP);
        }
        if (check_awake < 0 || check_awake >= BIT(CHECK_MCU_AWAKE_BY_GPIO)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    } else {
        // already disabled
        if (s_wkmcu_cfg.enable == 0) {
            return ESP_AT_RESULT_CODE_OK;
        }
    }

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // preset gpio status
    if (enable) {
        if (wk_mode == WKMCU_MODE_GPIO) {
            gpio_config_t io_conf;
            io_conf.pin_bit_mask = (1ULL << wk_number);
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_up_en = false;
            io_conf.pull_down_en = false;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
            gpio_set_level(wk_number, !wk_signal);
        }
    } else {
        if (s_wkmcu_cfg.wake_mode == WKMCU_MODE_GPIO) {
            gpio_config_t io_conf;
            io_conf.pin_bit_mask = (1ULL << s_wkmcu_cfg.wake_number);
            io_conf.mode = GPIO_MODE_DISABLE;
            io_conf.pull_up_en = false;
            io_conf.pull_down_en = false;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
        }
    }

    // set config
    if (enable) {
        s_wkmcu_cfg.wake_mode = wk_mode;
        s_wkmcu_cfg.wake_number = wk_number;
        s_wkmcu_cfg.wake_signal = wk_signal;
        s_wkmcu_cfg.delay_ms = delay_ms;
        s_wkmcu_cfg.check_mcu_awake = check_awake;
        s_wkmcu_cfg.enable = enable;
    } else {
        memset(&s_wkmcu_cfg, 0x0, sizeof(s_wkmcu_cfg));
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_usermcusleep(uint8_t para_num)
{
    int32_t cnt = 0, mcu_sleep = 0;

    if (s_wkmcu_cfg.enable != 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // mcu sleep state
    if (esp_at_get_para_as_digit(cnt++, &mcu_sleep) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (mcu_sleep < 0 || mcu_sleep > 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    at_handle_result_code(ESP_AT_RESULT_CODE_OK, NULL);

    if (s_wkmcu_cfg.check_mcu_awake & AT_MCU_AWAKE_ON_MCU_SLEEP) {
        if (mcu_sleep) {
            xEventGroupClearBits(s_wkmcu_evt_group, AT_MCU_AWAKE_BIT);
        } else {
            xEventGroupSetBits(s_wkmcu_evt_group, AT_MCU_AWAKE_ON_MCU_SLEEP);
        }
        s_mcu_sleep = mcu_sleep;

        if (s_wkmcu_cfg.wake_mode == WKMCU_MODE_GPIO) {
            gpio_set_level(s_wkmcu_cfg.wake_number, !s_wkmcu_cfg.wake_signal);
        }
    }

    return ESP_AT_RESULT_CODE_IGNORE;
}
#endif

static const esp_at_cmd_struct s_at_user_cmd[] = {
    {"+USERRAM", NULL, at_query_cmd_userram, at_setup_cmd_userram, NULL},
    {"+USEROTA", NULL, NULL, at_setup_cmd_userota, NULL},
    {"+USERDOCS", NULL, at_query_cmd_userdocs, NULL, NULL},
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    {"+USERWKMCUCFG", NULL, NULL, at_setup_cmd_userwkmcucfg, NULL},
    {"+USERMCUSLEEP", NULL, NULL, at_setup_cmd_usermcusleep, NULL},
#endif
};

bool esp_at_user_cmd_regist(void)
{
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    s_wkmcu_evt_group = xEventGroupCreate();
#endif
    return esp_at_custom_cmd_array_regist(s_at_user_cmd, sizeof(s_at_user_cmd) / sizeof(s_at_user_cmd[0]));
}

ESP_AT_CMD_SET_FIRST_INIT_FN(esp_at_user_cmd_regist);

#endif
