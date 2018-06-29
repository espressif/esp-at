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
#include "freertos/task.h"

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_parse_at.h"

#ifdef TEST_DEMO

static const char* TAG = "TestParseAT";

void wifiscanresult_cb(scan_ap_link* dev_head)
{
    printf("Receive a wifi scan result\n");
    scan_ap_link* scan_info = dev_head;
    scan_ap_link* scan_tmp;

    while(scan_info){
	scan_tmp = scan_info;
	ESP_LOGD(TAG, "scan info: ecn:%d, ssid:%s, rssi:%d, mac:%s, channel:%d\n",scan_info->ecn,scan_info->ssid,scan_info->rssi,scan_info->mac,scan_info->channel);

	scan_info = scan_info->next;
	free(scan_tmp);
    }
    printf("Scan OK\n");
}

void blescanresult_cb(ble_scan_info* dev_head)
{
    ble_scan_info* scan_info = dev_head;
    ble_scan_info* scan_tmp;

    while(scan_info){
	scan_tmp = scan_info;
	ESP_LOGI(TAG, "scan info:addr: %s, rssi:%d, adv_data:%s, scan_rsp_data:%s\n",scan_info->addr,scan_info->rssi,scan_info->adv_data,scan_info->scan_rsp_data);
	scan_info = scan_info->next;
	if(scan_tmp != dev_head){
	    free(scan_tmp);
	}
	if(scan_info == dev_head){
	    ESP_LOGI(TAG, "This is the deadline");
	    break;
	}
    }
    free(dev_head);
    ESP_LOGI(TAG, "BLE scan OK");
}

void app_main()
{
    esp_err_t ret;
    esp_at_arg at_arg;

    nvs_flash_init();
/****** Init parse frame, it must call before using other API ******/
    ESP_ERROR_CHECK(esp_at_init_parse());

/****** Read free ramsize ******/
    int32_t ram_size = esp_at_getramsize();
    ESP_LOGI(TAG, "ramsiz:%d",ram_size);


#ifdef WIFI_TEST
/****** Scan WiFi ******/
    ret = esp_at_wifi_scan(wifiscanresult_cb);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "command execution error");
    } 
    ESP_LOGI(TAG, "Scan WiFi success"); 

    vTaskDelay(1000 / portTICK_RATE_MS);
    ram_size = esp_at_getramsize();
    ESP_LOGI(TAG, "After scan WiFi ramsiz:%d",ram_size);

/****** Connect WiFi ******/
    memset(&at_arg, '\0', sizeof(at_arg));
    strcpy(at_arg.cwjap.ssid, "HUAWEI001");
    strcpy(at_arg.cwjap.pwd, "12345678");
    ret = esp_at_join_ap(SET, &at_arg);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "Error pwd or ssid");
    }

/****** Get connected WiFi info ******/
    memset(&at_arg, '\0', sizeof(at_arg));
    ret = esp_at_join_ap(QUERY, &at_arg);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "Error");
    }
    ESP_LOGI(TAG, "SSID:%s, bssid:%s",at_arg.queryjap.ssid, at_arg.queryjap.bssid);

/****** Get IP info ******/
    esp_at_ip_info ip_info;
    ret = esp_at_get_ip(SOFTAP,&ip_info);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "Error parameter\n");
    }
    ESP_LOGI(TAG, "SOFTAP IP:%s, mac:%s",ip_info.ip, ip_info.mac);

    ret = esp_at_get_ip(STATION,&ip_info);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "Error parameter\n");
    }
    ESP_LOGI(TAG, "STATION IP:%s, mac:%s",ip_info.ip, ip_info.mac);
#endif

#ifdef BLE_TEST
/****** Show current BLE role ******/
    ret = esp_at_ble_init(QUERY,&at_arg);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "Error parameter\n");
    }
    ESP_LOGI(TAG, "Current role:%d",at_arg.bleinit.init_mode);

/****** Set BLE role ******/
    at_arg.bleinit.init_mode = 1;
    ret = esp_at_ble_init(SET,&at_arg);
    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "Error parameter\n");
    }
    ESP_LOGI(TAG, "Set BLE role success");

/****** BLE scan ******/
    at_arg.blescan.enable = 1;
    at_arg.blescan.interval = 1;
    ret = esp_at_ble_scan(&at_arg,blescanresult_cb);

    if(ret == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG, "WiFi scan error");
    }


#endif


}

#endif

