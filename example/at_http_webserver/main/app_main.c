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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_partition.h"
#include "esp_wifi.h"

#include "espfs.h"
#include "httpd.h"
#include "httpdespfs.h"
#include "cgiat.h"
#include "button.h"
#include "atparse.h"

#define RESET_BUTTON_NUM 15

static const char* TAG = "HTTPAT";

static void httpat_button_reset_tap_cb(void* arg)
{
    const esp_partition_t* p = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);

    if (p == NULL) {
        ESP_LOGE(TAG, "Cannot find patition");
        return;
    }

    esp_wifi_stop();
    esp_partition_erase_range(p, 0, p->size);

    esp_restart();
}

static void initialise_key(void)
{
    button_handle_t btn_handle = button_dev_init(RESET_BUTTON_NUM, 0, BUTTON_ACTIVE_LOW);
    button_dev_add_tap_cb(BUTTON_PUSH_CB, httpat_button_reset_tap_cb, "PUSH", 50 / portTICK_PERIOD_MS, btn_handle);
}

HttpdBuiltInUrl builtInUrls[] = {
    {"/", cgiRedirect, "/login.html"},
    {"/routeinfo", cgiRouteInfo, NULL},
    {"/showapinfo", cgiShowApInfo, NULL},
    {"/configapinfo", cgiConfigApInfo, NULL},
    {"/connap", cgiConnAp, NULL},
    {"/getramsize", getRamSize, NULL},
    {"/ping", cgiping, NULL},
    {"/rst", cgirst, NULL},
    {"/restore", cgirestore, NULL},
    {"/loginpwd", cgiloginpwd, NULL},
    {"/changeloginpwd", cgiChangeLoginPwd, NULL},
    {"/closeap", cgiCloseAp, NULL},
    {"/setap", cgiSetAp, NULL},
    {"/scanap", cgiScanAp, NULL},
    {"/setdhcp", cgiSetDhcp, NULL},
    {"/showdhcpinfo", cgiShowDhcp, NULL},
    {"/setstaipinfo", cgiSetStaIpInfo, NULL},
    {"/setapipinfo", cgiSetApIpInfo, NULL},
    {"/setstamac", cgiSetStaMac, NULL},
    {"/setapmac", cgiSetApMac, NULL},
    {"/connip", cgiConnIp, NULL},
    {"/closeip", cgiCloseIp, NULL},
    {"/getipstatus", cgiGetIpStatus, NULL},
    {"/softapdevice", getConnDev, NULL},

    {"/bleset", cgiBleSet, NULL},
    {"/blescan", cgiBleScan, NULL},
    {"/getbleset", cgiGetBleSet, NULL},
    {"/bleconnect", cgiBleConn, NULL},
    {"/getbleconnect", cgiGetBleConn, NULL},
    {"/blecloseconnect", cgiBleCloseConn, NULL},
    {"/bleadvset", cgiBleAdvSet, NULL},
    {"/getbleadvset", cgiGetBleAdvSet, NULL},
    {"/bleadvstart", cgiBleAdvStart, NULL},
    {"/bleadvstop", cgiBleAdvStop, NULL},
    {"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
    {NULL, NULL, NULL}
};

void app_main()
{
    int ret;
    nvs_flash_init();

    initialise_key();

    ESP_ERROR_CHECK(esp_at_init_parse());

    ret =  espFsInit();
    assert(ret == 1);
    httpdInit(builtInUrls, 80);
}

