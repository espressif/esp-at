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
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
#include "esp_event.h"
#include "esp_wifi.h"
#endif

#if defined(CONFIG_BT_ENABLED)
#include "esp_bt.h"
#endif

#ifdef CONFIG_AT_QCLOUD_IOT_COMMAND_SUPPORT
#include "qcloud_iot_at.h"
#include "qcloud_iot_export_log.h"
#endif

#include "esp_at.h"
#include "at_interface.h"

#ifndef CONFIG_AT_SOCKET_MAX_CONN_NUM
#define CONFIG_AT_SOCKET_MAX_CONN_NUM       1
#endif

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
static void at_wifi_init(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
}
#endif

static void at_netif_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();
#endif

#ifdef CONFIG_AT_ETHERNET_SUPPORT
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *eth_netif = esp_netif_new(&cfg);
    ESP_ERROR_CHECK(esp_eth_set_default_handlers(eth_netif));
#endif
}

void app_main(void)
{
    esp_at_main_preprocess();
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    at_netif_init();

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    at_wifi_init();
#endif

    at_interface_init();
    esp_at_board_init();

    uint8_t *version = (uint8_t *)malloc(256);
    sprintf((char*)version, "compile time(%s):%s %s\r\n", ESP_AT_PROJECT_COMMIT_ID, __DATE__, __TIME__);
#ifdef CONFIG_ESP_AT_FW_VERSION
    if ((strlen(CONFIG_ESP_AT_FW_VERSION) > 0) && (strlen(CONFIG_ESP_AT_FW_VERSION) <= 128)){
        printf("%s\r\n", CONFIG_ESP_AT_FW_VERSION);
        sprintf((char*)version + strlen((char*)version),"Bin version:%s(%s)\r\n", CONFIG_ESP_AT_FW_VERSION, esp_at_get_current_module_name());
    }
#endif
    esp_at_module_init(CONFIG_AT_SOCKET_MAX_CONN_NUM, version);  // reserved one for server
    free(version);

#ifdef CONFIG_AT_BASE_COMMAND_SUPPORT
    if(esp_at_base_cmd_regist() == false) {
        printf("regist base cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_USER_COMMAND_SUPPORT
    if (esp_at_user_cmd_regist() == false) {
        printf("regist user cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    if(esp_at_wifi_cmd_regist() == false) {
        printf("regist wifi cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_NET_COMMAND_SUPPORT
    if(esp_at_net_cmd_regist() == false) {
        printf("regist net cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_MQTT_COMMAND_SUPPORT
    if(esp_at_mqtt_cmd_regist() == false) {
        printf("regist mqtt cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_HTTP_COMMAND_SUPPORT
    if(esp_at_http_cmd_regist() == false) {
        printf("regist http cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_BLE_COMMAND_SUPPORT
    if(esp_at_ble_cmd_regist() == false) {
        printf("regist ble cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_BLE_HID_COMMAND_SUPPORT
    if(esp_at_ble_hid_cmd_regist() == false) {
        printf("regist ble hid cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_BLUFI_COMMAND_SUPPORT
    if(esp_at_blufi_cmd_regist() == false) {
        printf("regist blufi cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_BT_COMMAND_SUPPORT
    if(esp_at_bt_cmd_regist() == false) {
        printf("regist bt cmd fail\r\n");
    }
#ifdef CONFIG_AT_BT_SPP_COMMAND_SUPPORT
    if(esp_at_bt_spp_cmd_regist() == false) {
        printf("regist bt spp cmd fail\r\n");
    }
#endif
#ifdef CONFIG_AT_BT_A2DP_COMMAND_SUPPORT
    if(esp_at_bt_a2dp_cmd_regist() == false) {
        printf("regist bt a2dp cmd fail\r\n");
    }
#endif
#endif

#if defined(CONFIG_BT_ENABLED)
#if defined(CONFIG_AT_BLE_COMMAND_SUPPORT) || defined(CONFIG_AT_BLE_HID_COMMAND_SUPPORT) || defined(CONFIG_AT_BLUFI_COMMAND_SUPPORT)
#if !defined(CONFIG_AT_BT_COMMAND_SUPPORT)
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
#endif
#else
#if defined(CONFIG_AT_BT_COMMAND_SUPPORT)
    esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
#else
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
#endif
#endif
#endif

#if defined(CONFIG_AT_MDNS_COMMAND_SUPPORT)
    if(esp_at_mdns_cmd_regist() == false) {
        printf("regist mdns cmd fail\r\n");
    }
#endif

#if defined(CONFIG_AT_WPS_COMMAND_SUPPORT)
    if(esp_at_wps_cmd_regist() == false) {
        printf("regist wps cmd fail\r\n");
    }
#endif

#if defined(CONFIG_AT_DRIVER_COMMAND_SUPPORT)
    if(esp_at_driver_cmd_regist() == false) {
        printf("regist driver cmd fail\r\n");
    }
#endif

#if defined(CONFIG_AT_SMARTCONFIG_COMMAND_SUPPORT)
    if(esp_at_smartconfig_cmd_regist() == false) {
        printf("regist smartconfig cmd fail\r\n");
    }
#endif

#if defined(CONFIG_AT_PING_COMMAND_SUPPORT)
    if(esp_at_ping_cmd_regist() == false) {
        printf("regist ping cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_ETHERNET_SUPPORT
    if(esp_at_eth_cmd_regist() == false) {
        printf("regist ethernet cmd fail\r\n");
    }
#endif
    
#ifdef CONFIG_AT_FS_COMMAND_SUPPORT
    if(esp_at_fs_cmd_regist() == false) {
        printf("regist fs cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_EAP_COMMAND_SUPPORT
    if(esp_at_eap_cmd_regist() == false) {
        printf("regist eap cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_OTA_SUPPORT
    if (esp_at_ota_cmd_regist() == false) {
        printf("regist ota cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_SIGNALING_COMMAND_SUPPORT
    if (esp_at_fact_cmd_regist() == false) {
        printf("regist fact cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_WEB_SERVER_SUPPORT
    if (esp_at_web_server_cmd_regist() == false) {
        printf("regist web conf wifi cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_COMMAND_TERMINATOR
    uint8_t cmd_terminator[] = {CONFIG_AT_COMMAND_TERMINATOR, 0};
    esp_at_custom_cmd_line_terminator_set((uint8_t *)&cmd_terminator);
#endif

#ifdef CONFIG_AT_QCLOUD_IOT_COMMAND_SUPPORT
    IOT_Log_Set_Level(CONFIG_AT_QCLOUD_IOT_LOG_LEVEL);

    if(esp_at_qcloud_iot_cmd_regist() == false) {
        printf("qcloud at init fail\r\n");
    }
#endif

    at_custom_init();
}
