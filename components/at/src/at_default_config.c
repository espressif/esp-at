/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_at.h"
#ifdef CONFIG_AT_SMARTCONFIG_COMMAND_SUPPORT
#include "esp_smartconfig.h"
#endif
#ifdef CONFIG_BT_ENABLED
#include "esp_bt.h"
#endif
#include "at_ota.h"
#ifdef CONFIG_AT_FS_COMMAND_SUPPORT
#include "esp_vfs_fat.h"
#endif

// unknown module name if not defined
#define ESP_AT_UNKNOWN_STR      "Unknown"

typedef struct {
    char *module_name;
    char *ota_token;
    char *ota_ssl_token;
} at_module_info_t;

// static variables
static uint8_t s_module_id = 0x0;
static const at_module_info_t s_module_info[] = {
#if defined(CONFIG_IDF_TARGET_ESP32)
    {"WROOM-32",        CONFIG_ESP_AT_OTA_TOKEN_WROOM32,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM32 },        // default:ESP32-WROOM-32
    {"WROOM-32",        CONFIG_ESP_AT_OTA_TOKEN_WROOM32,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM32 },        // ESP32-WROOM-32
    {"WROVER-32",       CONFIG_ESP_AT_OTA_TOKEN_WROVER32,      CONFIG_ESP_AT_OTA_SSL_TOKEN_WROVER32 },       // ESP32-WROVER
    {"PICO-D4",         CONFIG_ESP_AT_OTA_TOKEN_ESP32_PICO_D4, CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_PICO_D4},   // ESP32-PICO-D4
    {"SOLO-1",          CONFIG_ESP_AT_OTA_TOKEN_ESP32_SOLO_1,  CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_SOLO_1 },   // ESP32-SOLO-1
    {"MINI-1",          CONFIG_ESP_AT_OTA_TOKEN_ESP32_MINI_1,  CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_MINI_1 },   // ESP32-MINI-1
    {"ESP32-SDIO",      NULL,  NULL },   // ESP32-SDIO
    {"ESP32-D2WD",      NULL,  NULL },   // ESP32-D2WD
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C3)
    {"MINI-1",         CONFIG_ESP_AT_OTA_TOKEN_ESP32C3_MINI,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C3_MINI },
    {"ESP32C3-SPI",    NULL,  NULL },   // ESP32C3-SPI
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C2)
    {"ESP32C2-2MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C2_2MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C2_2MB},
    {"ESP32C2-4MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C2_4MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C2_4MB},
    {"ESP32C2-2MB-BLE", NULL, NULL},
    {"ESP32C2-2MB-G2", CONFIG_ESP_AT_OTA_TOKEN_ESP32C2_2MB_G2,      CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C2_2MB_G2},
    {"ESP32C2-4MB-G2", CONFIG_ESP_AT_OTA_TOKEN_ESP32C2_4MB_G2,      CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C2_4MB_G2},
    {"ESP32C2-2MB-NO-OTA-G2", NULL, NULL},
    {"ESP32C2-2MB-BLE-G2", NULL, NULL},
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C5)
    {"ESP32C5-4MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C5_4MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C5_4MB},
    {"ESP32C5-SDIO",   NULL,  NULL},
    {"ESP32C5-SPI",    NULL,  NULL },   // ESP32C5-SPI
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C6)
    {"ESP32C6-4MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C6_4MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C6_4MB},
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C61)
    {"ESP32C61-4MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C61_4MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C61_4MB},
#endif

#if defined(CONFIG_IDF_TARGET_ESP32S2)
    {"WROOM",          CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_WROOM,       CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_WROOM},
    {"WROVER",         CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_WROVER,      CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_WROVER},
    {"SOLO",           CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_SOLO,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_SOLO},
    {"MINI",           CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_MINI,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_MINI},
#endif
};
static const char *TAG = "at-default";

// the initialization of module information
const char *esp_at_get_current_module_name(void)
{
    if (sizeof(s_module_info) == 0) {
        return ESP_AT_UNKNOWN_STR;
    }

    if (s_module_id < sizeof(s_module_info) / sizeof(s_module_info[0])) {
        return s_module_info[s_module_id].module_name;
    }

    return ESP_AT_UNKNOWN_STR;
}

uint32_t esp_at_get_module_id(void)
{
    return s_module_id;
}

void esp_at_set_module_id(uint32_t id)
{
    s_module_id = id;
}

void esp_at_set_module_id_by_str(const char *buffer)
{
    for (int i = 0; i < sizeof(s_module_info) / sizeof(s_module_info[0]); ++i) {
        if (strcmp(buffer, s_module_info[i].module_name) == 0) {
            s_module_id = i;
            break;
        }
    }
}

/*-------------------------------------------------------------------------------------
 *      The dependencies required for the AT command set and AT framework
 *
 * The following dependencies are necessary for the specific AT component but are not
 * sufficient on their own to provide the complete functionality of this AT component.
 * Exposing these dependency APIs is intended to offer greater flexibility for
 * developers, enabling re-wrapping of the APIs according to specific needs.
 *-----------------------------------------------------------------------------------*/

/**********************************************************************
 *     The necessary dependencies required for the AT framework
 *********************************************************************/
__attribute__((weak)) esp_err_t esp_at_nvs_set_str(nvs_handle_t handle, const char *key, const char *value)
{
    return nvs_set_str(handle, key, value);
}

__attribute__((weak)) esp_err_t esp_at_nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length)
{
    return nvs_get_str(handle, key, out_value, length);
}

__attribute__((weak)) esp_err_t esp_at_nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length)
{
    return nvs_set_blob(handle, key, value, length);
}

__attribute__((weak)) esp_err_t esp_at_nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length)
{
    return nvs_get_blob(handle, key, out_value, length);
}

__attribute__((weak)) void esp_at_log_write(esp_log_level_t level, const char *tag, const char *format, ...)
{
    if (CONFIG_AT_LOG_DEFAULT_LEVEL >= level) {
        char level_str[ESP_LOG_MAX] = {'N', 'E', 'W', 'I', 'D', 'V'};
        va_list list;
        va_start(list, format);
        size_t new_format_length = strlen(tag) + strlen(format) + 30;
        char new_format[new_format_length];
        snprintf(new_format, new_format_length, "%c (%"PRIu32") %s: %s\n", level_str[level], esp_log_timestamp(), tag, format);
        esp_log_writev(level, tag, new_format, list);
        va_end(list);
    }
}

uint32_t esp_at_get_process_task_stack_size(void)
{
    return CONFIG_AT_PROCESS_TASK_STACK_SIZE;
}

/**************************************************************************
 * The necessary dependencies required for the AT SmartConfig command set
 *************************************************************************/
#ifdef CONFIG_AT_SMARTCONFIG_COMMAND_SUPPORT
esp_err_t esp_at_get_smartconfig_start_config(smartconfig_start_config_t *config)
{
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    memcpy(config, &cfg, sizeof(smartconfig_start_config_t));
    return ESP_OK;
}
#endif

/**************************************************************************
 *   The necessary dependencies required for the AT TCP/IP command set
 *************************************************************************/
#ifdef CONFIG_AT_NET_COMMAND_SUPPORT
uint32_t esp_at_get_socket_task_stack_size(void)
{
    return CONFIG_AT_SOCKET_TASK_STACK_SIZE;
}

uint32_t esp_at_get_netconn_max_count(void)
{
    return CONFIG_AT_SOCKET_MAX_CONN_NUM;
}
#endif

/**************************************************************************
 *    The necessary dependencies required for the AT HTTP command set
 *************************************************************************/
#ifdef CONFIG_AT_HTTP_COMMAND_SUPPORT
uint32_t esp_at_get_http_tx_buffer_size(void)
{
    return CONFIG_AT_HTTP_TX_BUFFER_SIZE;
}

uint32_t esp_at_get_http_rx_buffer_size(void)
{
    return CONFIG_AT_HTTP_RX_BUFFER_SIZE;
}
#endif

/**************************************************************************
 *    The necessary dependencies required for the AT OTA command set
 *************************************************************************/
#ifdef CONFIG_AT_OTA_SUPPORT
const char *esp_at_get_ota_token_by_id(uint32_t id, at_ota_mode_t ota_mode)
{
    char *ota_token = ESP_AT_UNKNOWN_STR;
    if (sizeof(s_module_info) == 0) {
        return ota_token;
    }

    if (id < sizeof(s_module_info) / sizeof(s_module_info[0])) {
        if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
            ota_token = s_module_info[id].ota_token;
        } else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
            ota_token = s_module_info[id].ota_ssl_token;
        }
    }

    return ota_token;
}
#endif

/**************************************************************************
 *    The necessary dependencies required for the AT BLE command set
 *************************************************************************/
#ifdef CONFIG_AT_BLE_COMMAND_SUPPORT
#ifdef CONFIG_BT_NIMBLE_ENABLED
uint32_t esp_at_get_nimble_mtu_size(void)
{
    return CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU;
}
#endif
#endif

#ifdef CONFIG_BT_CONTROLLER_ENABLED
bool esp_at_get_bluetooth_controller_default_config(esp_bt_controller_config_t *config)
{
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if (!config) {
        return false;
    }

    memcpy(config, &bt_cfg, sizeof(esp_bt_controller_config_t));
    return true;
}
#endif
