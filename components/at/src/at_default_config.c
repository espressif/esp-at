/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_at.h"

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT) \
    || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT) || defined(CONFIG_AT_BLUFI_COMMAND_SUPPORT)
#include "esp_bt.h"
#endif

#include "at_ota.h"

#define ESP_AT_UNKNOWN_STR      "Unknown"

typedef struct {
    char* module_name;
    char* ota_token;
    char* ota_ssl_token;
} esp_at_module_info_t;

static const esp_at_module_info_t esp_at_module_info[] = {
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
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C6)
    {"ESP32C6-4MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C6_4MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C6_4MB},
#endif
};

static uint8_t esp_at_module_id = 0x0;

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT) \
    || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT) || defined(CONFIG_AT_BLUFI_COMMAND_SUPPORT)
bool esp_at_get_bluetooth_controller_default_config(esp_bt_controller_config_t* config)
{
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if (!config) {
        return false;
    }

    memcpy(config, &bt_cfg, sizeof(esp_bt_controller_config_t));
    return true;
}
#endif

uint32_t esp_at_get_process_task_stack_size(void)
{
    return CONFIG_AT_PROCESS_TASK_STACK_SIZE;
}

uint32_t esp_at_get_socket_task_stack_size(void)
{
    return CONFIG_AT_SOCKET_TASK_STACK_SIZE;
}

#ifdef CONFIG_AT_OTA_SUPPORT
const char* esp_at_get_ota_token_by_id(uint32_t id, at_ota_mode_t ota_mode)
{
    char* ota_token = ESP_AT_UNKNOWN_STR;
    if (sizeof(esp_at_module_info) == 0) {
        return ota_token;
    }

    if (id < sizeof(esp_at_module_info) / sizeof(esp_at_module_info[0])) {
        if (ota_mode == ESP_AT_OTA_MODE_NORMAL) {
            ota_token = esp_at_module_info[id].ota_token;
        } else if (ota_mode == ESP_AT_OTA_MODE_SSL) {
            ota_token = esp_at_module_info[id].ota_ssl_token;
        }
    }

    return ota_token;
}
#endif

const char* esp_at_get_module_name_by_id(uint32_t id)
{
    if (sizeof(esp_at_module_info) == 0) {
        return ESP_AT_UNKNOWN_STR;
    }

    if (id < sizeof(esp_at_module_info) / sizeof(esp_at_module_info[0])) {
        return esp_at_module_info[id].module_name;
    }

    return ESP_AT_UNKNOWN_STR;
}

const char* esp_at_get_current_module_name(void)
{
    if (sizeof(esp_at_module_info) == 0) {
        return ESP_AT_UNKNOWN_STR;
    }

    if (esp_at_module_id < sizeof(esp_at_module_info) / sizeof(esp_at_module_info[0])) {
        return esp_at_module_info[esp_at_module_id].module_name;
    }

    return ESP_AT_UNKNOWN_STR;
}

uint32_t esp_at_get_module_id(void)
{
    return esp_at_module_id;
}

void esp_at_set_module_id(uint32_t id)
{
    esp_at_module_id = id;
}

void esp_at_set_module_id_by_str(const char *buffer)
{
    for (int i = 0; i < sizeof(esp_at_module_info) / sizeof(esp_at_module_info[0]); ++i) {
        if (strcmp(buffer, esp_at_module_info[i].module_name) == 0) {
            esp_at_module_id = i;
            break;
        }
    }
}
