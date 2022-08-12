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

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT) \
    || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT) || defined(CONFIG_AT_BLUFI_COMMAND_SUPPORT)
#include "esp_bt.h"
#endif

#include "at_ota.h"

#ifdef CONFIG_AT_BT_A2DP_COMMAND_SUPPORT
#include "at_i2s.h"
#endif

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
    {"ESP32_QCLOUD",    CONFIG_ESP_AT_OTA_TOKEN_ESP32_QCLOUD,   CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_QCLOUD },  // ESP32-QCLOUD
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C3)
    {"MINI-1",         CONFIG_ESP_AT_OTA_TOKEN_ESP32C3_MINI,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C3_MINI },
    {"ESP32C3-SPI",    NULL,  NULL },   // ESP32C3-SPI
    {"ESP32C3_QCLOUD", CONFIG_ESP_AT_OTA_TOKEN_ESP32C3_MINI_QCLOUD, CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C3_MINI_QCLOUD },
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C2)
    {"ESP32C2-2MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C2_2MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C2_2MB},
    {"ESP32C2-4MB",    CONFIG_ESP_AT_OTA_TOKEN_ESP32C2_4MB,         CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C2_4MB},
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

const char* esp_at_get_current_module_name(void)
{
    if (sizeof(esp_at_module_info) == 0) {
        return ESP_AT_UNKNOWN_STR;
    }

    if (esp_at_module_id < sizeof(esp_at_module_info)/sizeof(esp_at_module_info[0])) {
        return esp_at_module_info[esp_at_module_id].module_name;
    }

    return ESP_AT_UNKNOWN_STR;
}

static uint32_t esp_at_factory_parameter_init(void)
{
    const esp_partition_t * partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
    char* data = NULL;
    uint8_t version = 0;
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

    version = data[2];
    if (version <= 2) {
        // get module id
        esp_at_module_id = data[3];
    } else {
        const char* module_name = data + 56; // for more detail, please refer to 
        for (uint32_t loop = 0; loop < sizeof(esp_at_module_info)/sizeof(esp_at_module_info[0]); loop++) {
            if (strcmp(module_name, esp_at_module_info[loop].module_name) == 0) {
                esp_at_module_id = loop;
                break;
            }
        }
    }

    printf("module_name:%s\r\n", esp_at_get_current_module_name());
#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    // get max tx power
    if (data[4] != 0xFF) {
        if ((version != 1) || ((version == 1) && (data[4] >= 10))) {
            esp_err_t ret = esp_wifi_set_max_tx_power((int8_t)data[4]);
            printf("max tx power=%d,ret=%d\r\n", data[4], ret);
        }
    }

    memset(&country,0x0,sizeof(country));
    // max tx power, begin channel, end channel, country code
    if ((data[6] != 0xFF) && (data[7] != 0xFF) && (data[8] != 0xFF)) {
        if ((data[6] < 1) || (data[7] > 14) || (data[7] < data[6])) {
            printf("factory  error %s - %d\r\n", __func__, __LINE__);
        } else {
            country.schan = data[6];
            country.nchan = data[7] - data[6] + 1;
            memcpy(country.cc, &data[8], sizeof(country.cc));
            country.policy = WIFI_COUNTRY_POLICY_MANUAL;
            esp_wifi_set_country(&country);
        }
    }
#endif

    free(data);
    return 0;
}

void esp_at_peripheral_init(void)
{
#ifdef CONFIG_AT_BT_A2DP_COMMAND_SUPPORT
    esp_at_i2s_init();
#endif
}

void esp_at_board_init(void)
{
    esp_at_factory_parameter_init();
    esp_at_peripheral_init();
}

uint32_t esp_at_get_module_id(void)
{
    return esp_at_module_id;
}

#ifdef CONFIG_IDF_TARGET_ESP32
#include "hal/wdt_hal.h"
static void at_disable_rtc_wdt(void)
{
    /*
     * At this point, the flashboot protection of RWDT will have been
     * automatically enabled. We can disable flashboot protection as it's not
     * needed anymore.
     */
    // Disable RWDT flashboot protection.
    wdt_hal_context_t rtc_wdt_ctx = {.inst = WDT_RWDT, .rwdt_dev = &RTCCNTL};
    wdt_hal_write_protect_disable(&rtc_wdt_ctx);
    wdt_hal_set_flashboot_en(&rtc_wdt_ctx, false);
    wdt_hal_write_protect_enable(&rtc_wdt_ctx);
}
#endif

#if defined(CONFIG_ESP32_RTC_CLK_SRC_EXT_CRYS) || defined(CONFIG_RTC_CLK_SRC_EXT_CRYS)
#include "soc/rtc.h"

static void at_disable_rtc_clk_32k_if_no_ext_crys(void)
{
    if (rtc_clk_slow_freq_get() != RTC_SLOW_FREQ_32K_XTAL) {
        rtc_clk_32k_enable(false);
        printf("no external 32k oscillator, disable it now.\r\n");
    } else {
        printf("external 32k oscillator already started.\r\n");
    }
}
#endif

void esp_at_main_preprocess(void)
{
#ifdef CONFIG_IDF_TARGET_ESP32
    /**
     * A workaround for ota compatibility issue, description as the following:
     *
     * If esp32 upgraded from v1.1.3.0 to v2.2.0.0+, after the upgraded done,
     * then each call the esp_restart() will trigger the restart twice.
     * The first is normal SW_CPU_RESET, the second is extra RTCWDT_RTC_RESET.
     * (it could happen if upgraded from an old firmware to the new firmware)
     *
     * Here, disable RWDT to avoid the second restart (RTCWDT_RTC_RESET).
     */
    at_disable_rtc_wdt();
#endif

#if defined(CONFIG_ESP32_RTC_CLK_SRC_EXT_CRYS) || defined(CONFIG_RTC_CLK_SRC_EXT_CRYS)
    /**
     * A workaround for external 32 KHz oscillator.
     *
     * esp-at enabled BLE light sleep by default, which requires an external 32 KHz oscillator,
     * so that XTAL_32K_P(32K_XP) and XTAL_32K_N(32K_XN) can not used for common GPIOs
     * even if there is no a physically external 32 KHz oscillator.
     * More details are in RTC clock part of <esp_hardware_design_guidelines_en.pdf>.
     *
     * In order to make XTAL_32K_P(32K_XP) and XTAL_32K_N(32K_XN) can be used for common GPIOs,
     * esp-at will disable external rtc clock if there is no a physically external 32 KHz oscillator.
     */
    at_disable_rtc_clk_32k_if_no_ext_crys();
#endif
}
