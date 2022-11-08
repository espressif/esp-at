/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2022 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_at.h"
#include "esp_at_core.h"

#include "esp_rmaker_core.h"
#include "esp_rmaker_standard_types.h"
#include "esp_rmaker_standard_params.h"
#include "esp_rmaker_standard_devices.h"
#include "esp_rmaker_schedule.h"
#include "esp_rmaker_scenes.h"
#include "esp_rmaker_console.h"
#include <esp_rmaker_ota.h>
#include "esp_rmaker_utils.h"
#include <esp_rmaker_user_mapping.h>
#include <esp_rmaker_work_queue.h>

#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_ble.h>

#include <esp_rmaker_mqtt.h>
#include <esp_rmaker_common_events.h>

#define CHECK_PARAMS_NUM(cnt, total)                \
    if (cnt != total) {                             \
        return ESP_AT_RESULT_CODE_ERROR;            \
    }

#define RAINMAKER_PRE_CFG_PARTITION                 ("rmaker_mfg")      // please refer to at_customize.csv
#define RAINMAKER_PRE_NAMESPACE                     ("mfg_params")
#define RAINMAKER_PRE_KEY                           ("params")

#define RAINMAKER_PARAMS_MAX_SETS                   (16)
#define RAINMAKER_PARAMS_KV_GROUP_MAX               (RAINMAKER_PARAMS_MAX_SETS / 2 + 1)     // +1 for loop exit
#define RAINMAKER_NODE_ATTR_MAX_SETS                (16)
#define RAINMAKER_NODE_ATTR_KV_GROUP_MAX            (RAINMAKER_NODE_ATTR_MAX_SETS / 2 + 1)  // +1 for loop exit
#define RAINMAKER_PARAM_STR_LIST_MAX_SETS           (8)
#define RAINMAKER_PARAM_STR_LIST_ARAAY_MAX          (RAINMAKER_PARAM_STR_LIST_MAX_SETS + 1) // +1 for NULL termination

#define RAINMAKER_PASSTHROUGH_BUFFER_LEN            (2048)
#define RAINMAKER_PASSTHROUGH_MAX_PARAM_NUMS        (1)

#define RAINMAKER_NODE_NAME                         ("ESP RainMaker AT Node")
#define RAINMAKER_NODE_TYPE                         ("AT Node")

#define DATA_FORMAT                                 "+RMRECV:%s,%s,%s:"
#define SWAP_16(x)                                  ((uint16_t)((((x)&0xff00) >> 8) | (((x)&0x00ff) << 8)))
#define BLE_NAME_LEN_MAX                            (12)

#define RAINMAKER_MESS_CONNECTED                    ("+RMCONNECTED")
#define RAINMAKER_MESS_DISCONNECTED                 ("+RMDISCONNECTED")
#define RAINMAKER_MESS_RMRESET                      ("+RMRESET")
#define RAINMAKER_MESS_RMREBOOT                     ("+RMREBOOT")
#define RAINMAKER_MESS_RMTIMEZONE                   ("+RMTIMEZONE")
#define RAINMAKER_MESS_MAPPINGDONE                  ("+RMMAPPINGDONE")

#define RAINMAKER_CONN_TIMEOUT_S_MIN                (3)     // 3 seconds
#define RAINMAKER_CONN_TIMEOUT_S_MAX                (600)   // 600 seconds
#define RAINMAKER_CONN_TIMEOUT_S_DEF                (15)    // 15 seconds

static const int RM_NODE_INIT_DONE_EVENT            = BIT0;
static const int RM_MQTT_CONNECTED_EVENT            = BIT1;
static const int RM_PROVISIONING_STATUS             = BIT2;
static const int RM_IN_PASSTHROUGH_MODE             = BIT3;
static const int RM_CORE_STARTED_EVENT              = BIT4;
static const int RM_DEVICE_ADDED_EVENT              = BIT5;
static const int RM_WIFI_GOT_IP_STATUS              = BIT6;

typedef enum {
    RM_CONNECT_STATUS_ON = 0,
    RM_CONNECT_STATUS_OFF = 1,
} rm_connect_status_t;

static rm_connect_status_t rm_connect_status = RM_CONNECT_STATUS_OFF;
static uint32_t s_rm_param_nums = 0;

extern esp_rmaker_val_type_t esp_rmaker_param_get_data_type(const esp_rmaker_param_t *param);
extern esp_err_t esp_rmaker_reset_user_node_mapping(void);
static esp_err_t rmaker_prov_start(uint8_t *mfg, int mfg_len, uint8_t timeout_s, char *broadcast_name);
static esp_err_t rmaker_prov_stop(void);

typedef enum {
    AT_START_RM_PROV = 0,
    AT_STOP_RM_PROV,
} at_rainmaker_prov_mode_t;

typedef enum {
    AT_RM_NORMAL = 0,
    AT_RM_PASSTHROUGH,
} at_rainmaker_mode_t;

typedef enum {
    AT_RM_CONNECTED = 0,
    AT_RM_DISCONNECTED,
    AT_RM_RESET,
    AT_RM_REBOOT,
    AT_RM_TIMEZONE,
    AT_RM_MAPPINGDONE,
} at_rainmaker_mess_t;

typedef enum {
    AT_RM_DEV_OPT_ADD = 0,
    AT_RM_DEV_OPT_DEL,
} at_rainmaker_dev_opt_t;

typedef struct {
    char *device_type;
    uint16_t device_type_code;
} at_rm_device_code_t;

typedef struct {
    uint8_t *key;
    uint8_t *value;
} at_rm_kv_data_t;

typedef struct {
    uint16_t customer_id;
    char ble_name[BLE_NAME_LEN_MAX + 1]; // +1 for NULL termination
    uint8_t device_extra_code;
    uint32_t device_type_code;
    esp_rmaker_param_t *passthrough_param_handle;
} at_rm_profile_t;

static const at_rm_device_code_t code_list[10] = {
    {
        .device_type = "esp.device.light",
        .device_type_code = 0x0005,
    },
    {
        .device_type = "esp.device.lightbulb",
        .device_type_code = 0x0005,
    },
    {
        .device_type = "esp.device.switch",
        .device_type_code = 0x0080,
    },
    {
        .device_type = "esp.device.plug",
        .device_type_code = 0x0040,
    },
    {
        .device_type = "esp.device.socket",
        .device_type_code = 0x0040,
    },
    {
        .device_type = "esp.device.outlet",
        .device_type_code = 0x0040,
    },
    {
        .device_type = "esp.device.other",
        .device_type_code = 0x00FF,
    },
    {
        /* Must be NULL for loop exit */
        .device_type = NULL,
        /* For test data only, the actual value is 0x00ff */
        .device_type_code = 0x0001,
    }
};

static const char *TAG = "rainmaker";

static SemaphoreHandle_t s_at_rainmaker_sync_sema;
static TimerHandle_t s_prov_timer;
static TimerHandle_t s_mqtt_conn_timer;
static EventGroupHandle_t s_rm_event_group;
static at_rainmaker_mode_t s_mode = AT_RM_NORMAL;

static at_rm_profile_t s_profile = {
    .customer_id = 0x0001,
    .ble_name = "PROV_",
    .device_extra_code = 0x00,
    /* For test data only, the actual value is 0x00ff */
    .device_type_code = 0x0005,
};

static void at_rainmaker_wait_data_cb(void)
{
    xSemaphoreGive(s_at_rainmaker_sync_sema);
}

static void timer_cb(TimerHandle_t tmr)
{
    if (tmr == s_prov_timer) {
        ESP_LOGI(TAG, "Provisioning stop.");
        rmaker_prov_stop();
    } else if (tmr == s_mqtt_conn_timer) {
        esp_rmaker_mqtt_connect();
    }
}

static void rmaker_response_result(at_rainmaker_mess_t message)
{
#define TEMP_BUFFER_SIZE    32
    uint8_t buffer[TEMP_BUFFER_SIZE] = {0};

    switch (message) {
    case AT_RM_CONNECTED:
        if (rm_connect_status == RM_CONNECT_STATUS_OFF) {
            snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\n%s", RAINMAKER_MESS_CONNECTED);
            rm_connect_status = RM_CONNECT_STATUS_ON;
        }
        break;

    case AT_RM_DISCONNECTED:
        if (rm_connect_status == RM_CONNECT_STATUS_ON) {
            snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\n%s", RAINMAKER_MESS_DISCONNECTED);
            rm_connect_status = RM_CONNECT_STATUS_OFF;
        }
        break;

    case AT_RM_RESET:
        snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\n%s", RAINMAKER_MESS_RMRESET);
        break;

    case AT_RM_REBOOT:
        snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\n%s", RAINMAKER_MESS_RMREBOOT);
        break;

    case AT_RM_TIMEZONE:
        snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\n%s", RAINMAKER_MESS_RMTIMEZONE);
        break;

    case AT_RM_MAPPINGDONE:
        snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\n%s", RAINMAKER_MESS_MAPPINGDONE);
        break;

    default:
        break;

    }

    esp_at_port_write_data(buffer, strlen((char *)buffer));

    return;
}

static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                          const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx)
{
    if (ctx) {
        ESP_LOGI(TAG, "Received write request via : %s", esp_rmaker_device_cb_src_to_str(ctx->src));
    }
    const char *device_name = esp_rmaker_device_get_name(device);
    const char *param_name = esp_rmaker_param_get_name(param);
    const char *src_name = esp_rmaker_device_cb_src_to_str(ctx->src);

    // passthrough mode
    if ((xEventGroupGetBits(s_rm_event_group) & RM_IN_PASSTHROUGH_MODE)) {
        char *buf = NULL;
        if (val.type == RMAKER_VAL_TYPE_BOOLEAN) {
            asprintf(&buf, "%d", val.val.b);
        } else if (val.type == RMAKER_VAL_TYPE_INTEGER) {
            asprintf(&buf, "%d", val.val.i);
        } else if (val.type == RMAKER_VAL_TYPE_FLOAT) {
            asprintf(&buf, "%f", val.val.f);
        } else {
            asprintf(&buf, "%s", val.val.s);
        }
        esp_at_port_write_data((uint8_t *)buf, strlen((char *)buf));
        free(buf);
    } else {
        char *buf = NULL;
        if (val.type == RMAKER_VAL_TYPE_BOOLEAN) {
            asprintf(&buf, "\r\n"DATA_FORMAT"%d""\r\n", src_name, device_name, param_name, val.val.b);
        } else if (val.type == RMAKER_VAL_TYPE_INTEGER) {
            asprintf(&buf, "\r\n"DATA_FORMAT"%d""\r\n", src_name, device_name, param_name, val.val.i);
        } else if (val.type == RMAKER_VAL_TYPE_FLOAT) {
            asprintf(&buf, "\r\n"DATA_FORMAT"%f""\r\n", src_name, device_name, param_name, val.val.f);
        } else {
            asprintf(&buf, "\r\n"DATA_FORMAT"%s""\r\n", src_name, device_name, param_name, val.val.s);
        }
        esp_at_port_write_data((uint8_t *)buf, strlen((char *)buf));
        free(buf);
    }

    /* auto response to cloud */
    esp_rmaker_param_update_and_report(param, val);
    return ESP_OK;
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
        case WIFI_PROV_START:
            ESP_LOGI(TAG, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV: {
            wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
            ESP_LOGI(TAG, "Received Wi-Fi credentials"
                     "\n\tSSID     : %s\n\tPassword : %s",
                     (const char *)wifi_sta_cfg->ssid,
                     (const char *)wifi_sta_cfg->password);
            break;
        }
        case WIFI_PROV_CRED_FAIL: {
            wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
            ESP_LOGE(TAG, "Provisioning failed!\n\tReason : %s",
                     (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "Wi-Fi station authentication failed" : "Wi-Fi access-point not found");
            wifi_prov_mgr_reset_sm_state_on_failure();
            break;
        }
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(TAG, "Provisioning successful");
            if (!s_prov_timer) {
                s_prov_timer = xTimerCreate("prov stop timer", pdMS_TO_TICKS(1000 * 5), false, NULL, timer_cb);
            }
            xTimerReset(s_prov_timer, 0);
            break;
        case WIFI_PROV_END:
            wifi_prov_mgr_deinit();
            break;
        default:
            break;
        }
    } else if (event_base == RMAKER_COMMON_EVENT) {
        switch (event_id) {
        case RMAKER_MQTT_EVENT_CONNECTED:
            rmaker_response_result(AT_RM_CONNECTED);
            ESP_LOGI(TAG, "MQTT Connected.");
            xEventGroupSetBits(s_rm_event_group, RM_MQTT_CONNECTED_EVENT);
            break;
        case RMAKER_MQTT_EVENT_DISCONNECTED:
            rmaker_response_result(AT_RM_DISCONNECTED);
            ESP_LOGI(TAG, "MQTT Disconnected.");
            xEventGroupClearBits(s_rm_event_group, RM_MQTT_CONNECTED_EVENT);
            break;
        case RMAKER_EVENT_REBOOT:
            rmaker_response_result(AT_RM_REBOOT);
            ESP_LOGI(TAG, "AT_RM_REBOOT.");
            break;
        case RMAKER_EVENT_WIFI_RESET:
        case RMAKER_EVENT_FACTORY_RESET:
            rmaker_response_result(AT_RM_RESET);
            ESP_LOGI(TAG, "AT_RM_RESET.");
            break;
        case RMAKER_EVENT_TZ_POSIX_CHANGED:
        case RMAKER_EVENT_TZ_CHANGED:
            //rmaker_response_result(AT_RM_TIMEZONE);
            ESP_LOGI(TAG, "AT_RM_TIMEZONE.");
            break;
        default:
            break;
        }
    } else if (event_base == RMAKER_EVENT) {
        switch (event_id) {
        case RMAKER_EVENT_USER_NODE_MAPPING_DONE:
            rmaker_response_result(AT_RM_MAPPINGDONE);
            break;
        default:
            break;
        }
    } else if (event_base == WIFI_EVENT) {
        switch (event_id) {
        case WIFI_EVENT_STA_DISCONNECTED:
            xEventGroupClearBits(s_rm_event_group, RM_WIFI_GOT_IP_STATUS);
            break;
        default:
            ESP_LOGI(TAG, "WIFI_EVENT: %d", event_id);
            break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
        case IP_EVENT_STA_GOT_IP:
            xEventGroupSetBits(s_rm_event_group, RM_WIFI_GOT_IP_STATUS);
            break;
        default:
            ESP_LOGI(TAG, "IP_EVENT: %d", event_id);
            break;
        }
    }
}

static esp_err_t rmaker_generate_mfg_info(uint8_t inbuf[], int buf_len, uint32_t customer_id, uint32_t device_type_code, uint8_t device_extra_code)
{
    uint16_t _customer_id = SWAP_16((customer_id >= 0x000A ? customer_id : 0x0001));
    uint16_t _device_type_code = SWAP_16(device_type_code);

    // Fixed
    uint8_t mfg[12] = { 0xe5, 0x02, 'N', 'o', 'v', 'b'};

    // Customer id
    memcpy(&mfg[6], &_customer_id, 2);

    // Device type code
    memcpy(&mfg[8], &_device_type_code, 2);

    // Device subtype code, Not used
    mfg[10] = 0x00;

    // device_extra_code
    mfg[11] = device_extra_code;

    memcpy(inbuf, mfg, buf_len);

    return ESP_OK;
}

static uint16_t rmaker_get_device_code(uint8_t *device_type)
{
    /* For test data only, the actual value is 0x00ff */
    uint32_t device_type_code = 0x0005;

    // query device code
    for (int i = 0; code_list[i].device_type; i++) {
        if (strcmp(code_list[i].device_type, (const char *)device_type) == 0) {
            device_type_code = code_list[i].device_type_code;
            break;
        }
    }
    return device_type_code;
}

static void rmaker_structure_data(esp_rmaker_val_type_t type, uint8_t *inbuf, esp_rmaker_param_val_t *out_val)
{
    esp_rmaker_param_val_t val;
    memset(&val, 0, sizeof(esp_rmaker_param_val_t));
    if (type == RMAKER_VAL_TYPE_BOOLEAN) {
        int res = (float)atoi((const char *)inbuf);
        val = esp_rmaker_bool(res);
    } else if (type == RMAKER_VAL_TYPE_INTEGER) {
        int res = (float)atoi((const char *)inbuf);
        val = esp_rmaker_int(res);
    } else if (type == RMAKER_VAL_TYPE_FLOAT) {
        float res = (float)atof((const char *)inbuf);
        val = esp_rmaker_float(res);
    } else if (type == RMAKER_VAL_TYPE_STRING) {
        val = esp_rmaker_str((const char *)inbuf);
    } else if (type == RMAKER_VAL_TYPE_OBJECT) {
        val = esp_rmaker_obj((const char *)inbuf);
    } else if (type == RMAKER_VAL_TYPE_ARRAY) {
        val = esp_rmaker_array((const char *)inbuf);
    }
    memcpy(out_val, &val, sizeof(esp_rmaker_param_val_t));
}

static esp_err_t rmaker_prov_start(uint8_t *mfg, int mfg_len, uint8_t timeout_s, char *broadcast_name)
{
    wifi_prov_mgr_config_t mgr_config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM
    };

    wifi_prov_mgr_init(mgr_config);
    ESP_LOGI(TAG, "Starting provisioning");

    wifi_prov_scheme_ble_set_mfg_data(mfg, mfg_len);
    wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, NULL, broadcast_name, NULL);

    xEventGroupSetBits(s_rm_event_group, RM_PROVISIONING_STATUS);

    return ESP_OK;
}

static esp_err_t rmaker_prov_stop(void)
{
    wifi_prov_mgr_stop_provisioning();
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    xEventGroupClearBits(s_rm_event_group, RM_PROVISIONING_STATUS);

    return ESP_OK;
}

static uint8_t at_exe_cmd_rmnodeinit(uint8_t *cmd_name)
{
    if (!s_rm_event_group) {
        s_rm_event_group = xEventGroupCreate();
        if (!s_rm_event_group) {
            ESP_LOGE(TAG, "Couldn't create rm event group.");
            //TODO: err code, unkown error
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        return ESP_AT_RESULT_CODE_OK;
    }

    // init rmaker frame
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = false,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, RAINMAKER_NODE_NAME, RAINMAKER_NODE_TYPE);
    if (!node) {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        //TODO: error code, Claiming fail
        return ESP_AT_RESULT_CODE_ERROR;
    }

    //TODO: Load configuration from flash

    //register event
    esp_event_handler_register(RMAKER_COMMON_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(RMAKER_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);

    //enable service
    esp_rmaker_ota_config_t ota_config = {
        .server_cert = ESP_RMAKER_OTA_DEFAULT_SERVER_CERT,
    };
    esp_rmaker_system_serv_config_t serv_config = {
        .flags = SYSTEM_SERV_FLAGS_ALL,

        // currently, APP can only triggle ESP RainMaker cloud to send  "esp.param.factory-reset" message
        // after the device receives the message, it will only perform the reset operation
        // the reboot operation is determined by the Host MCU
        .reset_reboot_seconds = -1,
    };
    esp_rmaker_system_service_enable(&serv_config);
    esp_rmaker_ota_enable(&ota_config, OTA_USING_TOPICS);
    esp_rmaker_timezone_service_enable();
    esp_rmaker_schedule_enable();

    xEventGroupSetBits(s_rm_event_group, RM_NODE_INIT_DONE_EVENT);

    // set fw_version field in "info" in node configuration
    // format like "2.4.0(esp32c3_MINI-1_4b42408):Sep 20 2022 11:58:48"
    char *version = (char *)calloc(1, 256);
    sprintf(version,"%s(%s_%s_%s):%s %s",
        CONFIG_ESP_AT_FW_VERSION, CONFIG_IDF_TARGET,
        esp_at_get_current_module_name(),
        ESP_AT_PROJECT_COMMIT_ID,
        __DATE__,
        __TIME__);
    esp_rmaker_node_add_fw_version(node, (const char*)version);
    free(version);

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmnodeattr(uint8_t para_num)
{
    int cnt = 0;
    at_rm_kv_data_t kv_group[RAINMAKER_NODE_ATTR_KV_GROUP_MAX];
    memset(&kv_group[0], 0, sizeof(at_rm_kv_data_t) * RAINMAKER_NODE_ATTR_KV_GROUP_MAX);

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, already connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // name_1
    if (esp_at_get_para_as_str(cnt++, &kv_group[0].key) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // value_1
    if (esp_at_get_para_as_str(cnt++, &kv_group[0].value) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        // Supports 4 groups of 2 parameters each, So the for loop handles up to 8 parameters here.
        // `-2` means 2 parameters have been processed before
        // `/2` means process 2 parameters at once
        for (int i = 0; i < ((RAINMAKER_NODE_ATTR_MAX_SETS - 2) / 2); i++) {
            // next name
            if (esp_at_get_para_as_str(cnt++, &kv_group[i + 1].key) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            // next value
            if (esp_at_get_para_as_str(cnt++, &kv_group[i + 1].value) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            if (cnt == para_num) {
                break;
            }
        }
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    // add attribute to node
    for (int i = 0; kv_group[i].key; i++) {
        esp_rmaker_node_add_attribute(esp_rmaker_get_node(), (const char *)kv_group[i].key, (const char *)kv_group[i].value);
        ESP_LOGI(TAG, "Added attr key:%s value:%s", kv_group[i].key, kv_group[i].value);
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmdev(uint8_t para_num)
{
    int32_t cnt = 0;
    int32_t dev_opt = AT_RM_DEV_OPT_ADD;
    uint8_t *unique_name = NULL;
    uint8_t *device_name = NULL;
    uint8_t *device_type = NULL;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, already connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // dev_opt
    if (esp_at_get_para_as_digit(cnt++, &dev_opt) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((dev_opt != AT_RM_DEV_OPT_ADD) && (dev_opt != AT_RM_DEV_OPT_DEL)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (dev_opt == AT_RM_DEV_OPT_ADD) {
        if ((xEventGroupGetBits(s_rm_event_group) & RM_DEVICE_ADDED_EVENT)) {
            //TODO: error code, already added
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // unique_name
        if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // device_name
        if (esp_at_get_para_as_str(cnt++, &device_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // device_type
        if (esp_at_get_para_as_str(cnt++, &device_type) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // parameters are ready
        CHECK_PARAMS_NUM(cnt, para_num);

        // Create device
        esp_rmaker_device_t *device = esp_rmaker_device_create((const char *)unique_name, (const char *)device_type, NULL);
        if (!device) {
            //TODO: error code, unique_name already used
            return ESP_AT_RESULT_CODE_ERROR;
        }
        esp_rmaker_device_add_cb(device, write_cb, NULL);
        // Name is a required parameter
        esp_rmaker_device_add_param(device, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, (const char *)device_name));
        esp_rmaker_node_add_device(esp_rmaker_get_node(), device);

        //Update device code
        s_profile.device_type_code = rmaker_get_device_code(device_type);

        xEventGroupSetBits(s_rm_event_group, RM_DEVICE_ADDED_EVENT);
    } else {
        // unique_name
        if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // parameters are ready
        CHECK_PARAMS_NUM(cnt, para_num);

        esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
        if (!device) {
            //TODO: error code, device not found
            return ESP_AT_RESULT_CODE_ERROR;
        }

        esp_rmaker_node_remove_device(esp_rmaker_get_node(), device);
        esp_rmaker_device_delete(device);

        xEventGroupClearBits(s_rm_event_group, RM_DEVICE_ADDED_EVENT);
    }

    /* Reset the number of parameters in the device */
    s_rm_param_nums = 0;

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmparam(uint8_t para_num)
{
    int32_t cnt = 0;
    uint8_t *unique_name = NULL;
    uint8_t *param_name = NULL;
    uint8_t *param_type = NULL;
    int32_t data_type = 0;
    int32_t properties = 0;
    uint8_t *ui_type = NULL;
    uint8_t *def = NULL;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, already connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // unique_name
    if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_name
    if (esp_at_get_para_as_str(cnt++, &param_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_type
    if (esp_at_get_para_as_str(cnt++, &param_type) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // data_type
    if (esp_at_get_para_as_digit(cnt++, &data_type) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // properties
    if (esp_at_get_para_as_digit(cnt++, &properties) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // ui_type
    if (esp_at_get_para_as_str(cnt++, &ui_type) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // def
    if (esp_at_get_para_as_str(cnt++, &def) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    // device check
    esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
    if (!device) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // Handling different data types
    esp_rmaker_param_val_t val = {0};
    rmaker_structure_data((esp_rmaker_val_type_t)data_type, def, &val);

    // Create parameters
    esp_rmaker_param_t *param = esp_rmaker_param_create((const char *)param_name,
                                (const char *)param_type,
                                val,
                                properties);

    // Add ui type
    esp_rmaker_param_add_ui_type(param, (const char *)ui_type);

    // Add parameters to the device
    esp_err_t err = esp_rmaker_device_add_param(device, param);
    if (err != ESP_OK) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }

    /* Record the number of parameters, which will be checked in the transmission mode */
    s_rm_param_nums++;

    /* Passthrough mode only allows one parameter */
    if (s_rm_param_nums == RAINMAKER_PASSTHROUGH_MAX_PARAM_NUMS) {
        s_profile.passthrough_param_handle = param;
    } else {
        s_profile.passthrough_param_handle = NULL;
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmparambounds(uint8_t para_num)
{
    int cnt = 0;
    uint8_t *unique_name = NULL;
    uint8_t *param_name = NULL;
    uint8_t *param_type = NULL;
    uint8_t *min = NULL;
    uint8_t *max = NULL;
    uint8_t *step = NULL;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, already connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // unique_name
    if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_name
    if (esp_at_get_para_as_str(cnt++, &param_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // min
    if (esp_at_get_para_as_str(cnt++, &min) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // max
    if (esp_at_get_para_as_str(cnt++, &max) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // step
    if (esp_at_get_para_as_str(cnt++, &step) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
    if (!device) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_param_t *param = esp_rmaker_device_get_param_by_name(device, (const char *)param_name);
    if (!param) {
        //TODO: error code, param not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_val_type_t type = esp_rmaker_param_get_data_type(param);
    if (type == RMAKER_VAL_TYPE_INVALID) {
        //TODO: error code, param type invalid
        return ESP_AT_RESULT_CODE_ERROR;
    } else if (type >= RMAKER_VAL_TYPE_FLOAT) {
        //TODO: error code, param type error
        return ESP_AT_RESULT_CODE_ERROR;
    }

    esp_rmaker_param_val_t val_min;
    esp_rmaker_param_val_t val_max;;
    esp_rmaker_param_val_t val_step;

    rmaker_structure_data(type, min, &val_min);
    rmaker_structure_data(type, max, &val_max);
    rmaker_structure_data(type, step, &val_step);
    esp_rmaker_param_add_bounds(param, val_min, val_max, val_step);

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmparamstrlist(uint8_t para_num)
{
    int cnt = 0;
    uint8_t *unique_name = NULL;
    uint8_t *param_name = NULL;
    char *str_list[RAINMAKER_PARAM_STR_LIST_ARAAY_MAX];
    memset(str_list, 0, sizeof(char *) * RAINMAKER_PARAM_STR_LIST_ARAAY_MAX);

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, already connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // unique_name
    if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_name
    if (esp_at_get_para_as_str(cnt++, &param_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // str1
    if (esp_at_get_para_as_str(cnt++, (uint8_t **)(&str_list[0])) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        // Supports 8 parameters
        // `-1` means one parameters have been processed before
        for (int i = 0; i < (RAINMAKER_PARAM_STR_LIST_MAX_SETS - 1); i++) {
            if (cnt != para_num) {
                // next str
                if (esp_at_get_para_as_str(cnt++, (uint8_t **)(&str_list[i + 1])) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
            }
        }
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
    if (!device) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_param_t *param = esp_rmaker_device_get_param_by_name(device, (const char *)param_name);
    if (!param) {
        //TODO: error code, param not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_val_type_t type = esp_rmaker_param_get_data_type(param);
    if (type == RMAKER_VAL_TYPE_INVALID) {
        //TODO: error code, param type invalid
        return ESP_AT_RESULT_CODE_ERROR;
    } else if (type != RMAKER_VAL_TYPE_STRING) {
        //TODO: error code, param type error
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // Valid parameters do not include unique_name and param_name, so are not counted here
    int str_count = para_num - 2;
    char **_list;
    _list = (char **)calloc(1, str_count * sizeof(char *));

    for (int i = 0; i < str_count; i++) {
        _list[i] = strdup(str_list[i]);
    }
    esp_rmaker_param_add_valid_str_list(param, (const char **)_list, str_count);

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmparamcount(uint8_t para_num)
{
    int cnt = 0;
    uint8_t *unique_name = NULL;
    uint8_t *param_name = NULL;
    int32_t array_count = 0;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, already connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // unique_name
    if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_name
    if (esp_at_get_para_as_str(cnt++, &param_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // array_count
    if (esp_at_get_para_as_digit(cnt++, &array_count) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
    if (!device) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_param_t *param = esp_rmaker_device_get_param_by_name(device, (const char *)param_name);
    if (!param) {
        //TODO: error code, param not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_val_type_t type = esp_rmaker_param_get_data_type(param);
    if (type == RMAKER_VAL_TYPE_INVALID) {
        //TODO: error code, param type invalid
        return ESP_AT_RESULT_CODE_ERROR;
    } else if (type != RMAKER_VAL_TYPE_ARRAY) {
        //TODO: error code, param type error
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_param_add_array_max_count(param, array_count);

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmusermapping(uint8_t para_num)
{
    int cnt = 0;
    uint8_t *user_id = NULL;
    uint8_t *key = NULL;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, not connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // user_id
    if (esp_at_get_para_as_str(cnt++, &user_id) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // key
    if (esp_at_get_para_as_str(cnt++, &key) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    if (esp_rmaker_start_user_node_mapping((char *)user_id, (char *)key) == ESP_OK) {
        return ESP_AT_RESULT_CODE_OK;
    } else {
        //TODO: error code, unknown error
        return ESP_AT_RESULT_CODE_ERROR;
    }
}

static uint8_t at_exe_cmd_rmuserunmapping(uint8_t *cmd_name)
{
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, not connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_rmaker_reset_user_node_mapping() == ESP_OK) {
        return ESP_AT_RESULT_CODE_OK;
    } else {
        //TODO: error code, unknown error
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmprov(uint8_t para_num)
{
    int32_t cnt = 0;
    int32_t mode = AT_START_RM_PROV;
    int32_t customer_id = 0;
    int32_t device_extra_code = 0;
    uint8_t *broadcase_name = NULL;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // mode
    if (esp_at_get_para_as_digit(cnt++, &mode) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((mode != AT_START_RM_PROV) && (mode != AT_STOP_RM_PROV)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // customer_id
    if (cnt != para_num) {
        if (esp_at_get_para_as_digit(cnt++, &customer_id) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (customer_id > UINT16_MAX) {
            //TODO: error code, customer_id len error
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    // device_extra code
    if (cnt != para_num) {
        if (esp_at_get_para_as_digit(cnt++, &device_extra_code) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (device_extra_code > UINT8_MAX) {
            //TODO: error code, device len error
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    // broadcase_name
    if (cnt != para_num) {
        if (esp_at_get_para_as_str(cnt++, &broadcase_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if (strlen((const char *)broadcase_name) > BLE_NAME_LEN_MAX) {
            //TODO: error code, ble name len error
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    if ((xEventGroupGetBits(s_rm_event_group) & RM_PROVISIONING_STATUS) && mode == AT_START_RM_PROV) {
        //TODO: error code, prov ing
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (mode == AT_START_RM_PROV) {
        uint8_t mfg[12] = {0};
        if (customer_id != 0) {
            s_profile.customer_id = customer_id;
        }
        if (device_extra_code != 0) {
            s_profile.device_extra_code = device_extra_code;
        }
        if (broadcase_name) {
            memset(s_profile.ble_name, 0, BLE_NAME_LEN_MAX);
            memcpy(s_profile.ble_name, broadcase_name, strlen((const char *)broadcase_name));
        }
        rmaker_generate_mfg_info(mfg, sizeof(mfg), s_profile.customer_id, s_profile.device_type_code, s_profile.device_extra_code);
        ESP_LOG_BUFFER_HEXDUMP(TAG, mfg, sizeof(mfg), ESP_LOG_INFO);
        //Start the rmaker framework
        esp_rmaker_mqtt_disconnect();
        esp_wifi_restore();

        if (!(xEventGroupGetBits(s_rm_event_group) & RM_CORE_STARTED_EVENT)) {
            esp_rmaker_start();
        } else {
            esp_rmaker_work_queue_stop();
            esp_rmaker_work_queue_deinit();
            esp_rmaker_work_queue_init();
            rmaker_prov_stop();
            esp_rmaker_stop();
            esp_rmaker_start();
        }
        xEventGroupSetBits(s_rm_event_group, RM_CORE_STARTED_EVENT);
        rmaker_prov_start(mfg, 12, 0, s_profile.ble_name);
    } else {
        rmaker_prov_stop();
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t get_rmaker_mqtt_cnt_result(const int32_t timeout_s)
{
    EventBits_t bits = xEventGroupWaitBits(s_rm_event_group,
                                        RM_MQTT_CONNECTED_EVENT,
                                        pdFALSE,
                                        pdTRUE,
                                        (timeout_s * 1000) / portTICK_PERIOD_MS);
    if (bits & RM_MQTT_CONNECTED_EVENT) {
        return ESP_AT_RESULT_CODE_OK;
    } else {
        esp_rmaker_mqtt_disconnect();

        // No disconnect event will be reported if the device actively disconnects the MQTT connection
        // So here to clear the RM_MQTT_CONNECTED_EVENT bit
        xEventGroupClearBits(s_rm_event_group, RM_MQTT_CONNECTED_EVENT);

        return ESP_AT_RESULT_CODE_ERROR;
    }
}

static uint8_t at_setup_cmd_rmconn(uint8_t para_num)
{
    int cnt = 0;
    int32_t conn_timeout_s = 0;

    // conn_timeout_s
    if (esp_at_get_para_as_digit(cnt++, &conn_timeout_s) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((conn_timeout_s < RAINMAKER_CONN_TIMEOUT_S_MIN) || (conn_timeout_s > RAINMAKER_CONN_TIMEOUT_S_MAX)) {
        ESP_LOGE(TAG, "conn_timeout_s range: [%d, %d]", RAINMAKER_CONN_TIMEOUT_S_MIN, RAINMAKER_CONN_TIMEOUT_S_MAX);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    // node is not initialized
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // wifi not connected
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_WIFI_GOT_IP_STATUS)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // ESP RainMaker agent thread is not running
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_CORE_STARTED_EVENT)) {
        esp_rmaker_start();
        xEventGroupSetBits(s_rm_event_group, RM_CORE_STARTED_EVENT);

        return get_rmaker_mqtt_cnt_result(conn_timeout_s);
    } else {
        // ESP RainMaker agent thread is running. Here needs to start an MQTT connection
        esp_rmaker_mqtt_connect();

        return get_rmaker_mqtt_cnt_result(conn_timeout_s);
    }
}

static uint8_t at_exe_cmd_rmconn(uint8_t *cmd_name)
{
    // node is not initialized
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // wifi not connected
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_WIFI_GOT_IP_STATUS)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // ESP RainMaker agent thread is not running
    if (!(xEventGroupGetBits(s_rm_event_group) & RM_CORE_STARTED_EVENT)) {
        esp_rmaker_start();
        xEventGroupSetBits(s_rm_event_group, RM_CORE_STARTED_EVENT);

        return get_rmaker_mqtt_cnt_result(RAINMAKER_CONN_TIMEOUT_S_DEF);
    } else {
        // ESP RainMaker agent thread is running. Here needs to start an MQTT connection
        esp_rmaker_mqtt_connect();

        return get_rmaker_mqtt_cnt_result(RAINMAKER_CONN_TIMEOUT_S_DEF);
    }
}

static uint8_t at_setup_cmd_rmclose(uint8_t *cmd_name)
{
    if ((xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        esp_rmaker_mqtt_disconnect();

        rm_connect_status = RM_CONNECT_STATUS_OFF;

        // No disconnect event will be reported if the device actively disconnects the MQTT connection
        // So here to clear the RM_MQTT_CONNECTED_EVENT bit
        xEventGroupClearBits(s_rm_event_group, RM_MQTT_CONNECTED_EVENT);
    }

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmparamupdate(uint8_t para_num)
{
    int cnt = 0;
    uint8_t *unique_name = NULL;
    at_rm_kv_data_t kv_group[RAINMAKER_PARAMS_KV_GROUP_MAX];
    memset(&kv_group[0], 0, sizeof(at_rm_kv_data_t) * RAINMAKER_PARAMS_KV_GROUP_MAX);

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, not connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // unique_name
    if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_name_1
    if (esp_at_get_para_as_str(cnt++, (uint8_t **)(&kv_group[0].key)) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_value_1
    if (esp_at_get_para_as_str(cnt++, (uint8_t **)(&kv_group[0].value)) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        // Supports 4 groups of 2 parameters each, So the for loop handles up to 8 parameters here.
        // `-2` means 2 parameters have been processed before
        // `/2` means process 2 parameters at once
        for (int i = 0; i < ((RAINMAKER_PARAMS_MAX_SETS - 2) / 2); i++) {
            // next param
            if (esp_at_get_para_as_str(cnt++, (uint8_t **)(&kv_group[i + 1].key)) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            // next value
            if (esp_at_get_para_as_str(cnt++, (uint8_t **)(&kv_group[i + 1].value)) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            if (cnt == para_num) {
                break;
            }
        }
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    // Valid parameters do not include unique_name, so are not counted here
    int para_group_count = (para_num - 1) / 2;
    esp_rmaker_param_val_t val;
    esp_rmaker_param_t *param = NULL;
    esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
    if (!device) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }

    //  Query param validity through rmaker API, only continue if all are valid
    for (int i = 0; i < para_group_count; i++) {
        param = NULL;
        param = esp_rmaker_device_get_param_by_name(device, (const char *)kv_group[i].key);
        if (!param) {
            //TODO: error code, param not found
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    // Write
    for (int i = 0; i < para_group_count; i++) {
        memset(&val, 0, sizeof(esp_rmaker_param_val_t));
        param = NULL;
        param = esp_rmaker_device_get_param_by_name(device, (const char *)kv_group[i].key);
        val.type = esp_rmaker_param_get_data_type(param);
        rmaker_structure_data(val.type, kv_group[i].value, &val);
        esp_rmaker_param_update(param, val);
    }
    esp_rmaker_param_update_and_report(param, val);

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmmode(uint8_t para_num)
{
    int cnt = 0;
    int32_t mode = AT_RM_NORMAL;
    uint8_t *buf = NULL;
    int32_t had_received_len = 0;

    // mode
    if (esp_at_get_para_as_digit(cnt++, &mode) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((mode != AT_RM_NORMAL) && (mode != AT_RM_PASSTHROUGH)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    /* Passthrough only supports one parameter in the device */
    if ((mode == AT_RM_PASSTHROUGH) && (s_rm_param_nums != RAINMAKER_PASSTHROUGH_MAX_PARAM_NUMS)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    s_mode = mode;

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_setup_cmd_rmsend(uint8_t para_num)
{
#define TEMP_BUFFER_SIZE    32
    uint8_t buffer[TEMP_BUFFER_SIZE] = {0};
    int cnt = 0;
    uint8_t *unique_name = NULL;
    uint8_t *param_name = NULL;
    int32_t len = 0;
    uint8_t *buf = NULL;
    int32_t had_received_len = 0;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, not connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // unique_name
    if (esp_at_get_para_as_str(cnt++, &unique_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // param_name
    if (esp_at_get_para_as_str(cnt++, &param_name) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // len
    if (esp_at_get_para_as_digit(cnt++, &len) == ESP_AT_PARA_PARSE_RESULT_FAIL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // parameters are ready
    CHECK_PARAMS_NUM(cnt, para_num);

    esp_rmaker_device_t *device = esp_rmaker_node_get_device_by_name(esp_rmaker_get_node(), (const char *)unique_name);
    if (!device) {
        //TODO: error code, device not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_param_t *param = esp_rmaker_device_get_param_by_name(device, (const char *)param_name);
    if (!param) {
        //TODO: error code, param not found
        return ESP_AT_RESULT_CODE_ERROR;
    }
    esp_rmaker_val_type_t type = esp_rmaker_param_get_data_type(param);

    // data processing
    buf = (uint8_t *)calloc(1, len);
    if (buf == NULL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!s_at_rainmaker_sync_sema) {
        s_at_rainmaker_sync_sema = xSemaphoreCreateBinary();
        if (!s_at_rainmaker_sync_sema) {
            free(buf);
            buf = NULL;
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    esp_at_port_enter_specific(at_rainmaker_wait_data_cb);

    // output "OK" and ">"
    esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);

    // receive at cmd port data
    while (xSemaphoreTake(s_at_rainmaker_sync_sema, portMAX_DELAY)) {
        had_received_len += esp_at_port_read_data(buf + had_received_len, len - had_received_len);
        if (had_received_len == len) {
            snprintf((char *)buffer, TEMP_BUFFER_SIZE, "\r\nRecv %d bytes\r\n", len);
            esp_at_port_write_data(buffer, strlen((char *)buffer));

            esp_at_port_exit_specific();

            /* Report to the cloud */
            esp_rmaker_param_val_t val;
            memset(&val, 0, sizeof(esp_rmaker_param_val_t));
            val.type = esp_rmaker_param_get_data_type(param);
            rmaker_structure_data(val.type, buf, &val);
            esp_rmaker_param_update_and_report(param, val);

            had_received_len = esp_at_port_get_data_length();
            if (had_received_len > 0) {
                esp_at_port_recv_data_notify(had_received_len, portMAX_DELAY);
            }

            break;
        }
    }

    vSemaphoreDelete(s_at_rainmaker_sync_sema);
    s_at_rainmaker_sync_sema = NULL;

    free(buf);
    buf = NULL;

    return ESP_AT_RESULT_CODE_SEND_OK;
}

static uint8_t at_exe_cmd_rmsend(uint8_t *cmd_name)
{
    int cnt = 0;
    uint8_t *buf;
    int32_t had_received_len = 0;

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_NODE_INIT_DONE_EVENT)) {
        //TODO: error code, not init
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (!(xEventGroupGetBits(s_rm_event_group) & RM_MQTT_CONNECTED_EVENT)) {
        //TODO: error code, not connect
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (s_mode != AT_RM_PASSTHROUGH) {
        //TODO: error code, not allowed
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // data processing
    buf = (uint8_t *)calloc(1, RAINMAKER_PASSTHROUGH_BUFFER_LEN);
    if (buf == NULL) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (!s_at_rainmaker_sync_sema) {
        s_at_rainmaker_sync_sema = xSemaphoreCreateBinary();
        if (!s_at_rainmaker_sync_sema) {
            free(buf);
            buf = NULL;
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }
    esp_at_port_enter_specific(at_rainmaker_wait_data_cb);
    // output "OK" and ">"
    esp_at_response_result(ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT);
    xEventGroupSetBits(s_rm_event_group, RM_IN_PASSTHROUGH_MODE);
    // receive at cmd port data
    while (xSemaphoreTake(s_at_rainmaker_sync_sema, portMAX_DELAY)) {
        memset(buf, 0, RAINMAKER_PASSTHROUGH_BUFFER_LEN);
        had_received_len = esp_at_port_read_data(buf, RAINMAKER_PASSTHROUGH_BUFFER_LEN);
        // Check whether to exit the passthrough mode
        // the exit condition is the "+++" string received
        if ((had_received_len == 3) && (strncmp((const char *)buf, "+++", 3) == 0)) {
            esp_at_port_exit_specific();
            xEventGroupClearBits(s_rm_event_group, RM_IN_PASSTHROUGH_MODE);
            break;
        } else {
            /* Report to the cloud */
            esp_rmaker_param_val_t val;
            memset(&val, 0, sizeof(esp_rmaker_param_val_t));
            val.type = esp_rmaker_param_get_data_type(s_profile.passthrough_param_handle);
            rmaker_structure_data(val.type, buf, &val);
            esp_rmaker_param_update_and_report(s_profile.passthrough_param_handle, val);
        }
    }
    vSemaphoreDelete(s_at_rainmaker_sync_sema);
    s_at_rainmaker_sync_sema = NULL;
    free(buf);
    buf = NULL;
    return ESP_AT_RESULT_CODE_PROCESS_DONE;
}

static const esp_at_cmd_struct s_at_rainmaker_cmd[] = {
    {"+RMNODEINIT", NULL, NULL, NULL, at_exe_cmd_rmnodeinit},
    {"+RMNODEATTR", NULL, NULL, at_setup_cmd_rmnodeattr, NULL},
    {"+RMDEV", NULL, NULL, at_setup_cmd_rmdev, NULL},
    {"+RMPARAM", NULL, NULL, at_setup_cmd_rmparam, NULL},
    {"+RMPARAMBOUNDS", NULL, NULL, at_setup_cmd_rmparambounds, NULL},
    {"+RMPARAMSTRLIST", NULL, NULL, at_setup_cmd_rmparamstrlist, NULL},
    {"+RMPARAMCOUNT", NULL, NULL, at_setup_cmd_rmparamcount, NULL},
    {"+RMUSERMAPPING", NULL, NULL, at_setup_cmd_rmusermapping, NULL},
    {"+RMUSERUNMAPPING", NULL, NULL, NULL, at_exe_cmd_rmuserunmapping},
    {"+RMPROV", NULL, NULL, at_setup_cmd_rmprov, NULL},
    {"+RMCONN", NULL, NULL, at_setup_cmd_rmconn, at_exe_cmd_rmconn},
    {"+RMCLOSE", NULL, NULL, NULL, at_setup_cmd_rmclose},
    {"+RMPARAMUPDATE", NULL, NULL, at_setup_cmd_rmparamupdate, NULL},
    {"+RMMODE", NULL, NULL, at_setup_cmd_rmmode, NULL},
    {"+RMSEND", NULL, NULL, at_setup_cmd_rmsend, at_exe_cmd_rmsend},
};

esp_err_t esp_rmaker_factory_init(void)
{
    static bool esp_rmaker_storage_init_done;
    if (esp_rmaker_storage_init_done) {
        ESP_LOGW(TAG, "ESP RainMaker Storage already initialized");
        return ESP_OK;
    }

    const esp_partition_t *partition = esp_at_custom_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, RAINMAKER_PRE_CFG_PARTITION);
    esp_err_t err = nvs_flash_init_partition_ptr(partition);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS Flash init failed");
    } else {
        esp_rmaker_storage_init_done = true;
    }
    return err;
}

void *esp_rmaker_factory_get(const char *key)
{
    nvs_handle handle;
    esp_err_t err;

    if ((err = nvs_open(RAINMAKER_PRE_NAMESPACE, NVS_READONLY, &handle)) == ESP_OK) {
        ESP_LOGD(TAG, "NVS open for %s %s success", RAINMAKER_PRE_NAMESPACE, key);
    } else {
        if ((err = nvs_open_from_partition(RAINMAKER_PRE_CFG_PARTITION, RAINMAKER_PRE_NAMESPACE,
                                           NVS_READONLY, &handle)) != ESP_OK) {
            ESP_LOGD(TAG, "NVS open for %s %s %s failed with error %d", RAINMAKER_PRE_CFG_PARTITION, RAINMAKER_PRE_NAMESPACE, key, err);
            return NULL;
        }
    }

    size_t required_size = 0;
    if ((err = nvs_get_blob(handle, key, NULL, &required_size)) != ESP_OK) {
        ESP_LOGD(TAG, "Failed to read key %s with error %d size %d", key, err, required_size);
        nvs_close(handle);
        return NULL;
    }
    void *value = calloc(required_size + 1, 1); /* + 1 for NULL termination */
    if (value) {
        nvs_get_blob(handle, key, value, &required_size);
    }
    nvs_close(handle);
    return value;
}

esp_err_t esp_rmaker_factory_set(const char *key, void *value, size_t len)
{
    nvs_handle handle;
    esp_err_t err;
    if ((err = nvs_open(RAINMAKER_PRE_NAMESPACE, NVS_READWRITE, &handle)) != ESP_OK) {
        ESP_LOGE(TAG, "NVS open failed with error %d", err);
        return ESP_FAIL;
    }
    if ((err = nvs_set_blob(handle, key, value, len)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write key %s with error %d size %d", key, err, len);
        nvs_close(handle);
        return ESP_FAIL;
    }
    nvs_commit(handle);
    nvs_close(handle);
    return ESP_OK;
}

esp_err_t at_rmaker_wifi_event_register(void)
{
    if (!s_rm_event_group) {
        s_rm_event_group = xEventGroupCreate();
        if (!s_rm_event_group) {
            ESP_LOGE(TAG, "Couldn't create rm event group.");
            return ESP_FAIL;
        }
    }
    esp_rmaker_factory_init();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);

    return ESP_OK;
}

bool esp_at_rainmaker_cmd_regist(void)
{
    if (at_rmaker_wifi_event_register() == ESP_OK) {
        return esp_at_custom_cmd_array_regist(s_at_rainmaker_cmd, sizeof(s_at_rainmaker_cmd) / sizeof(s_at_rainmaker_cmd[0]));
    } else {
        return false;
    }
}
