/* Bluetooth provisioning task/functionality
   August 2017
*/

// System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Component
#include <jansson.h>

// ESP-IDF
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "bt.h"
#include "bta_api.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_main.h"
#include "sdkconfig.h"
#include "device_nv.h"
#include "system_util.h"
#include "authentication_util.h"
//# -- AT Related
#include "at_port.h"
#include "at.h"

// Device
#include "mem.h"

// TEST
#include "at_wifiCmd.h"

#define TAG "bt_provisioning.c"

static bool parse_credentials( char* json_data, uint16_t length );
static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_c_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

#define PROVISIONING_TIMEOUT_MINUTES 0x0F

#define GATTS_SERVICE_UUID_TEST_A   0x00FF
#define GATTS_CHAR_UUID_TEST_A      0xFF01
#define GATTS_DESCR_UUID_TEST_A     0x3333
#define GATTS_NUM_HANDLE_TEST_A     4

#define GATTS_SERVICE_UUID_TEST_B   0x00EE
#define GATTS_CHAR_UUID_TEST_B      0xEE01
#define GATTS_DESCR_UUID_TEST_B     0x2222
#define GATTS_NUM_HANDLE_TEST_B     4

#define GATTS_SERVICE_UUID_TEST_C   0x00DD
#define GATTS_CHAR_UUID_TEST_C      0xDD01
#define GATTS_DESCR_UUID_TEST_C     0x1111
#define GATTS_NUM_HANDLE_TEST_C     4

#define MAX_CREDENTIALS     256
#define MAX_SSID_LENGTH     32
#define MAX_PASS_LENGTH     64

static char device_name[32] = "SimpliSafe 00000000";
static char wifi_credentials[MAX_CREDENTIALS] = {0};

#define TEST_MANUFACTURER_DATA_LEN  17

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40
#define PREPARE_BUF_MAX_SIZE 1024

#define MAX_STATUS_LENGTH   64

uint32_t ap_list_idx = MAX_AP_LIST_LENGTH;

extern bool at_wifi_auto_reconnect_flag;
extern char ap_list_out[MAX_AP_LIST_LENGTH];

char connection_status[MAX_STATUS_LENGTH] = "Ready for credentials..."; // {0};

uint8_t char1_str[] = {0x11,0x22,0x33};
esp_attr_value_t gatts_demo_char1_val = 
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len     = sizeof(char1_str),
    .attr_value   = char1_str,
};

#ifdef CONFIG_SET_RAW_ADV_DATA
static uint8_t raw_adv_data[] = {
        0x02, 0x01, 0x06, 0x0f, 0x09, 
        0x53, 0x49, 0x4d, 0x50, 0x4c, 0x49, 0x53, 0x41, 0x46, 0x45, /* "SIMPLISAFE" */
        0x02, 0x0a, 0xeb, 0x03, 0x03, 0xab, 0xcd
};
static uint8_t raw_scan_rsp_data[] = {
        0x02, 0x01, 0x06, 0x0f, 0x09, 
        0x53, 0x49, 0x4d, 0x50, 0x4c, 0x49, 0x53, 0x41, 0x46, 0x45, /* "SIMPLISAFE" */
        0x02, 0x0a, 0xeb, 0x03, 0x03, 0xab, 0xcd
};
#else
static uint8_t test_service_uuid128[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xAB, 0xCD, 0x00, 0x00,
    //second uuid, 32bit, [12], [13], [14], [15] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xAB, 0xCD, 0xAB, 0xCD,
};

//static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};
static esp_ble_adv_data_t test_adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x20,
    .max_interval = 0x40,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 32,
    .p_service_uuid = test_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
#endif /* CONFIG_SET_RAW_ADV_DATA */

static esp_ble_adv_params_t test_adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr        =
    //.peer_addr_type   =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

#define PROFILE_NUM 3
#define PROFILE_A_APP_ID 0
#define PROFILE_B_APP_ID 1
#define PROFILE_C_APP_ID 2

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
    [PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
    [PROFILE_B_APP_ID] = {
        .gatts_cb = gatts_profile_b_event_handler,                   /* This demo does not implement, similar as profile A */
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
    [PROFILE_C_APP_ID] = {
        .gatts_cb = gatts_profile_c_event_handler,                   /* This demo does not implement, similar as profile A */
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

typedef struct {
    uint8_t                 *prepare_buf;
    int                     prepare_len;
} prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;
static prepare_type_env_t b_prepare_write_env;
static prepare_type_env_t c_prepare_write_env;

static void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);
static void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

static uint16_t parse_message(uint8_t* incoming, uint32_t length);

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        esp_ble_gap_start_advertising(&test_adv_params);
        break;
    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
        esp_ble_gap_start_advertising(&test_adv_params);
        break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
        esp_ble_gap_start_advertising(&test_adv_params);
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        //advertising start complete event to indicate advertising start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Advertising start failed\n");
        }
        break;
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Advertising stop failed\n");
        }
        else {
            ESP_LOGI(TAG, "Stop adv successfully\n");
        }
        break;
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
         ESP_LOGI(TAG, "update connetion params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                  param->update_conn_params.status,
                  param->update_conn_params.min_int,
                  param->update_conn_params.max_int,
                  param->update_conn_params.conn_int,
                  param->update_conn_params.latency,
                  param->update_conn_params.timeout);
        break;
    default:
        break;
    }
}

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    esp_gatt_status_t status = ESP_GATT_OK;
    if (param->write.need_rsp){
        if (param->write.is_prep){
            if (prepare_write_env->prepare_buf == NULL) {
                prepare_write_env->prepare_buf = (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE*sizeof(uint8_t));
                prepare_write_env->prepare_len = 0;
                if (prepare_write_env->prepare_buf == NULL) {
                    LOG_ERROR("Gatt_server prep no mem\n");
                    status = ESP_GATT_NO_RESOURCES;
                }
            } else {
                if(param->write.offset > PREPARE_BUF_MAX_SIZE) {
                    status = ESP_GATT_INVALID_OFFSET;
                } else if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE) {
                    status = ESP_GATT_INVALID_ATTR_LEN;
                }
            }

            esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
            gatt_rsp->attr_value.len = param->write.len;
            gatt_rsp->attr_value.handle = param->write.handle;
            gatt_rsp->attr_value.offset = param->write.offset;
            gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
            memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
            esp_err_t response_err = esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp);
            if (response_err != ESP_OK){
               LOG_ERROR("Send response error\n");
            }
            free(gatt_rsp);
            if (status != ESP_GATT_OK){
                return;
            }
            memcpy(prepare_write_env->prepare_buf + param->write.offset,
                   param->write.value,
                   param->write.len);
            prepare_write_env->prepare_len += param->write.len;

        }else{
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
        }
    }
}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC){
        esp_log_buffer_hex(TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    }else{
        ESP_LOGI(TAG,"ESP_GATT_PREP_WRITE_CANCEL");
    }
    if (prepare_write_env->prepare_buf) {
        free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
        gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
        gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_A;

        esp_ble_gap_set_device_name(device_name);
#ifdef CONFIG_SET_RAW_ADV_DATA
        esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
        esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
#else
        esp_ble_gap_config_adv_data(&test_adv_data);
#endif
        esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
        break;
    case ESP_GATTS_READ_EVT: {
        ESP_LOGI(TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
        rsp.attr_value.len = strnlen(connection_status, MAX_STATUS_LENGTH-1);
        strncpy((char*)rsp.attr_value.value, connection_status, rsp.attr_value.len);
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
        break;
    }
    case ESP_GATTS_WRITE_EVT: {
        ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
        ESP_LOGI(TAG, "GATT_WRITE_EVT, value len %d, value %s\n", param->write.len, param->write.value);
#define READ_COMPLETE_CREDENTIALSx
#ifdef READ_COMPLETE_CREDENTIALS
        char* data = NULL;
        data = malloc(param->write.len + 1);
        strncpy( (char*)data, (char*)param->write.value, (size_t)param->write.len );
        parse_credentials( data, param->write.len );
        free(data);
#else // NOT READ_COMPLETE_CREDENTIALS
        #warning "TODO: Consider HDLC framing, check with app team."
        uint16_t length = parse_message(param->write.value, param->write.len);
        if( length > 0 )
        {
            parse_credentials( wifi_credentials, length );
        }
#endif // READ_COMPLETE_CREDENTIALS

#define RESPOND_IMMEDIATELY
#ifdef RESPOND_IMMEDIATELY
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;

        rsp.attr_value.len = strnlen(connection_status, MAX_STATUS_LENGTH-1);
        strncpy((char*)rsp.attr_value.value, connection_status, rsp.attr_value.len);
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, &rsp);
#else
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
#endif // RESPOND_IMMEDIATELY
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(TAG,"ESP_GATTS_EXEC_WRITE_EVT");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        example_exec_write_event_env(&a_prepare_write_env, param);
        break;
    case ESP_GATTS_MTU_EVT:
    case ESP_GATTS_CONF_EVT:
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
        ESP_LOGI(TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        gl_profile_tab[PROFILE_A_APP_ID].service_handle = param->create.service_handle;
        gl_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;

        esp_ble_gatts_start_service(gl_profile_tab[PROFILE_A_APP_ID].service_handle);

        esp_ble_gatts_add_char(gl_profile_tab[PROFILE_A_APP_ID].service_handle, &gl_profile_tab[PROFILE_A_APP_ID].char_uuid,
                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                               ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY, 
                               &gatts_demo_char1_val, NULL);
        break;
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT: {
        uint16_t length = 0;
        const uint8_t *prf_char;

        ESP_LOGI(TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        gl_profile_tab[PROFILE_A_APP_ID].char_handle = param->add_char.attr_handle;
        gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_ble_gatts_get_attr_value(param->add_char.attr_handle,  &length, &prf_char);

        ESP_LOGI(TAG, "the gatts demo char length = %x\n", length);
        for(int i = 0; i < length; i++){
            ESP_LOGI(TAG, "prf_char[%x] =%x\n",i,prf_char[i]);
        }
        esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_A_APP_ID].service_handle, &gl_profile_tab[PROFILE_A_APP_ID].descr_uuid,
                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
        break;
    }
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        ESP_LOGI(TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT: {
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x50;    // max_int = 0x50*1.25ms = 100ms
        conn_params.min_int = 0x30;    // min_int = 0x30*1.25ms = 60ms
        conn_params.timeout = 400;     // timeout = 400*10ms = 4000ms
        ESP_LOGI(TAG, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:, is_conn %d\n",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5],
                 param->connect.is_connected);
        gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
        //start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
}
    case ESP_GATTS_DISCONNECT_EVT:
        esp_ble_gap_start_advertising(&test_adv_params);
        break;
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

char *token = NULL;

#define COMPLETE_AP_PER_PACKETx
#ifdef COMPLETE_AP_PER_PACKET
#define MAX_PDU 100
#else
#define MAX_PDU 20
#endif // COMPLETE_AP_PER_PACKET

static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        gl_profile_tab[PROFILE_B_APP_ID].service_id.is_primary = true;
        gl_profile_tab[PROFILE_B_APP_ID].service_id.id.inst_id = 0x00;
        gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_B;

        esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_B_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_B);
        break;
    case ESP_GATTS_READ_EVT: {
        ESP_LOGI(TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
#define COMPLETE_AP_PER_PACKETx
#ifdef COMPLETE_AP_PER_PACKET
        if( token != NULL )
        {
            ESP_LOGI(TAG, "%s\n", token );
            rsp.attr_value.len = strnlen(token, MAX_PDU);
            strncpy((char*)rsp.attr_value.value, token, MAX_PDU);
            token = strtok(NULL, "\n");
        }
        else
        {
            ESP_LOGW(TAG, "AP LIST EMPTY!\n");
            rsp.attr_value.len = sizeof("AP LIST EMPTY!");
            strncpy((char*)rsp.attr_value.value, "AP LIST EMPTY!", MAX_PDU);
        }
#else
        if( ap_list_idx <= strnlen(ap_list_out, MAX_AP_LIST_LENGTH) )
        {
            strncpy((char*)rsp.attr_value.value, (char*)(ap_list_out + ap_list_idx), MAX_PDU );
            #warning "TODO - not true at end of list, but doesn't cause issues. Clean this up."
            rsp.attr_value.len = MAX_PDU;
            ap_list_idx += MAX_PDU;
        }
        else
        {
            ESP_LOGW(TAG, "AP LIST EMPTY!\n");
            rsp.attr_value.len = sizeof("AP LIST EMPTY!");
            strncpy((char*)rsp.attr_value.value, "AP LIST EMPTY!", MAX_PDU);
        }
#endif // COMPLETE_AP_PER_PACKET

        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                    ESP_GATT_OK, &rsp);
        break;
    }
    case ESP_GATTS_WRITE_EVT: {
        ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
        ESP_LOGI(TAG, "GATT_WRITE_EVT, value len %d, value %08x\n", param->write.len, *(uint32_t *)param->write.value);
        example_write_event_env(gatts_if, &b_prepare_write_env, param);

        uint8_t* test = NULL;
        at_exeCmdCwlap(test);
        ESP_LOGD(TAG, "AP LIST: %s", ap_list_out);

#ifdef COMPLETE_AP_PER_PACKET
        // get the first token
        token = strtok(ap_list_out, "\n");
#endif // COMPLETE_AP_PER_PACKET

        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(TAG,"ESP_GATTS_EXEC_WRITE_EVT");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        example_exec_write_event_env(&b_prepare_write_env, param);
        break;
    case ESP_GATTS_MTU_EVT:
    case ESP_GATTS_CONF_EVT:
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
        ESP_LOGI(TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        gl_profile_tab[PROFILE_B_APP_ID].service_handle = param->create.service_handle;
        gl_profile_tab[PROFILE_B_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_B_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_B;

        esp_ble_gatts_start_service(gl_profile_tab[PROFILE_B_APP_ID].service_handle);

        esp_ble_gatts_add_char(gl_profile_tab[PROFILE_B_APP_ID].service_handle, &gl_profile_tab[PROFILE_B_APP_ID].char_uuid,
                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                               ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
                               NULL, NULL);
        break;
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
        ESP_LOGI(TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);

        gl_profile_tab[PROFILE_B_APP_ID].char_handle = param->add_char.attr_handle;
        gl_profile_tab[PROFILE_B_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_B_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_B_APP_ID].service_handle, &gl_profile_tab[PROFILE_B_APP_ID].descr_uuid,
                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                     NULL, NULL);
        break;
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        ESP_LOGI(TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
        ESP_LOGI(TAG, "CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:, is_conn %d\n",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5],
                 param->connect.is_connected);
        gl_profile_tab[PROFILE_B_APP_ID].conn_id = param->connect.conn_id;
        break;
    case ESP_GATTS_DISCONNECT_EVT:
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

static void gatts_profile_c_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        gl_profile_tab[PROFILE_C_APP_ID].service_id.is_primary = true;
        gl_profile_tab[PROFILE_C_APP_ID].service_id.id.inst_id = 0x00;
        gl_profile_tab[PROFILE_C_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_C_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_C;

        esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_C_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_C);
        break;
    case ESP_GATTS_READ_EVT: {
        ESP_LOGI(TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
        rsp.attr_value.len = 4;
        rsp.attr_value.value[0] = 0xde;
        rsp.attr_value.value[1] = 0xed;
        rsp.attr_value.value[2] = 0xbe;
        rsp.attr_value.value[3] = 0xef;
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
        break;
    }
    case ESP_GATTS_WRITE_EVT: {
        ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
        ESP_LOGI(TAG, "GATT_WRITE_EVT, value len %d, value %08x\n", param->write.len, *(uint32_t *)param->write.value);
        example_write_event_env(gatts_if, &c_prepare_write_env, param);
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(TAG,"ESP_GATTS_EXEC_WRITE_EVT");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        example_exec_write_event_env(&c_prepare_write_env, param);
        break;
    case ESP_GATTS_MTU_EVT:
    case ESP_GATTS_CONF_EVT:
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
        ESP_LOGI(TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        gl_profile_tab[PROFILE_C_APP_ID].service_handle = param->create.service_handle;
        gl_profile_tab[PROFILE_C_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_C_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_C;

        esp_ble_gatts_start_service(gl_profile_tab[PROFILE_C_APP_ID].service_handle);

        esp_ble_gatts_add_char(gl_profile_tab[PROFILE_C_APP_ID].service_handle, &gl_profile_tab[PROFILE_C_APP_ID].char_uuid,
                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                               ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
                               NULL, NULL);
        break;
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
        ESP_LOGI(TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);

        gl_profile_tab[PROFILE_C_APP_ID].char_handle = param->add_char.attr_handle;
        gl_profile_tab[PROFILE_C_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_C_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_C_APP_ID].service_handle, &gl_profile_tab[PROFILE_C_APP_ID].descr_uuid,
                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                     NULL, NULL);
        break;
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        ESP_LOGI(TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
        ESP_LOGI(TAG, "CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:, is_conn %d\n",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5],
                 param->connect.is_connected);
        gl_profile_tab[PROFILE_C_APP_ID].conn_id = param->connect.conn_id;
        break;
    case ESP_GATTS_DISCONNECT_EVT:
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status == ESP_GATT_OK) {
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        } else {
            ESP_LOGI(TAG, "Reg app failed, app_id %04x, status %d\n",
                    param->reg.app_id, 
                    param->reg.status);
            return;
        }
    }

    /* If the gatts_if equal to profile A, call profile A cb handler,
     * so here call each profile's callback */
    do {
        int idx;
        for (idx = 0; idx < PROFILE_NUM; idx++) {
            if (gatts_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
                    gatts_if == gl_profile_tab[idx].gatts_if) {
                if (gl_profile_tab[idx].gatts_cb) {
                    gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
            }
        }
    } while (0);
}

void bt_provisioning(void *pvParameter)
{
    esp_err_t ret = ESP_OK; 

    ESP_LOGI(TAG, "Spooling up bt_provisioning.." );

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    
    if (ret) {
        ESP_LOGE(TAG, "%s initialize controller failed\n", __func__);
        return;
    }
    ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (ret) {
        ESP_LOGE(TAG, "%s enable controller failed\n", __func__);
        return;
    }
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "%s init bluetooth failed\n", __func__);
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "%s enable bluetooth failed\n", __func__);
        return;
    }

    esp_ble_gatts_register_callback(gatts_event_handler);
    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatts_app_register(PROFILE_A_APP_ID);
    esp_ble_gatts_app_register(PROFILE_B_APP_ID);
    //esp_ble_gatts_app_register(PROFILE_C_APP_ID);

    while(1)
    {
        vTaskDelay(100);
    }
}

uint8_t StartBluetooth(char* serial_number)
{
    esp_err_t ret;

    strcpy(device_name, serial_number); 
    
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);

    ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (ret) {
        ESP_LOGE(TAG, "%s enable bt controller failed\n", __func__);
        return 0;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "%s init bluedroid failed\n", __func__);
        return 0;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "%s enable bluedroid failed\n", __func__);
        return 0;
    }

    esp_ble_gatts_register_callback(gatts_event_handler);
    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatts_app_register(PROFILE_A_APP_ID);
    esp_ble_gatts_app_register(PROFILE_B_APP_ID);
    //esp_ble_gatts_app_register(PROFILE_C_APP_ID);

    checkmem("StartBluetooth() complete...");

    return 1;
}

uint8_t StopBluetooth()
{
    esp_err_t ret;

    ret = esp_bluedroid_disable();
    if (ret) {
        ESP_LOGE(TAG, "%s disable bluedroid failed\n", __func__);
        return 0;
    }   

    ret = esp_bluedroid_deinit();
    if (ret) {
        ESP_LOGE(TAG, "%s init bluedroid failed\n", __func__);
        return 0;
    }    

    ret = esp_bt_controller_disable(ESP_BT_MODE_BTDM);
    if (ret) {
        ESP_LOGE(TAG, "%s disable bt controller failed\n", __func__);
        return 0;
    }   

    esp_bt_controller_deinit();

    checkmem("StopBluetooth() complete...");

    return 1;
}

static int8_t wifi_start_connection(char* ssid, char* password)
{
    wifi_config_t config;
    wifi_mode_t mode = WIFI_MODE_NULL;
    int8_t status = -1; // error condition
    uint8_t wait_time;
    uint8_t temp[32];

    at_memset(&config,0x0, sizeof(config));
    at_memcpy(config.sta.ssid, ssid, strnlen(ssid, MAX_SSID_LENGTH));
    at_memcpy(config.sta.password, password, strnlen(password, MAX_PASS_LENGTH));

    ESP_LOGE(TAG, "SSID: %s, PASSWORD: %s", config.sta.ssid, config.sta.password);

    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return -1;
    }

    if ((WIFI_MODE_AP == mode) || (WIFI_MODE_NULL == mode))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return -1;
    }

    at_wifi_auto_reconnect_flag = false;
    esp_wifi_disconnect();

    if (esp_wifi_set_config(WIFI_IF_STA,&config) != ESP_OK) {
        return -1;
    }

    wifi_station_init_connect_status();
    at_wifi_auto_reconnect_flag = true;
    esp_wifi_connect();

    wait_time = 16;

    while(wait_time--)
    {
        vTaskDelay(1000/(1000 /xPortGetTickRateHz()));  // delay 1000 ms
        status = wifi_station_get_connect_status();
        if (STATION_GOT_IP == status)
        {
            // If we've connected, leave.
            break;
        }
    }

    at_wifi_auto_reconnect_flag = false;

    if (STATION_GOT_IP != status)
    {
        esp_wifi_disconnect();
        at_sprintf(temp,"+CWJAP:%d\r\n",status);
        at_port_print(temp);
    }

    return status;
}

// Accepts credentials of format: 
// {"ssid": "blehwireless","pass": "somethingsomethingpassword","token": "asdfjklasdfjl;k"}
static bool parse_credentials( char* json_data, uint16_t length )
{
    bool b_return = false;
    int8_t status = -1; // error condition
    json_t* root = 0;
    json_t* ssid = 0;
    json_t* password = 0;
    json_t* token = 0;
    json_error_t error;
    
    json_data[length] = '\0';

    root = json_loads(json_data, 0, &error);

    if(NULL == root)
    {
        ESP_LOGI(TAG, "Bad JSON when reading OTA params: %s", json_data);
        b_return = false;
    }
    else
    {
        if(!json_is_object(root))
        {
            ESP_LOGI(TAG, "JSON is not an object, error: %d: %s\n", error.line, error.text );
            json_decref( root );
            b_return = false;
        }
        else
        {
            ssid = json_object_get(root, "ssid");
            password = json_object_get(root, "pass");
            token = json_object_get(root, "token");

            if ( !json_is_string(ssid) || !json_is_string(password) || !json_is_string(token) )
            {
                ESP_LOGI(TAG, "Attributes are not all strings!.\n" );
                b_return = false;
            }
            else
            {
                ESP_LOGI(TAG, "SSID: %s", json_string_value(ssid));
                ESP_LOGI(TAG, "Password: %s", json_string_value(password));
                ESP_LOGI(TAG, "Token: %s", json_string_value(token));

                char* tmp = "Unknown response!";
                strncpy(connection_status, tmp, MAX_STATUS_LENGTH-1);

                esp_wifi_set_storage(WIFI_STORAGE_FLASH);
                status = wifi_start_connection((char*)json_string_value(ssid), (char*)json_string_value(password));
                esp_wifi_set_storage(WIFI_STORAGE_RAM);

                if (0 > status)
                {
                    strncpy((char*)connection_status, "STATION_UNKNOWN_ERROR", MAX_STATUS_LENGTH-1);
                }
                else if (STATION_IDLE == status)
                {
                    strncpy((char*)connection_status, "STATION_IDLE", MAX_STATUS_LENGTH-1);
                }
                else if (STATION_CONNECTING == status)
                {
                    strncpy((char*)connection_status, "STATION_CONNECTING", MAX_STATUS_LENGTH-1);
                }
                else if (STATION_WRONG_PASSWORD == status)
                {
                    strncpy((char*)connection_status, "STATION_WRONG_PASSWORD", MAX_STATUS_LENGTH-1);
                }
                else if (STATION_NO_AP_FOUND == status)
                {
                    strncpy((char*)connection_status, "STATION_NO_AP_FOUND", MAX_STATUS_LENGTH-1);
                }
                else if (STATION_CONNECT_FAIL == status)
                {
                    strncpy((char*)connection_status, "STATION_CONNECT_FAIL", MAX_STATUS_LENGTH-1);
                }
                else if (STATION_GOT_IP == status)
                {
                    strncpy((char*)connection_status, "STATION_GOT_IP", MAX_STATUS_LENGTH-1);

                    // We've connected, so save and mark credentials as good.
                    write_wifi_creds_to_nv( json_string_value(ssid), json_string_value(password));
                    write_token_to_nv( json_string_value(token) );

                    ESP_LOGW(TAG, "Credentials saved!");
                    nvs_write_string( BT_PROV_READY, "keystored", false );
                }
                else
                {
                    ESP_LOGE(TAG, "Unknown station connect return!");
                }
            }
        }
    }

    connection_status[MAX_STATUS_LENGTH-1] = '\0';
    return b_return;
}

static uint16_t parse_message(uint8_t* incoming, uint32_t length)
{
    static bool seen_start = false;
    static uint16_t idx = 0;

    uint16_t index = 0;
    uint16_t complete_msg_length = 0;

    // Guarantee a NULL
    incoming[length] = '\0';

    // Spin through chars, quit before NULL
    while( index <= length - 1 )
    {
        if( true == seen_start )
        {
            if( '\r' == incoming[index] )
            {
                ESP_LOGD(TAG, "Received full frame: %s", wifi_credentials);
                seen_start = false;
                complete_msg_length = idx;
                idx = 0;
                break;
            }
            else if( '\n' == incoming[index] )
            {
                ESP_LOGW(TAG, "Received 'LF', restarting frame.");
                idx = 0;
            }
            else
            {
                if(idx > MAX_CREDENTIALS - 1)
                {
                    ESP_LOGE(TAG, "Credential overflow. Send LF to begin new frame.");
                    seen_start = false;
                    idx = 0;
                }
                else
                {
                    ESP_LOGD(TAG, "Store incoming[%d]: '%c'", index, incoming[index]);
                    wifi_credentials[idx++] = incoming[index];
                }
            }
        }
        else
        {
            ESP_LOGD(TAG, "Throwaway incoming[%d]: '%c'", index, incoming[index]);
            if( '\n' == incoming[index] )
            {
                seen_start = true;
            }
        }

        index++;
    }

    return complete_msg_length;
}

bool check_credentials( void )
{   
    size_t  size = 20;
    char key[20] = {0};

    key[0] = '\0'; 
    key[1] = '\0';

    nvs_read_string( BT_PROV_READY, key, &size );
    ESP_LOGD(TAG, "key: %s", key);

    if( 'k' == key[0] )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void clear_credentials( void )
{
    nvs_write_string( BT_PROV_READY, "none", false );
}
