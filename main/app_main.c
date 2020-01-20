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
#include "esp_event_loop.h"
#include "esp_wifi.h"
#endif

#if defined(CONFIG_BT_ENABLED)
#include "esp_bt.h"
#endif

#include "esp_at.h"

#ifdef CONFIG_AT_OTA_SUPPORT
#include "at_upgrade.h"
#endif

#include "at_interface.h"
#include "at_default_config.h"

#ifdef CONFIG_AT_ETHERNET_SUPPORT
#include "at_eth_init.h"
#endif

#ifdef CONFIG_AT_OTA_SUPPORT
static uint8_t at_exeCmdCipupdate(uint8_t *cmd_name)//add get station ip and ap ip
{

    if (esp_at_upgrade_process(ESP_AT_OTA_MODE_NORMAL,NULL)) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(portMAX_DELAY);
        esp_restart();
        for(;;){
        }
    }

    return ESP_AT_RESULT_CODE_ERROR;
}

static uint8_t at_setupCmdCipupdate(uint8_t para_num)
{
    int32_t ota_mode = 0;
    int32_t cnt = 0;
    uint8_t* version = NULL;

    if (esp_at_get_para_as_digit (cnt++,&ota_mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        if(esp_at_get_para_as_str (cnt++,&version) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_upgrade_process(ota_mode,version)) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(portMAX_DELAY);
        esp_restart();
        for(;;){
        }
    }

    return ESP_AT_RESULT_CODE_ERROR;
}

static esp_at_cmd_struct at_update_cmd[] = {
    {"+CIUPDATE", NULL, NULL, at_setupCmdCipupdate, at_exeCmdCipupdate},
};
#endif

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
static esp_err_t at_wifi_event_handler(void *ctx, system_event_t *event)
{
    esp_err_t ret = esp_at_wifi_event_handler(ctx, event);

    return ret;
}

static void initialise_wifi(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK( esp_event_loop_init(at_wifi_event_handler, NULL) );
    
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}
#endif

void app_main()
{
    const esp_partition_t * partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
    char* data = NULL;
    uint32_t module_id = 0;

	if (partition) {
	    data = (char*)malloc(ESP_AT_FACTORY_PARAMETER_SIZE); // Notes
	    assert(data != NULL);
	    if(esp_partition_read(partition, 0, data, ESP_AT_FACTORY_PARAMETER_SIZE) != ESP_OK){
	        free(data);
	        printf("esp_partition_read failed\r\n");
	        return ;
	    } else {
	        module_id = data[3];
            free(data);
	    }
	} else {
        printf("factory_parameter partition missed\r\n");
    }

    const char* module_name = esp_at_get_module_name_by_id(module_id);
    uint8_t *version = (uint8_t *)malloc(256);
#ifdef CONFIG_AT_COMMAND_TERMINATOR
    uint8_t cmd_terminator[2] = {CONFIG_AT_COMMAND_TERMINATOR,0};
#endif

    nvs_flash_init();
    tcpip_adapter_init();
#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    initialise_wifi();
#endif
    at_interface_init();

    sprintf((char*)version, "compile time(%s):%s %s\r\n", ESP_AT_PROJECT_COMMIT_ID, __DATE__, __TIME__);
#ifdef CONFIG_ESP_AT_FW_VERSION
    if ((strlen(CONFIG_ESP_AT_FW_VERSION) > 0) && (strlen(CONFIG_ESP_AT_FW_VERSION) <= 128)){
        printf("%s\r\n", CONFIG_ESP_AT_FW_VERSION);
        sprintf((char*)version + strlen((char*)version),"Bin version:%s(%s)\r\n", CONFIG_ESP_AT_FW_VERSION, module_name);
    }
#endif
    esp_at_module_init (CONFIG_AT_SOCKET_MAX_CONN_NUM, version);  // reserved one for server
    free(version);
    esp_at_factory_parameter_init();

#ifdef CONFIG_AT_BASE_COMMAND_SUPPORT
    if(esp_at_base_cmd_regist() == false) {
        printf("regist base cmd fail\r\n");
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
    if(at_eth_init() == false) {
        printf("ethernet init fail\r\n");
    }else {
        if(esp_at_eth_cmd_regist() == false) {
            printf("regist ethernet cmd fail\r\n");
        }
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

#ifdef CONFIG_AT_COMMAND_TERMINATOR
    esp_at_custom_cmd_line_terminator_set((uint8_t*)&cmd_terminator);
#endif

#ifdef CONFIG_AT_OTA_SUPPORT
    esp_at_custom_cmd_array_regist (at_update_cmd, sizeof(at_update_cmd)/sizeof(at_update_cmd[0]));
#endif
    at_custom_init();
}
