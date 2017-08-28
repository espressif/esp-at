/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

// System/RTOS
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// ESP32
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_log.h"

// Application
#include "version.h"
#include "device_nv.h"
#include "bt_provisioning.h"
#include "mem.h"
#include "system_util.h"

// Component
#include "at_task.h"

static const char *TAG = "main.c";

void app_main()
{
    struct timeval tv;
    EventGroupHandle_t wifi_event_group = NULL;

    wifi_event_group = at_task_init();

    // Initialize NV flash for data storage
    if( ESP_OK != nvs_init() )
    {
        ESP_LOGE( TAG, "NVS Flash Initialization Failed!" );
        esp_restart();
    }

    if( check_credentials() )
    {
        checkmem("before bt");
        xTaskCreate(&bt_provisioning, "bt_provisioning", 8192, NULL, 5, NULL);
        checkmem("after bt");
        #warning place system event block here with timeout
        delay_minutes(60);
        esp_restart();
        return;     
        ESP_LOGE(TAG, "Should't get here, check time spent in provisioning..");
    }
    else
    {
        ESP_LOGI(TAG, "We have stored credentials, continuing..");
    }

    // Make sure we connected..
    #warning "Define this somewhere better, so everyone can see it.."
    #define WIFI_CONNECTED_BIT BIT0
    if( NULL != wifi_event_group )
    {
        // Wait for the callback to set the WIFI_CONNECTED_BIT in the event group. 
        // - Time and clear credentials after 15 minutes.                              15 minutes = 900000(ms)
        EventBits_t uxBits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, 900000); //portMAX_DELAY);

        if( ( uxBits & WIFI_CONNECTED_BIT ) != 0 )
        {
            gettimeofday(&tv, NULL);
            ESP_LOGI(TAG, "Connected to WiFi @ %ld.%03ld, nothing to do...", tv.tv_sec, tv.tv_usec/1000);
        }
        else
        {
            // We timed out.. reset wifi creds and reboot.
            gettimeofday(&tv, NULL);
            ESP_LOGE(TAG, "WiFi connection timed out @ %ld.%03ld. Clear credentials and restart...", tv.tv_sec, tv.tv_usec/1000);
            clear_credentials();
            esp_restart();
            return;
        }
    }

    ESP_LOGE( TAG, "SS3-ESP32 Application Version: %d.%d.%d.%d\n", MAJOR, MINOR, PATCH, BUILD );
}