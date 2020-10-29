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
#ifdef CONFIG_IDF_TARGET_ESP8266
#include "internal/esp_wifi_internal.h"
#endif
#endif

#if defined(CONFIG_BT_ENABLED)
#include "esp_bt.h"
#ifdef CONFIG_AT_BT_A2DP_COMMAND_SUPPORT
#include "at_i2s.h"
#endif
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

#define ESP_AT_VERSION_LEN_MAX                64
#define ESP_AT_PARTITION_NAME_LEN_MAX         64
#define NB_OTA_TASK_STACK_SIZE              5120  // for non-blocking ota

typedef struct {
    int32_t ota_mode;
    char version[ESP_AT_VERSION_LEN_MAX+1];
    char partition_name[ESP_AT_PARTITION_NAME_LEN_MAX+1];
} ota_param_t;

static bool s_esp_at_ota_started = false;

static uint8_t at_exeCmdCipupgrade(uint8_t *cmd_name)
{   
    if (s_esp_at_ota_started) {
        printf("ALREADY IN OTA\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_upgrade_process(ESP_AT_OTA_MODE_NORMAL, NULL, "ota")) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(portMAX_DELAY);
        esp_restart();
        for(;;){
        }
    }

    esp_at_set_upgrade_state(ESP_AT_OTA_STATE_FAILED);
    return ESP_AT_RESULT_CODE_ERROR;
}

static void non_blocking_upgrade_task(void *parameter)
{
    s_esp_at_ota_started = true;
    ota_param_t *p = (ota_param_t *)parameter;

    if (esp_at_upgrade_process((esp_at_ota_mode_type)(p->ota_mode), (uint8_t *)(p->version), p->partition_name)) {
        esp_at_port_wait_write_complete(portMAX_DELAY);
    } else {
        esp_at_set_upgrade_state(ESP_AT_OTA_STATE_FAILED);
        esp_at_port_write_data((uint8_t*)"+CIPUPDATE:-1\r\n", strlen("+CIPUPDATE:-1\r\n"));
    }

    free(p);
    p = NULL;
    s_esp_at_ota_started = false;
    vTaskDelete(NULL);
}

static uint8_t at_setupCmdCipupgrade(uint8_t para_num)
{
#define ESP_AT_NONBLOCKING_MODE_MAX     2
    ota_param_t *param = NULL;
    uint8_t *version = NULL, *partition_name = NULL;
    int32_t cnt = 0, ota_mode = 0, nonblocking = 0, version_len = 0;

    if (esp_at_get_para_as_digit(cnt++, &ota_mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((ota_mode < 0) || (ota_mode >= ESP_AT_OTA_MODE_MAX)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        if (esp_at_get_para_as_str(cnt++, &version) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if (version != NULL) {
            version_len = strlen((const char *)version);
            if (version_len > ESP_AT_VERSION_LEN_MAX) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
    }

    if (cnt < para_num) {
        if (esp_at_get_para_as_str(cnt++, &partition_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if ((partition_name == NULL) || (strlen((const char *)partition_name) == 0)) {
        partition_name = (uint8_t *)"ota";
    } else if ((strlen((const char *)partition_name)) > ESP_AT_PARTITION_NAME_LEN_MAX) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        if (esp_at_get_para_as_digit(cnt++, &nonblocking) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if ((nonblocking < 0) || (nonblocking >= ESP_AT_NONBLOCKING_MODE_MAX)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (s_esp_at_ota_started) {
        printf("ALREADY IN OTA\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (nonblocking == 1) {
        param = (ota_param_t *)calloc(1, sizeof(ota_param_t));
        if (param == NULL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        param->ota_mode = ota_mode;

        if (version_len > 0) {
            memcpy(param->version, version, version_len);
        } else {
            version = NULL;
        }

        memcpy(param->partition_name, partition_name, strlen((const char *)partition_name));

        if (xTaskCreate(non_blocking_upgrade_task, "nb-ota", NB_OTA_TASK_STACK_SIZE, (void *)param, 5, NULL) != pdPASS) {
            free(param);
            param = NULL;
            return ESP_AT_RESULT_CODE_ERROR;
        }
        return ESP_AT_RESULT_CODE_OK;
    }

    if (esp_at_upgrade_process(ota_mode, version, (const char *)partition_name)) {
        if (memcmp(partition_name, "ota", strlen("ota")) == 0) {
            esp_at_response_result(ESP_AT_RESULT_CODE_OK);
            esp_at_port_wait_write_complete(portMAX_DELAY);
            esp_restart();
            for(;;){
            }
        } else {
            return ESP_AT_RESULT_CODE_OK;
        }
    }

    esp_at_set_upgrade_state(ESP_AT_OTA_STATE_FAILED);
    return ESP_AT_RESULT_CODE_ERROR;
}

static uint8_t at_queryCmdCipupgrade(uint8_t *cmd_name)
{
    uint8_t buffer[32] = {0};
    esp_at_ota_state_t state = esp_at_get_upgrade_state();
    snprintf((char *)buffer, sizeof(buffer), "%s:%d\r\n", cmd_name, state);
    esp_at_port_write_data(buffer, strlen((char *)buffer));
    return ESP_AT_RESULT_CODE_OK;
}

static esp_at_cmd_struct at_upgrade_cmd[] = {
    {"+CIUPDATE", NULL, at_queryCmdCipupgrade, at_setupCmdCipupgrade, at_exeCmdCipupgrade},
};
#endif

#ifdef CONFIG_AT_SIGNALING_COMMAND_SUPPORT
static uint8_t at_setupCmdFactPlcp(uint8_t para_num)
{
    int32_t enable = 0;
    int32_t tx_with_long = 0;
    int32_t cnt = 0;

    if (esp_at_get_para_as_digit (cnt++, &enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((enable != 0) && (enable != 1)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit (cnt++, &tx_with_long) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((tx_with_long != 0) && (tx_with_long != 1)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    esp_wifi_set_11b_tx_plcp(enable, tx_with_long);

    return ESP_AT_RESULT_CODE_OK;
}

static esp_at_cmd_struct at_fact_cmd[] = {
    {"+FACTPLCP", NULL, NULL, at_setupCmdFactPlcp, NULL},
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
    } else {
        esp_at_i2s_init();
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

#ifdef CONFIG_AT_COMMAND_TERMINATOR
    esp_at_custom_cmd_line_terminator_set((uint8_t*)&cmd_terminator);
#endif

#ifdef CONFIG_AT_OTA_SUPPORT
    esp_at_custom_cmd_array_regist (at_upgrade_cmd, sizeof(at_upgrade_cmd)/sizeof(at_upgrade_cmd[0]));
#endif

#ifdef CONFIG_AT_SIGNALING_COMMAND_SUPPORT
    esp_at_custom_cmd_array_regist (at_fact_cmd, sizeof(at_fact_cmd)/sizeof(at_fact_cmd[0]));
#endif
    at_custom_init();
}
