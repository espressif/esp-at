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

#if defined(CONFIG_BT_ENABLED)
#include "esp_bt.h"
#endif

#include "esp_at.h"

#include "at_upgrade.h"
#include "at_interface.h"

#ifdef CONFIG_AT_ETHERNET_SUPPORT
#include "at_eth_init.h"
#endif
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

void app_main()
{
    uint8_t *version = (uint8_t *)malloc(192);
#ifdef CONFIG_AT_COMMAND_TERMINATOR
    uint8_t cmd_terminator[2] = {CONFIG_AT_COMMAND_TERMINATOR,0};
#endif

    nvs_flash_init();
    at_interface_init();

    sprintf((char*)version, "compile time:%s %s\r\n", __DATE__, __TIME__);
#ifdef CONFIG_ESP_AT_FW_VERSION
    if ((strlen(CONFIG_ESP_AT_FW_VERSION) > 0) && (strlen(CONFIG_ESP_AT_FW_VERSION) <= 128)){
        printf("%s\r\n", CONFIG_ESP_AT_FW_VERSION);
        strcat((char*)version, CONFIG_ESP_AT_FW_VERSION);
    }
#endif
    esp_at_module_init (CONFIG_LWIP_MAX_SOCKETS - 1, version);  // reserved one for server
    free(version);

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

#ifdef CONFIG_AT_BLE_COMMAND_SUPPORT
    if(esp_at_ble_cmd_regist() == false) {
        printf("regist ble cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_BT_COMMAND_SUPPORT
    if(esp_at_bt_cmd_regist() == false) {
        printf("regist bt cmd fail\r\n");
    }
#endif

#if defined(CONFIG_BT_ENABLED)
#ifdef CONFIG_AT_BLE_COMMAND_SUPPORT
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

    esp_at_custom_cmd_array_regist (at_update_cmd, sizeof(at_update_cmd)/sizeof(at_update_cmd[0]));
    at_custom_init();
}
