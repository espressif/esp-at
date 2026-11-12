/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/socket.h>
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_private/esp_gpio_reserve.h"
#endif

#if defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED) && defined(CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT)
#include "at_compress_ota.h"
#endif

#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_timer.h"
#include "esp_at_core.h"
#include "esp_at.h"
#include "esp_at_internal.h"
#include "at_ota.h"

#ifdef CONFIG_AT_USER_COMMAND_SUPPORT

#define AT_USERRAM_READ_BUFFER_SIZE     1024
#define AT_USEROTA_URL_LEN_MAX          (8 * 1024)
#define AT_USEROTA_AUTH_NONE            0
#define AT_USEROTA_AUTH_VERIFY_SERVER   1
#define AT_USEROTA_AUTH_CLIENT_CERT     2
#define AT_USEROTA_AUTH_MUTUAL          3
#define AT_USEROTA_AUTH_MAX             4
/* One socket wait. Also capped by <total timeout> when that limit is shorter. */
#define AT_USEROTA_HTTP_TIMEOUT_MS_DEFAULT  10000
#define AT_USEROTA_HTTP_TIMEOUT_MS_MIN      1000
#define AT_USEROTA_HTTP_TIMEOUT_MS_MAX      (30 * 1000)
#define AT_USEROTA_TIMEOUT_SEC_MIN          10
#define AT_USEROTA_TIMEOUT_SEC_MAX          (5 * 60)
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
/* Socket of a timed upgrade. The timer task shuts it down; the download task closes the client. */
static atomic_int s_user_ota_fd = -1;
static atomic_bool s_user_ota_timed_out = false;
static bool s_user_ota_arm_fd = false;
/* Held across shutdown and across withdrawing the fd. close() frees the socket
 * only after DISCONNECTED returns, so the timer cannot shut down a reused fd. */
static SemaphoreHandle_t s_user_ota_fd_lock;
static const char *TAG = "at-user";

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
        if ((uint32_t)offset > s_user_ram_size || (uint32_t)length > s_user_ram_size - (uint32_t)offset) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        int32_t received = esp_at_port_read_bytes(sp_user_ram + offset, length,
                                                  ESP_AT_PORT_PROMPT_OK, portMAX_DELAY);
        if (received != length) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        ESP_AT_LOGI(TAG, "recv %d bytes", received);
        esp_at_port_write_data((uint8_t *)"\r\nWRITE OK\r\n", strlen("\r\nWRITE OK\r\n"));
        return ESP_AT_RESULT_CODE_PROCESS_DONE;
    }

    // read
    case AT_USERRAM_READ: {
        if (sp_user_ram == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if ((uint32_t)offset > s_user_ram_size || (uint32_t)length > s_user_ram_size - (uint32_t)offset) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        uint8_t *pbuffer = calloc(1, esp_at_min(AT_USERRAM_READ_BUFFER_SIZE, length) + HEAD_BUFFER_SIZE);
        if (pbuffer == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        ESP_AT_LOGI(TAG, "to read %d bytes", length);

        uint32_t head_len = 0, had_read_len = 0, to_read_len = 0;
        do {
            to_read_len = esp_at_min(length - had_read_len, AT_USERRAM_READ_BUFFER_SIZE);
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

static void at_userota_timeout_cb(void *arg)
{
    int fd;
    bool locked = false;

    (void)arg;
    atomic_store(&s_user_ota_timed_out, true);
    if (s_user_ota_fd_lock != NULL && xSemaphoreTake(s_user_ota_fd_lock, portMAX_DELAY) == pdTRUE) {
        locked = true;
    }
    fd = atomic_exchange(&s_user_ota_fd, -1);
    if (fd >= 0) {
        shutdown(fd, SHUT_RDWR);
    }
    if (locked) {
        xSemaphoreGive(s_user_ota_fd_lock);
    }
    ESP_AT_LOGE(TAG, "ota timeout");
}

static void at_userota_stop_timer(esp_timer_handle_t timer)
{
    if (timer == NULL) {
        return;
    }
    if (esp_timer_stop_blocking(timer, portMAX_DELAY) == ESP_OK) {
        esp_timer_delete(timer);
    }
    s_user_ota_arm_fd = false;
    atomic_store(&s_user_ota_fd, -1);
    atomic_store(&s_user_ota_timed_out, false);
}

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    /* DISCONNECTED runs inside close(), before the fd is freed. Withdraw it under the
     * lock the timer holds across shutdown. Publishing it would let the timer shut
     * down a number the OS may already have given to another socket. */
    if (s_user_ota_arm_fd && evt->client != NULL) {
        if (evt->event_id == HTTP_EVENT_DISCONNECTED) {
            bool locked = false;

            if (s_user_ota_fd_lock != NULL && xSemaphoreTake(s_user_ota_fd_lock, portMAX_DELAY) == pdTRUE) {
                locked = true;
            }
            atomic_store(&s_user_ota_fd, -1);
            if (locked) {
                xSemaphoreGive(s_user_ota_fd_lock);
            }
        } else {
            int fd = esp_http_client_get_socket(evt->client);

            if (fd >= 0) {
                atomic_store(&s_user_ota_fd, fd);
                /* The timer may have fired before this socket existed. Shut it down on this task. */
                if (atomic_load(&s_user_ota_timed_out) && atomic_exchange(&s_user_ota_fd, -1) >= 0) {
                    shutdown(fd, SHUT_RDWR);
                }
            }
        }
    }

    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        esp_at_http_handle_error_event(evt);
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_AT_LOGI(TAG, "http connected");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_AT_LOGI(TAG, "http header sent");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_AT_LOGI(TAG, "http header, key=%s, value=%s", evt->header_key, evt->header_value);

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
            ESP_AT_LOGI(TAG, "receive len=%d, receive total len=%d", evt->data_len, s_user_ota_recv_size);
        } else {
            ESP_AT_LOGI(TAG, "total_len=%d(%d), %0.1f%%!", s_user_ota_total_size, s_user_ota_recv_size, (s_user_ota_recv_size * 1.0) * 100 / s_user_ota_total_size);
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_AT_LOGI(TAG, "http finished");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_AT_LOGW(TAG, "http disconnected");
        break;
    default:
        break;
    }

    return ESP_OK;
}

static bool at_userota_url_is_https(const uint8_t *url)
{
    const char prefix[] = "https://";

    for (size_t i = 0; prefix[i] != '\0'; i++) {
        char c = (char)url[i];
        if (c >= 'A' && c <= 'Z') {
            c = (char)(c - 'A' + 'a');
        }
        if (c != prefix[i]) {
            return false;
        }
    }
    return true;
}

/* Resolve the URL host within timeout_ms. The address is discarded; the URL is not changed. */
static esp_err_t at_userota_probe_dns(const char *url, uint32_t timeout_ms)
{
    const char *host;
    const char *end;
    const char *at;
    char name[254];
    size_t len;
    ip_addr_t addr;

    host = strstr(url, "://");
    if (host == NULL) {
        return ESP_OK;
    }
    host += 3;
    end = strpbrk(host, "/?#");
    at = strchr(host, '@');
    if (at != NULL && (end == NULL || at < end)) {
        host = at + 1;
    }
    if (*host == '[') {
        host++;
        end = strchr(host, ']');
        if (end == NULL) {
            return ESP_FAIL;
        }
    } else {
        const char *colon = strchr(host, ':');

        if (colon != NULL && (end == NULL || colon < end)) {
            end = colon;
        }
        if (end == NULL) {
            end = host + strlen(host);
        }
    }
    len = (size_t)(end - host);
    if (len == 0 || len >= sizeof(name)) {
        return ESP_FAIL;
    }
    memcpy(name, host, len);
    name[len] = '\0';
    esp_err_t err = esp_at_hostname_to_ipaddr(name, ESP_AT_IPPROTO_UNSPEC, &addr, timeout_ms);
    if (err != ESP_OK) {
        ESP_AT_LOGE(TAG, "dns failed, errno:0x%x (%s)", (unsigned)err, esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

static uint8_t at_setup_cmd_userota(uint8_t para_num)
{
#define TEMP_BUFFER_SIZE    32
    uint8_t buffer[TEMP_BUFFER_SIZE] = {0};
    int32_t length = 0;
    int32_t auth_mode = AT_USEROTA_AUTH_NONE;
    int32_t ca_id = 0;
    int32_t timeout_sec = 0;
    int32_t http_timeout_ms = AT_USEROTA_HTTP_TIMEOUT_MS_DEFAULT;
    int32_t cnt = 0;
    char *ca_pem = NULL;

    // length
    if (esp_at_get_para_as_digit(cnt++, &length) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((length <= 0) || (length > AT_USEROTA_URL_LEN_MAX)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        int32_t value = 0;
        esp_at_para_parse_ret_t parsed = esp_at_get_para_as_digit(cnt++, &value);
        if (parsed == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (parsed == ESP_AT_PARA_PARSE_RESULT_OK) {
            if (value < AT_USEROTA_AUTH_NONE || value >= AT_USEROTA_AUTH_MAX) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            /* Client certificate and mutual authentication are reserved. */
            if (value == AT_USEROTA_AUTH_CLIENT_CERT || value == AT_USEROTA_AUTH_MUTUAL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            auth_mode = value;
        }
    }
    if (cnt < para_num) {
        int32_t value = 0;
        esp_at_para_parse_ret_t parsed = esp_at_get_para_as_digit(cnt++, &value);
        if (parsed == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (parsed == ESP_AT_PARA_PARSE_RESULT_OK) {
            if (auth_mode == AT_USEROTA_AUTH_VERIFY_SERVER && value < 0) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            ca_id = value;
        }
    }
    if (cnt < para_num) {
        int32_t value = 0;
        esp_at_para_parse_ret_t parsed = esp_at_get_para_as_digit(cnt++, &value);
        if (parsed == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (parsed == ESP_AT_PARA_PARSE_RESULT_OK) {
            if (value < AT_USEROTA_HTTP_TIMEOUT_MS_MIN || value > AT_USEROTA_HTTP_TIMEOUT_MS_MAX) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            http_timeout_ms = value;
        }
    }
    if (cnt < para_num) {
        int32_t value = 0;
        esp_at_para_parse_ret_t parsed = esp_at_get_para_as_digit(cnt++, &value);
        if (parsed == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (parsed == ESP_AT_PARA_PARSE_RESULT_OK) {
            if (value < AT_USEROTA_TIMEOUT_SEC_MIN || value > AT_USEROTA_TIMEOUT_SEC_MAX) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            timeout_sec = value;
        }
    }

    // parameters are ready
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (auth_mode == AT_USEROTA_AUTH_VERIFY_SERVER) {
        if (esp_at_ota_load_ca(ca_id, &ca_pem) != ESP_OK || esp_at_ota_check_time() != ESP_OK) {
            free(ca_pem);
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    uint8_t *url = (uint8_t *)calloc(1, length + 1);
    if (url == NULL) {
        free(ca_pem);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    int32_t received = esp_at_port_read_bytes(url, length, ESP_AT_PORT_PROMPT_OK, portMAX_DELAY);
    if (received != length) {
        free(url);
        free(ca_pem);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\nRecv %d bytes\r\n", length);
    esp_at_port_write_data(buffer, strlen((char *)buffer));

    ESP_AT_LOGI(TAG, "url: %s", url);
    if (timeout_sec > 0) {
        ESP_AT_LOGI(TAG, "ota timeout %d s", (int)timeout_sec);
    }

    if (auth_mode == AT_USEROTA_AUTH_VERIFY_SERVER && !at_userota_url_is_https(url)) {
        free(url);
        free(ca_pem);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    s_user_ota_total_size = 0;
    s_user_ota_recv_size = 0;
    s_user_ota_is_chunked = true;

    int32_t io_timeout_ms = http_timeout_ms;
    esp_timer_handle_t timer = NULL;

    if (timeout_sec > 0 && (int32_t)timeout_sec * 1000 < io_timeout_ms) {
        io_timeout_ms = (int32_t)timeout_sec * 1000;
    }
    if (timeout_sec > 0) {
        const esp_timer_create_args_t timer_args = {
            .callback = at_userota_timeout_cb,
            .name = "userota",
        };

        atomic_store(&s_user_ota_timed_out, false);
        atomic_store(&s_user_ota_fd, -1);
        s_user_ota_arm_fd = true;
        if (esp_timer_create(&timer_args, &timer) != ESP_OK ||
                esp_timer_start_once(timer, (uint64_t)timeout_sec * 1000000) != ESP_OK) {
            if (timer != NULL) {
                esp_timer_delete(timer);
            }
            s_user_ota_arm_fd = false;
            free(url);
            free(ca_pem);
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if (at_userota_probe_dns((const char *)url, (uint32_t)io_timeout_ms) != ESP_OK ||
            (timeout_sec > 0 && atomic_load(&s_user_ota_timed_out))) {
        at_userota_stop_timer(timer);
        free(url);
        free(ca_pem);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    esp_http_client_config_t config = {
        .url = (const char *)url,
        .event_handler = _http_event_handler,
        .keep_alive_enable = true,
        .timeout_ms = io_timeout_ms,
        .buffer_size = 2048,
        .cert_pem = ca_pem,
    };

#if defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED) && defined(CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT)
    esp_err_t ret = at_compress_https_ota(&config);
#else
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };
    esp_err_t ret = esp_https_ota(&ota_config);
#endif

    at_userota_stop_timer(timer);
    free(url);
    free(ca_pem);

    if (ret == ESP_OK) {
        esp_at_write_result(ESP_AT_RESULT_CODE_OK);
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
void at_set_mcu_state_if_sleep(esp_at_sleep_mode_t mode)
{
    if (!(s_wkmcu_cfg.check_mcu_awake & AT_MCU_AWAKE_ON_AT_SLEEP)) {
        return;
    }

    switch (mode) {
    case ESP_AT_SLEEP_DISABLE:
        xEventGroupSetBits(s_wkmcu_evt_group, AT_MCU_AWAKE_ON_AT_SLEEP);
        s_mcu_sleep = false;
        break;

    case ESP_AT_SLEEP_MIN_MODEM:
    case ESP_AT_SLEEP_LIGHT:
    case ESP_AT_SLEEP_MAX_MODEM:
        xEventGroupClearBits(s_wkmcu_evt_group, AT_MCU_AWAKE_BIT);
        s_mcu_sleep = true;
        break;

    default:
        break;
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
        gpio_hold_dis(s_wkmcu_cfg.wake_number);
        gpio_set_level(s_wkmcu_cfg.wake_number, s_wkmcu_cfg.wake_signal);
        gpio_hold_en(s_wkmcu_cfg.wake_number);
        break;

    case WKMCU_MODE_UART:
        write_data_fn(&s_wkmcu_cfg.wake_signal, 1);
        break;

    default:
        break;
    }

    ESP_AT_LOGI(TAG, "wait %ums or wake-up signal", s_wkmcu_cfg.delay_ms);
    EventBits_t uxBits = xEventGroupWaitBits(s_wkmcu_evt_group, s_wkmcu_cfg.check_mcu_awake, pdFALSE, pdFALSE, s_wkmcu_cfg.delay_ms / portTICK_PERIOD_MS);

    if (!(uxBits & s_wkmcu_cfg.check_mcu_awake)) {
        // timeout
        if (s_wkmcu_cfg.check_mcu_awake & AT_MCU_AWAKE_ON_TIMEO) {
            xEventGroupSetBits(s_wkmcu_evt_group, AT_MCU_AWAKE_ON_TIMEO);
            s_mcu_sleep = false;
        } else {
            xEventGroupClearBits(s_wkmcu_evt_group, AT_MCU_AWAKE_BIT);
            s_mcu_sleep = true;
        }
    }

    // reverse wake up signal
    if (s_wkmcu_cfg.wake_mode == WKMCU_MODE_GPIO) {
        gpio_hold_dis(s_wkmcu_cfg.wake_number);
        gpio_set_level(s_wkmcu_cfg.wake_number, !s_wkmcu_cfg.wake_signal);
        gpio_hold_en(s_wkmcu_cfg.wake_number);
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
            // Reject a GPIO already in use, for example by SPI flash, UART, or another peripheral.
            if (esp_gpio_is_reserved(1ULL << wk_number)) {
                ESP_AT_LOGE(TAG, "GPIO%d is in use", (int)wk_number);
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
            gpio_hold_dis(wk_number);
            gpio_set_level(wk_number, !wk_signal);
            gpio_hold_en(wk_number);
        }
    } else {
        if (s_wkmcu_cfg.wake_mode == WKMCU_MODE_GPIO) {
            gpio_config_t io_conf;
            io_conf.pin_bit_mask = (1ULL << s_wkmcu_cfg.wake_number);
            io_conf.mode = GPIO_MODE_DISABLE;
            io_conf.pull_up_en = false;
            io_conf.pull_down_en = false;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_hold_dis(s_wkmcu_cfg.wake_number);
            gpio_config(&io_conf);
            // gpio_config(OUTPUT) reserved this pin. DISABLE does not clear that bit.
            esp_gpio_revoke(1ULL << s_wkmcu_cfg.wake_number);
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

    esp_at_dispatch_result(ESP_AT_RESULT_CODE_OK, NULL);

    if (s_wkmcu_cfg.check_mcu_awake & AT_MCU_AWAKE_ON_MCU_SLEEP) {
        if (mcu_sleep) {
            xEventGroupClearBits(s_wkmcu_evt_group, AT_MCU_AWAKE_BIT);
        } else {
            xEventGroupSetBits(s_wkmcu_evt_group, AT_MCU_AWAKE_ON_MCU_SLEEP);
        }
        s_mcu_sleep = mcu_sleep;

        if (s_wkmcu_cfg.wake_mode == WKMCU_MODE_GPIO) {
            gpio_hold_dis(s_wkmcu_cfg.wake_number);
            gpio_set_level(s_wkmcu_cfg.wake_number, !s_wkmcu_cfg.wake_signal);
            gpio_hold_en(s_wkmcu_cfg.wake_number);
        }
    }

    return ESP_AT_RESULT_CODE_IGNORE;
}
#endif

static const esp_at_cmd_t s_at_user_cmd[] = {
    {"+USERRAM", NULL, at_query_cmd_userram, at_setup_cmd_userram, NULL},
    {"+USEROTA", NULL, NULL, at_setup_cmd_userota, NULL},
    {"+USERDOCS", NULL, at_query_cmd_userdocs, NULL, NULL},
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    {"+USERWKMCUCFG", NULL, NULL, at_setup_cmd_userwkmcucfg, NULL},
    {"+USERMCUSLEEP", NULL, NULL, at_setup_cmd_usermcusleep, NULL},
#endif
};

bool esp_at_user_cmd_register(void)
{
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    s_wkmcu_evt_group = xEventGroupCreate();
#endif
    if (s_user_ota_fd_lock == NULL) {
        s_user_ota_fd_lock = xSemaphoreCreateMutex();
    }
    return esp_at_custom_cmd_array_register(s_at_user_cmd, sizeof(s_at_user_cmd) / sizeof(s_at_user_cmd[0]));
}

ESP_AT_CMD_SET_FIRST_INIT_FN(esp_at_user_cmd_register, 24);

#endif
