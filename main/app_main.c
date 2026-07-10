/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_at.h"
#include "esp_at_internal.h"
#include "esp_at_init.h"

#if CONFIG_AT_WIFI_COMMAND_SUPPORT
#include "esp_wifi.h"
#endif

static const char *TAG = "app_main";

/*
 * PCI-oriented defaults applied once before AT accepts external commands.
 * Keeps changes in application layer; no AT core / IDF patches required.
 */
void esp_at_ready_before(void)
{
#if CONFIG_AT_WIFI_COMMAND_SUPPORT
    wifi_config_t cfg = {0};
    if (esp_wifi_get_config(WIFI_IF_STA, &cfg) == ESP_OK) {
        /* Scenario B: reject OPEN / WEP / WPA, allow WPA2 and above (incl. WPA3). */
        cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        esp_err_t err = esp_wifi_set_config(WIFI_IF_STA, &cfg);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "set Wi-Fi auth threshold failed: %s", esp_err_to_name(err));
        }
    }
#endif

#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
    /* Scenario B: scan list hides OPEN(0)/WEP(1)/WPA_PSK(2); mask 0xFFF8. */
    if (at_exe_cmd("AT+CWLAPOPT=,31,-100,65528\r\n", "OK", 3000) != ESP_OK) {
        ESP_LOGW(TAG, "preset CWLAPOPT failed");
    }
    /* Scenario B: default CWJAP authmode threshold = WPA2_PSK (3) when MCU omits it. */
    if (at_exe_cmd("AT+CWJAP=,,,3\r\n", "OK", 3000) != ESP_OK) {
        ESP_LOGW(TAG, "preset CWJAP authmode failed");
    }
#endif

#if CONFIG_AT_BLE_COMMAND_SUPPORT && CONFIG_AT_SELF_COMMAND_SUPPORT
    /*
     * Scenario C: Mode 1 Level 4 — SC+MITM+bond, KeyboardDisplay (no Just Works),
     * 128-bit key, disconnect if peer cannot meet the level (auth_option=1).
     */
    if (at_exe_cmd("AT+BLESECPARAM=13,4,16,3,3,1\r\n", "OK", 3000) != ESP_OK) {
        ESP_LOGW(TAG, "preset BLESECPARAM failed");
    }
#endif
}

void app_main(void)
{
    esp_at_main_preprocess();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_at_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_at_init();
}
