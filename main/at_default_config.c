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

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT)
#include "esp_bt.h"
#endif

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_COMMAND_SUPPORT)
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

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
bool esp_at_get_wifi_default_config(wifi_init_config_t* config)
{
    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();

    if (!config) {
        return false;
    }

    memcpy(config, &wifi_cfg, sizeof(wifi_init_config_t));
    return true;
}
#endif
