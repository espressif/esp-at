/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#include <string.h>

#include "freertos/FreeRTOS.h"

#include "esp_system.h"

#include "esp_at.h"

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
#include "esp_wifi.h"
#endif

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT)
#include "esp_bt.h"
#endif

#include "at_default_config.h"

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
#endif

#if defined(CONFIG_IDF_TARGET_ESP8266)
    {"WROOM-02",        CONFIG_ESP_AT_OTA_TOKEN_WROOM_02,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM_02 },
    {"WROOM-S2",        CONFIG_ESP_AT_OTA_TOKEN_WROOM_S2,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM_S2 },
#endif
};

static uint8_t esp_at_module_id = 0x1;

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT)
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
    return 6*1024;
}

uint32_t esp_at_get_socket_task_stack_size(void)
{
    return 6*1024;
}

#ifdef CONFIG_AT_OTA_SUPPORT
const char* esp_at_get_ota_token_by_id(uint32_t id, esp_at_ota_mode_type ota_mode)
{
    char* ota_token = ESP_AT_UNKNOWN_STR;
    if (sizeof(esp_at_module_info) == 0) {
        return ota_token;
    }

    if (id < sizeof(esp_at_module_info)/sizeof(esp_at_module_info[0])) {
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

    if (id < sizeof(esp_at_module_info)/sizeof(esp_at_module_info[0])) {
        return esp_at_module_info[id].module_name;
    }

    return ESP_AT_UNKNOWN_STR;
}

uint32_t esp_at_factory_parameter_init(void)
{
    const esp_partition_t * partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
    char* data = NULL;
#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    wifi_country_t country;
#endif

    if (!partition) {
        printf("factory_parameter partition missed\r\n");
        return -1;
    }

    data = (char*)malloc(ESP_AT_FACTORY_PARAMETER_SIZE); // Notes
    assert(data != NULL);
    if(esp_partition_read(partition, 0, data, ESP_AT_FACTORY_PARAMETER_SIZE) != ESP_OK){
        free(data);
        return -1;
    }

    if ((data[0] != 0xFC) || (data[1] != 0xFC)) { // check magic flag, should be 0xfc 0xfc
        return -1;
    }

    // data[2] is version, now we donot care it, so skip it
    // get module id
    esp_at_module_id = data[3];
#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    // get max tx power
    if (data[4] != 0xFF) {
        esp_err_t ret = esp_wifi_set_max_tx_power((int8_t)data[4]);
        printf("max tx power=%d,ret=%d\r\n", data[4], ret);
    }

    memset(&country,0x0,sizeof(country));
    // max tx power, begin channel, end channel, country code
    if ((data[6] != 0xFF) && (data[7] != 0xFF) && (data[8] != 0xFF)) {
        if ((data[6] < 1) || (data[7] > 14) || (data[7] < data[6])) {
            printf("factory  error %s - %d\r\n", __func__, __LINE__);
            return -1;
        }

        country.schan = data[6];
        country.nchan = data[7] - data[6] + 1;
        memcpy(country.cc, &data[8], sizeof(country.cc));
        country.policy = WIFI_COUNTRY_POLICY_MANUAL;
        // esp_wifi_get_max_tx_power(&country.max_tx_power);
        esp_wifi_set_country(&country);
    }
#endif

    free(data);
    return 0;
}

uint32_t esp_at_get_module_id(void)
{
    return esp_at_module_id;
}
