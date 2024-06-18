/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <time.h>
#include <sys/queue.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "cJSON.h"

#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "esp_ota_ops.h"
#include "esp_image_format.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "esp_mac.h"

#include "esp_at.h"

#ifdef CONFIG_AT_WEB_SERVER_SUPPORT
#include "esp_http_server.h"
// AT web can use fatfs to storge html or use embeded file to storge html.
// If use fatfs,we should enable AT FS Command support.
#ifdef CONFIG_AT_WEB_USE_FATFS
#include "esp_vfs_fat.h"
#include "diskio_wl.h"
#include "diskio_impl.h"
#endif
#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
#include "at_web_dns_server.h"
static char *s_at_web_redirect_url = NULL;
#endif

#define ESP_AT_WEB_SERVER_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                 \
    {                                                                                  \
        if (!(a))                                                                      \
        {                                                                              \
            ESP_LOGE(TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                             \
        }                                                                              \
    } while (0)

#define ESP_AT_WEB_VERSION                             "1.0"
#define ESP_AT_WEB_FILE_PATH_MAX                       (ESP_VFS_PATH_MAX + 128)
#define ESP_AT_WEB_SCRATCH_BUFSIZE                     320
#define ESP_AT_WEB_WIFI_MAX_RECONNECT_TIMEOUT          60     // 60sec
#define ESP_AT_WEB_WIFI_MIN_RECONNECT_TIMEOUT          21     // 21sec
#define ESP_AT_WEB_MOUNT_POINT                         "/www"
#define ESP_AT_WEB_TIMER_POLLING_PERIOD                500    // 500ms
#define ESP_AT_WEB_BROADCAST_TIMES_DEFAULT             20     // When connect without ssid, for multicast wifi connect result
#define ESP_AT_WEB_BROADCAST_INTERVAL_DEFAULT          500000 // 500000us
#define ESP_AT_WEB_UDP_PORT_DEFAULT                    3339   // When connect without ssid, for multicast wifi connect result
#define ESP_AT_WEB_IPV4_MAX_IP_LEN_DEFAULT             32
#define ESP_AT_WEB_RECEIVED_ACK_MESSAGE                "received"
#define ESP_AT_WEB_AP_SCAN_NUM_DEFAULT                 10
#define ESP_AT_WEB_AP_RECORD_JSON_STR_LEN              600
#define ESP_AT_WEB_WIFI_CONNECTED_BIT                  BIT0
#define ESP_AT_WEB_WIFI_FAIL_BIT                       BIT1
#define ESP_AT_WEB_SCAN_RSSI_THRESHOLD                 -50
#define ESP_AT_WEB_SCAN_LIST_SIZE                      40
#define ESP_AT_WEB_ENABLE_VIRTUAL_MAC_MATCH            1
#define ESP_AT_WEB_ENABLE_CONNECT_HIGHEST_RSSI         1
#define ESP_AT_WEB_HIGH_RSSI_CONNECT_COUNT             1
#define ESP_AT_WEB_WIFI_TRY_CONNECT_TIMEOUT            8000 // try connect timeout is 8000ms
#define ESP_AT_WEB_WIFI_SSID_LEN_DEFAULT               32
#define ESP_AT_WEB_WIFI_LAST_SCAN_TIMEOUT              10   // 10s
#define ESP_AT_WEB_ROOT_DIR_DEFAULT                    CONFIG_AT_WEB_ROOT_DIR
#define ESP_AT_WEB_REDIRECT_URL_PREFIX_LEN             24

#define ESP_AT_WEB_HEADER_AUTH_NAME                    ("Object")
#define ESP_AT_UPGRADE_PARTITION_NAME                  ("ota")

extern void at_wifi_reconnect_stop(void);
extern void at_wifi_reconnect_init(bool force);
extern esp_err_t at_wifi_connect(void);
extern esp_err_t at_wifi_disconnect(void);
extern esp_err_t at_wifi_scan_start(const wifi_scan_config_t *config, bool block);

typedef struct router_obj {
    uint8_t ssid[32];
    int8_t rssi;
    uint8_t mac[6];
    SLIST_ENTRY(router_obj) next;
} router_obj_t;

typedef struct web_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[ESP_AT_WEB_SCRATCH_BUFSIZE];
} web_server_context_t;

typedef struct {
    uint8_t ssid[33];
    uint8_t password[65];
} wifi_sta_connect_config_t;

typedef enum {
    ESP_AT_WIFI_STA_NOT_START = 0x0,
    ESP_AT_WIFI_STA_CONFIG_DONE = 0x1,
    ESP_AT_WIFI_STA_CONNECTING = 0x2,
    ESP_AT_WIFI_STA_CONNECT_FAIL = 0x3,
    ESP_AT_WIFI_STA_CONNECT_OK = 0x4,
    ESP_AT_WIFI_STA_RESULT_CHECKED = 0x5,
} esp_at_web_config_wifi_status;

typedef struct {
    esp_at_web_config_wifi_status config_status;
    char sta_ip[ESP_AT_WEB_IPV4_MAX_IP_LEN_DEFAULT];
} wifi_sta_connection_info_t;

typedef struct {
    int udp_socket;
    struct sockaddr_in broadcast_addr;
    struct sockaddr_in unicast_addr;
    char sendline[64];
    char rx_buffer[32];
} udp_broadcast_info_t;

static web_server_context_t *s_web_context = NULL;
static httpd_handle_t s_server = NULL;
static int32_t s_at_web_wifi_reconnect_timeout = ESP_AT_WEB_WIFI_MAX_RECONNECT_TIMEOUT;
static wifi_sta_connection_info_t s_wifi_sta_connection_info = {0};
static wifi_sta_connect_config_t s_wifi_sta_connect_config = {0};
static TimerHandle_t s_wifi_sta_connect_timer_handler = NULL;
static EventGroupHandle_t s_wifi_sta_connect_event_group = NULL;
static uint8_t s_mobile_phone_mac[6] = {0};
static bool s_sta_got_ip_flag = false;
static const char *s_wifi_start_connect_response = "+WEBSERVERRSP:1\r\n";
static const char *s_wifi_conncet_finish_response = "+WEBSERVERRSP:2\r\n";
static const char *s_ota_start_response = "+WEBSERVERRSP:3\r\n";
static const char *s_ota_receive_success_response = "+WEBSERVERRSP:4\r\n";
static const char *s_ota_receive_fail_response = "+WEBSERVERRSP:5\r\n";
static SLIST_HEAD(router_fail_list_head_, router_obj) s_router_fail_list = SLIST_HEAD_INITIALIZER(s_router_fail_list);
static const char *TAG = "at web";

// AT web can use fatfs to storge html or use embeded file to storge html.
// If use fatfs,we should enable AT FS Command support.
#ifdef CONFIG_AT_WEB_USE_FATFS
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE; // Handle of the wear levelling library instance
static BYTE pdrv = 0xFF;
#endif

static uint8_t at_web_get_mac_match_len(uint8_t *mac1, uint8_t *mac2, uint8_t mac_length)
{
    uint8_t match_len = 0;
    uint8_t i = 0;
    if (mac1 == NULL || mac2 == NULL) {
        return 0;
    }

    for (i = 0; i < mac_length; i++) {
        if (mac1[i] == mac2[i]) {
            match_len++;
        }
    }

    return match_len;
}

/**
 * @brief Apply ssid、password、bssid to Wi-Fi config and start connect.
 *
 * @param[in] ssid - ssid used for Wi-Fi connect config
 * @param[in] password - password used for Wi-Fi connect config
 * @param[in] bssid - bssid used for Wi-Fi connect config, can be null.
 * @param[in] connect_event - the handler of wifi connect status eventgroup, used to feedback try connect result.
 *
 * @return
 * - ESP_OK : success
 * - Others : fail
 */
static esp_err_t at_web_try_connect(uint8_t *ssid, uint8_t *password, uint8_t *bssid, EventGroupHandle_t connect_event)
{
    esp_err_t ret;
    EventBits_t bits;
    char temp_ssid[ESP_AT_WEB_WIFI_SSID_LEN_DEFAULT + 1] = {0};
    wifi_sta_config_t sta = {0};
    memcpy(sta.ssid, ssid, sizeof(sta.ssid));
    memcpy(sta.password, password, sizeof(sta.password));
    memcpy(temp_ssid, ssid, sizeof(sta.ssid));

    if (bssid != NULL) {
        sta.bssid_set = 1;
        memcpy(sta.bssid, bssid, sizeof(sta.bssid));
    }
    at_wifi_disconnect();
    // stop reconnect if reconnect is in process(if has connected, then keep the connection)
    at_wifi_reconnect_stop();

    ret = esp_wifi_set_config(ESP_IF_WIFI_STA, (wifi_config_t*) &sta);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "wifi set config fail");
        return ret;
    }
    // force connect to AP
    at_wifi_reconnect_init(true);
    // apply connect
    at_wifi_connect();

    if (connect_event != NULL) { // need to wait wifi connect result, now it's phone config wifi and ssid is null
        bits = xEventGroupWaitBits(connect_event,
                                   ESP_AT_WEB_WIFI_CONNECTED_BIT | ESP_AT_WEB_WIFI_FAIL_BIT,
                                   pdTRUE,
                                   pdFALSE,
                                   ESP_AT_WEB_WIFI_TRY_CONNECT_TIMEOUT / portTICK_PERIOD_MS); // wait until timeout

        if (bits & ESP_AT_WEB_WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG, "connected to ap SSID:%s", temp_ssid);
        } else if (bits & ESP_AT_WEB_WIFI_FAIL_BIT) {
            ESP_LOGI(TAG, "connecting to SSID:%s, reconnect timeout", temp_ssid);
            ret = ESP_ERR_TIMEOUT;
        } else if (bits == 0) { // timeout expeird
            ESP_LOGI(TAG, "try connected to ap SSID:%s timeout", temp_ssid);
            ret = ESP_FAIL;
        } else {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
            ret = ESP_ERR_INVALID_STATE;
        }
    } else { // don't need to wait wifi connect result
        printf("connect config finish\r\n");
    }
    return ret;
}

/**
 * @brief Get mobile phone's mac connecting to the ESP AP
 *
 * @note If there is more than one station connecting to the ESP AP, the function will return the last station's mac.
 *
 * @param[out] mobile_phone_mac
 *
 * @return
 *    - ESP_OK
 *    - ESP_FAIL
 */
static esp_err_t at_web_get_mobile_phone_mac(uint8_t *mobile_phone_mac)
{
    esp_err_t err;
    wifi_sta_list_t sta_list;

    err = esp_wifi_ap_get_sta_list(&sta_list);
    if ((err != ESP_OK) && (err != ESP_ERR_WIFI_MODE)) {
        ESP_LOGE(TAG, "Get sta list fail");
        return ESP_FAIL;
    } else if (err == ESP_ERR_WIFI_MODE) {
        ESP_LOGW(TAG, "WiFi mode is wrong");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Find mobile phone mac is : %02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(sta_list.sta[sta_list.num - 1].mac));
    memcpy(mobile_phone_mac, sta_list.sta[sta_list.num - 1].mac, sizeof(sta_list.sta[sta_list.num - 1].mac));
    return ESP_OK;
}

// return false means repeat try error connect
static bool check_fail_list(uint8_t *bssid)
{
    if (bssid == NULL) {
        ESP_LOGI(TAG, "ERROR bssid");
        return false;
    }

    if (!SLIST_EMPTY(&s_router_fail_list)) {
        for (router_obj_t *fail_item = SLIST_FIRST(&s_router_fail_list); fail_item != NULL; fail_item = SLIST_NEXT(fail_item, next)) {
            if (at_web_get_mac_match_len(fail_item->mac, bssid, sizeof(fail_item->mac)) == 6) {
                ESP_LOGI(TAG, "Skip ssid: %s", fail_item->ssid);
                return false;
            }
        }
    }

    return true;
}

static esp_err_t stop_scan_filter(void)
{
    if (!SLIST_EMPTY(&s_router_fail_list)) {
        for (router_obj_t *fail_item = SLIST_FIRST(&s_router_fail_list); fail_item != NULL; fail_item = SLIST_NEXT(fail_item, next)) {
            SLIST_REMOVE(&s_router_fail_list, fail_item, router_obj, next);
            free(fail_item);
        }
    }
    return ESP_OK;
}

static void insert_fail_list(router_obj_t *item)
{
    if (item == NULL) {
        return;
    }

    SLIST_INSERT_HEAD(&s_router_fail_list, item, next);
}

static void ap_record_sort_by_rssi(wifi_ap_record_t *ap_record_array, int len)
{
    int i, j;
    wifi_ap_record_t tmp_ap_record;
    int flag;
    for (i = 0; i < len - 1; i++) {
        flag = 1;
        for (j = 0; j < len - i - 1; j++) {
            if (ap_record_array[j].rssi < ap_record_array[j + 1].rssi) {
                tmp_ap_record = ap_record_array[j];
                ap_record_array[j] = ap_record_array[j + 1];
                ap_record_array[j + 1] = tmp_ap_record;
                flag = 0;
            }
        }
        if (flag == 1) {
            break;
        }
    }
}

/**
  * @brief Start one scan, and get AP list found in the scan.
  *
  * @param[in,out] number As input param, it stores max AP number ap_records can hold.
  *                As output param, it receives the actual AP number this API returns.
  * @param[out]    ap_records  wifi_ap_record_t array to hold the found APs
  *
  * @return
  *    - ESP_OK: succeed
  *    - ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
  *    - ESP_ERR_WIFI_NOT_STARTED: WiFi is not started by esp_wifi_start
  *    - ESP_ERR_INVALID_ARG: invalid argument
  *    - ESP_ERR_NO_MEM: out of memory
  */
esp_err_t at_web_wifi_scan_get_ap_records(uint16_t *number, wifi_ap_record_t *ap_records)
{
    if ((number == NULL) || (ap_records == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t ret = ESP_FAIL;
    ret = at_wifi_scan_start(NULL, true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "scan start fail");
        return ret;
    }

    ret = esp_wifi_scan_get_ap_records(number, ap_records);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "get scan fail");
        return ret;
    }

    if (*number == 0) {
        ESP_LOGW(TAG, "There is no ap here");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Total APs scanned = %u", *number);
    // sort ap_record according to rssi
    ap_record_sort_by_rssi(ap_records, *number);
    return ESP_OK;
}

static esp_err_t at_web_check_ap_info(wifi_ap_record_t *ap_info)
{
    if (ap_info == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    // check ssid
    if (strlen((const char *)ap_info->ssid) == 0) {
        ESP_LOGD(TAG, "Ignore hidden ssid");
        goto check_err;
    }
    // check encryption
    if (ap_info->authmode > 5) {
        ESP_LOGD(TAG, "error encryption %d, ssid: %s", ap_info->authmode, ap_info->ssid);
        goto check_err;
    }
    if (ap_info->authmode < 2) {
        ESP_LOGD(TAG, "Ignore unsafe password, ssid: %s", ap_info->ssid);
        goto check_err;
    }
    // check rssi
    if (ap_info->rssi < ESP_AT_WEB_SCAN_RSSI_THRESHOLD) {
        ESP_LOGD(TAG, "Ignore low rssi, ssid: %s", ap_info->ssid);
        goto check_err;
    }
    return ESP_OK;
check_err:
    return ESP_FAIL;
}

/**
 * @brief Start Wi-Fi scan and try connect
 *
 * @param[in] phone_mac - the mac of phone which post the connect data.
 * @param[in] password - web server received Wi-Fi connect password.
 * @param[in] max_connect_time - the Max connection time allowed to attempt(include scan delay and try connect time, unit: s).
 * @param[in] connect_event - the handler of wifi connect status eventgroup, used to feedback try connect result.
 *
 * @return
 *    - ESP_OK
 *    - ESP_ERR_INVALID_ARG
 *    - ESP_FAIL
 */
static esp_err_t at_web_start_scan_filter(uint8_t *phone_mac, uint8_t *password, int32_t max_connect_time, EventGroupHandle_t connect_event)
{
    esp_err_t ret = ESP_FAIL;
    router_obj_t *last = NULL;
    uint8_t try_connect_count = 0;
    // Calculate the max number of try to connect
    uint8_t max_try_connect_num = (max_connect_time * 1000) / ESP_AT_WEB_WIFI_TRY_CONNECT_TIMEOUT;
    int32_t current_available_time = max_connect_time;
    uint16_t ap_scan_number = 0;
    int32_t loop = 0;
    bool last_scan = false;
    uint64_t start = 0;
    uint64_t end = 0;
    router_obj_t *item = NULL;
    router_obj_t *head_item = NULL;
    wifi_ap_record_t *ap_info = NULL;
    uint8_t highest_rssi_connect_count = 0;
    static uint8_t s_connect_success_flag = 0;
    SLIST_HEAD(router_all_list_head_, router_obj) s_router_all_list = SLIST_HEAD_INITIALIZER(s_router_all_list);

    if ((password == NULL) || (max_connect_time <= 0)) {
        return ESP_ERR_INVALID_ARG;
    }

    ap_info = (wifi_ap_record_t*) malloc(ESP_AT_WEB_SCAN_LIST_SIZE * sizeof(wifi_ap_record_t));
    if (ap_info == NULL) {
        ESP_LOGE(TAG, "ap info malloc fail");
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGD(TAG, "max connect time is %d", max_connect_time);

    while (max_try_connect_num > 0) {
        start = clock();
        if (current_available_time <= ESP_AT_WEB_WIFI_LAST_SCAN_TIMEOUT) {
            last_scan = true;
        }
        // clear the value of the variable
        try_connect_count = 0;
        ap_scan_number = ESP_AT_WEB_SCAN_LIST_SIZE;
        memset(ap_info, 0, ESP_AT_WEB_SCAN_LIST_SIZE * sizeof(wifi_ap_record_t));

        ret = at_web_wifi_scan_get_ap_records(&ap_scan_number, ap_info);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "get scan fail");
            goto err;
        }

        for (loop = 0; loop < ap_scan_number; loop++) {
            if (at_web_check_ap_info(&ap_info[loop]) != ESP_OK) {
                continue;
            }

            item = (router_obj_t*) malloc(sizeof(router_obj_t));
            if (item == NULL) {
                ESP_LOGE(TAG, "router malloc fail");
                break;
            }
            memset(item, 0x0, sizeof(router_obj_t));
            // copy mac
            memcpy(item->mac, ap_info[loop].bssid, sizeof(item->mac));
            // copy ssid
            memcpy(item->ssid, ap_info[loop].ssid, sizeof(item->ssid));
            // cpoy rssi
            item->rssi = ap_info[loop].rssi;

            if (last == NULL) {
                SLIST_INSERT_HEAD(&s_router_all_list, item, next);
            } else {
                SLIST_INSERT_AFTER(last, item, next);
            }

            last = item;
        }

        if (SLIST_EMPTY(&s_router_all_list)) {
            ESP_LOGE(TAG, "Not find router");
            goto err;
        }

        // If have mobile phone mac, first consider connect the router which has similar mac
        if (phone_mac != NULL) {
            ESP_LOGI(TAG, "Try to macth MAC");
            for (item = SLIST_FIRST(&s_router_all_list); (item != NULL) && (try_connect_count < max_try_connect_num); item = SLIST_NEXT(item, next)) {
                // some phone(like XIAOMI10), the difference between SOFTAP and STA is two bytes
                if ((at_web_get_mac_match_len(phone_mac, item->mac, sizeof(item->mac)) >= 5)) {
                    if (check_fail_list(item->mac)) { // ignore fail connect mac
                        try_connect_count++;
                        ret = at_web_try_connect(item->ssid, password, item->mac, connect_event);
                        if (ret != ESP_OK) {
                            ESP_LOGW(TAG, "match mac connect error");
                            SLIST_REMOVE(&s_router_all_list, item, router_obj, next);
                            insert_fail_list(item);
                        } else {
                            s_connect_success_flag = 1;
                        }
                        break; // find ssid, skip search
                    }
                }
            }
            if (item == NULL) {
                ESP_LOGI(TAG, "No macth MAC");
            }
        }
#if ESP_AT_WEB_ENABLE_VIRTUAL_MAC_MATCH
        // if connect fail, try to connect the router which has virtual mac
        if (last_scan == true && s_connect_success_flag == 0 && try_connect_count < max_try_connect_num) {
            ESP_LOGI(TAG, "Try to connect router through virtual MAC");
            for (item = SLIST_FIRST(&s_router_all_list); (item != NULL) && (try_connect_count < max_try_connect_num); item = SLIST_NEXT(item, next)) {
                if ((item->mac[1]) & 0x2) { // Compare the first byte
                    ESP_LOGI(TAG, "Find a virtual MAC: %02x:%02x:%02x:%02x:%02x:%02x, ssid: %s", MAC2STR(item->mac), item->ssid);
                    if (check_fail_list(item->mac)) {
                        try_connect_count++;
                        ret = at_web_try_connect(item->ssid, password, item->mac, connect_event);
                        if (ret != ESP_OK) {
                            ESP_LOGW(TAG, "virtual mac connect error");
                            SLIST_REMOVE(&s_router_all_list, item, router_obj, next);
                            insert_fail_list(item);
                        } else {
                            s_connect_success_flag = 1;
                        }
                        break; // find ssid, skip seach
                    }
                }
            }
            if (item == NULL) {
                ESP_LOGI(TAG, "No virtual MAC");
            }
        }
#endif

#if ESP_AT_WEB_ENABLE_CONNECT_HIGHEST_RSSI
        // if connect fail, try to connect the router which has highest rssi
        if (last_scan == true && s_connect_success_flag == 0 && try_connect_count < max_try_connect_num) {
            ESP_LOGI(TAG, "Try to connect highest RSSI SSID");
            head_item = SLIST_FIRST(&s_router_all_list);
            highest_rssi_connect_count = ESP_AT_WEB_HIGH_RSSI_CONNECT_COUNT;
            while ((highest_rssi_connect_count > 0) && (try_connect_count < max_try_connect_num)) {
                if (check_fail_list(head_item->mac)) {
                    ESP_LOGI(TAG, "Try to connect highest rssi ssid %s, rssi: %d", head_item->ssid, head_item->rssi);
                    try_connect_count++;
                    ret = at_web_try_connect(head_item->ssid, password, head_item->mac, connect_event);
                    if (ret != ESP_OK) {
                        ESP_LOGW(TAG, "rssi connect error");
                        SLIST_REMOVE(&s_router_all_list, head_item, router_obj, next);
                        insert_fail_list(head_item);
                    } else {
                        s_connect_success_flag = 1;
                        break;
                    }
                }
                highest_rssi_connect_count--;
                head_item = SLIST_NEXT(head_item, next);
                if (head_item == NULL) {
                    ESP_LOGE(TAG, "List is NULL");
                    break;
                }

            }
        }
#endif
        // delete scan list
        for (item = SLIST_FIRST(&s_router_all_list); item != NULL; item = SLIST_NEXT(item, next)) {
            ESP_LOGD(TAG, "Delete SSID:%s", item->ssid);
            SLIST_REMOVE(&s_router_all_list, item, router_obj, next);
            free(item);
        }

        if (s_connect_success_flag) {
            s_connect_success_flag = 0;
            free(ap_info);
            ap_info = NULL;
            stop_scan_filter();
            ESP_LOGI(TAG, "try connect count is %d", try_connect_count);
            return ESP_OK;
        }

        last = NULL;
        end = clock();
        if (end > start) {
            ESP_LOGI(TAG, "scan and try connect use time is %d", (int32_t)((end - start) / 1000));
            current_available_time -= ((end - start) / 1000);
        } else if (end < start) {
            ESP_LOGI(TAG, "scan and try connect use time is %d", (int32_t)((end + 0xFFFFFFFFUL - start) / 1000));
            current_available_time -= ((end + 0xFFFFFFFFUL - start) / 1000);
        } else {
            ESP_LOGE(TAG, "time interval fatal error");
            break;
        }

        if ((current_available_time <= 0) || (current_available_time > max_connect_time)) {
            break;
        } else {
            max_try_connect_num = (current_available_time * 1000) / ESP_AT_WEB_WIFI_TRY_CONNECT_TIMEOUT;
            ESP_LOGI(TAG, "current avail time is %d, max_try_connect_num is %d", current_available_time, max_try_connect_num);
        }
    }
    if (ap_info != NULL) {
        free(ap_info);
        ap_info = NULL;
    }
    // delete fail connect list
    stop_scan_filter();
    ESP_LOGW(TAG, "scan filter timeout");
    return ESP_FAIL;
err:
    free(ap_info);
    ap_info = NULL;
    // delete fail connect list
    stop_scan_filter();
    ESP_LOGW(TAG, "scan filter error");
    return ESP_FAIL;
}

static int at_web_url_decode(char *src, int src_len, char *des, int des_len)
{
    int len = MIN(src_len, des_len);

    memset(des, 0, des_len);
    memcpy(des, src, len);

    return len;
}

/**
 * @brief Find a specific arg in a string of get- or post-data.
 * Line is the string of post/get-data, arg is the name of the value to find. The
 * zero-terminated result is written in buff, with at most buffLen bytes used. The
 * function returns the length of the result, or -1 if the value wasn't found. The
 * returned string will be urldecoded already.
 *
 */
static int at_web_find_arg(char *line, char *arg, char *buff, int buffLen)
{
    char *p,  *e;

    if (line == NULL) {
        return -1;
    }

    p = line;

    while (p != NULL && *p != '\n' && *p != '\r' && *p != 0) {
        if (strncmp(p, arg, strlen(arg)) == 0 && p[strlen(arg)] == '=') {
            p += strlen(arg) + 1; // move p to start of value
            e = (char*) strstr(p, "&");

            if (e == NULL) {
                e = p + strlen(p);
            }

            return at_web_url_decode(p, (e - p), buff, buffLen);
        }

        p = (char*) strstr(p, "&");

        if (p != NULL) {
            p += 1;
        }
    }

    ESP_LOGD(TAG, "Finding %s in %s: Not found :/\n", arg, line);
    return -1; // not found
}

// AT web can use fatfs to storge html or use embeded file to storge html.
// If use fatfs,we should enable AT FS Command support.
#ifdef CONFIG_AT_WEB_USE_FATFS
/* Send HTTP response with the contents of the requested file */
static esp_err_t web_common_get_handler(httpd_req_t *req)
{
    char filepath[ESP_AT_WEB_FILE_PATH_MAX];
    esp_err_t err = ESP_FAIL;
    web_server_context_t *s_web_context = (web_server_context_t*) req->user_ctx;
    strlcpy(filepath, s_web_context->base_path, sizeof(filepath));
    strlcat(filepath, "/index.html", sizeof(filepath)); // Now, we just send the index html for the common handler

    ESP_LOGW(TAG, "open file : %s", filepath);
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        ESP_LOGE(TAG, "Failed to open file : %s, errno =%d", filepath, errno);
        /*Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "text/html");

    char *chunk = s_web_context->scratch;
    ssize_t read_bytes;
    do {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, ESP_AT_WEB_SCRATCH_BUFSIZE);
        if (read_bytes == -1) {
            ESP_LOGE(TAG, "Failed to read file : %s", filepath);
        } else if (read_bytes > 0) {
            /* Send the buffer contents as HTTP response chunk */
            err = httpd_resp_send_chunk(req, chunk, read_bytes);
            if (err != ESP_OK) {
                close(fd);
                ESP_LOGE(TAG, "File sending failed!,err: %d,read_bytes: %d", err, read_bytes);
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(fd);
    ESP_LOGD(TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
#else
static esp_err_t index_html_get_handler(httpd_req_t *req)
{
    extern const char html_start[] asm("_binary_index_html_start");
    extern const char html_end[]   asm("_binary_index_html_end");
    const size_t html_size = (html_end - html_start);
    httpd_resp_set_type(req, "text/html");
    /* Add file upload form and script which on execution sends a POST request to /upload */
    httpd_resp_send_chunk(req, (const char*) html_start, html_size);
    /* Respond with an empty chunk to signal HTTP response completion */
    return httpd_resp_send_chunk(req, NULL, 0);
}

/* Send HTTP response with the contents of the requested file */
static esp_err_t web_common_get_handler(httpd_req_t *req)
{
    return index_html_get_handler(req);
    return ESP_OK;
}
#endif

/* A help function to get post request data */
static esp_err_t recv_post_data(httpd_req_t *req, char *buf)
{
    int total_len = req->content_len;
    int cur_len = 0;
    int received = 0;

    if (total_len >= ESP_AT_WEB_SCRATCH_BUFSIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_500(req);
        ESP_LOGE(TAG, "context too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_500(req);
            ESP_LOGE(TAG, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0'; // now ,the post is str format, like ssid=yuxin&pwd=TestPWD&chl=1&ecn=0&maxconn=1&ssidhidden=0
    ESP_LOGI(TAG, "Post data is : %s\n", buf);
    return ESP_OK;
}

static void at_web_response_ok(httpd_req_t *req)
{
    const char *temp_str = "{\"state\": 0}";
    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    httpd_resp_set_status(req, HTTPD_200);

    httpd_resp_send(req, temp_str, strlen(temp_str));
}

static void at_web_response_error(httpd_req_t *req, const char *status)
{
    const char *temp_str = "{\"state\": 1}";
    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    httpd_resp_set_status(req, status);

    httpd_resp_send(req, temp_str, strlen(temp_str));
}

static void at_web_update_sta_connect_config(wifi_sta_connect_config_t *connect_conf)
{
    memcpy(&s_wifi_sta_connect_config, connect_conf, sizeof(wifi_sta_connect_config_t));
}

static wifi_sta_connect_config_t *at_web_get_sta_connect_config(void)
{
    return &s_wifi_sta_connect_config;
}

static void at_web_clear_sta_connect_config(void)
{
    memset(&s_wifi_sta_connect_config, 0x0, sizeof(wifi_sta_connect_config_t));
}

static void at_web_update_sta_connection_info(wifi_sta_connection_info_t *connection_info)
{
    memcpy(&s_wifi_sta_connection_info, connection_info, sizeof(wifi_sta_connection_info_t));
}

static wifi_sta_connection_info_t *at_web_get_sta_connection_info(void)
{
    return &s_wifi_sta_connection_info;
}

static void at_web_update_sta_reconnect_timeout(int32_t timeout)
{
    s_at_web_wifi_reconnect_timeout = timeout;
}

static int32_t at_web_get_sta_reconnect_timeout(void)
{
    return s_at_web_wifi_reconnect_timeout;
}

void at_web_update_sta_got_ip_flag(bool flag)
{
    s_sta_got_ip_flag = flag;
}

static bool at_web_get_sta_got_ip_flag(void)
{
    return s_sta_got_ip_flag;
}

static void listen_sta_connect_status_timer_cb(TimerHandle_t timer)
{
    wifi_sta_connection_info_t connection_info = {0};
    int32_t reconnnect_timeout = at_web_get_sta_reconnect_timeout();
    int32_t connect_max_count = (reconnnect_timeout * 1000) / ESP_AT_WEB_TIMER_POLLING_PERIOD;
    esp_err_t ret = ESP_FAIL;
    bool sta_got_ip = false;
    static int connect_count = 1;
    ESP_LOGD(TAG, "Connect callback timer %p count = %d", timer, connect_count);
    esp_netif_ip_info_t sta_ip = {0};
    esp_netif_t *sta_if = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if (connect_count < connect_max_count) {
        sta_got_ip = at_web_get_sta_got_ip_flag(); // to check whether sta has connnected to appointed ap(like at_wifi_station_get_connect_status())
        if (sta_got_ip != true) {
            connect_count++;
            return;
        } else {
            ESP_LOGI(TAG, "Connect SSID success");
            ret = esp_netif_get_ip_info(sta_if, &sta_ip);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "get sta ip fail");
            }
            ESP_LOGI(TAG, "Congratulate, got ip:" IPSTR, IP2STR(&sta_ip.ip));
            sprintf(connection_info.sta_ip, IPSTR, IP2STR(&sta_ip.ip));
            connection_info.config_status = ESP_AT_WIFI_STA_CONNECT_OK;
            goto connect_finish;
        }
    } else {
        ESP_LOGW(TAG, "Listen connect %d times and connect fail", connect_count);
        connection_info.config_status = ESP_AT_WIFI_STA_CONNECT_FAIL;
        at_wifi_reconnect_stop();
        goto connect_finish;
    }
    return;

connect_finish:
    connect_count = 1;
    at_web_update_sta_got_ip_flag(false);
    at_web_update_sta_connection_info(&connection_info);
    xTimerStop(s_wifi_sta_connect_timer_handler, portMAX_DELAY);
    xTimerDelete(s_wifi_sta_connect_timer_handler, portMAX_DELAY);
}

static int udp_create(uint16_t port, char *bind_ip)
{
    struct sockaddr_in6 dest_addr = {0};
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in*) &dest_addr;
    int ip_protocol = 0;
    const int on = 1;
    inet_aton(bind_ip, &dest_addr_ip4->sin_addr.s_addr); // bind sta ip
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(port);
    ip_protocol = IPPROTO_IP;

    int sock = socket(AF_INET, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return -1;
    }

    int err = bind(sock, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        shutdown(sock, 0);
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) {
        ESP_LOGE(TAG, "reuse addr fail");
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) != 0) {
        ESP_LOGE(TAG, "broadcast enable fail");
        close(sock);
        return -1;
    }

    return sock;
}

static int readable_check(int fd, int sec, int usec)
{
    fd_set rset;
    struct timeval tv;

    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    tv.tv_sec = sec;
    tv.tv_usec = usec;
    /* > 0 if descriptor is readable */
    return (select(fd + 1, &rset, NULL, NULL, &tv));
}

static void listen_sta_connect_success_timer_cb(TimerHandle_t timer)
{
    bool sta_got_ip = false;
    wifi_sta_connection_info_t *current_connection_info = at_web_get_sta_connection_info();

    if (current_connection_info->config_status == ESP_AT_WIFI_STA_CONNECTING) {
        sta_got_ip = at_web_get_sta_got_ip_flag(); // to check whether sta has connnected to appointed ap(like at_wifi_station_get_connect_status())
        if (sta_got_ip != true) {
            return;
        } else {
            ESP_LOGI(TAG, "Connect SSID success");
            at_web_update_sta_got_ip_flag(false); // clear connect status flag
            xEventGroupSetBits(s_wifi_sta_connect_event_group, ESP_AT_WEB_WIFI_CONNECTED_BIT);
            return;
        }
    }
}

/**
 * @brief Apply WiFi connect info to try connect
 *
 * @param[in] udp_port: indicates the device in use, -1: brower, others: WeChat;
 *  when Wechat is in use and ssid is null, we use udp send wifi connect result.
 *
 * @return
 *  - ESP_OK: Success
 *  - Others: Fail
 */
static esp_err_t at_web_apply_wifi_connect_info(int32_t udp_port)
{
    esp_err_t ret;
    wifi_sta_connect_config_t *connect_config = at_web_get_sta_connect_config();
    int32_t reconnnect_timeout = at_web_get_sta_reconnect_timeout();
    wifi_sta_connection_info_t connection_info = {0};
    int send_count = ESP_AT_WEB_BROADCAST_TIMES_DEFAULT;
    int udp_socket = -1;
    int len = 0;
    char gateway[ESP_AT_WEB_IPV4_MAX_IP_LEN_DEFAULT] = {0};
    struct sockaddr_in broadcast_addr = {0};
    struct sockaddr_in unicast_addr = {0};
    char sendline[64] = {0};
    char rx_buffer[32] = {0};
    // Calculate the max connect time,unit: s
    int32_t connect_timeout = reconnnect_timeout - ESP_AT_WEB_BROADCAST_TIMES_DEFAULT * ESP_AT_WEB_BROADCAST_INTERVAL_DEFAULT / 1000000;

    // Clear connect status flag
    at_web_update_sta_got_ip_flag(false);
    // According to config wifi device to try connect
    // when udp_port == -1, it's web browser post data to config wifi. otherwise, Now, It's WeChat post data to config wifi.
    // when (strlen((char *)connect_config->ssid) == 0) && (udp_port != -1), it's WeChat post data, and target AP is local phone.
    if ((strlen((char *)connect_config->ssid) != 0) || (udp_port == -1)) {
        ESP_LOGI(TAG, "Use SSID %s direct connect", (char *)connect_config->ssid);
        ret = at_web_try_connect(connect_config->ssid, connect_config->password, NULL, NULL);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Apply connect fail");
            goto err;
        } else {
            ESP_LOGI(TAG, "Apply connect success");
            connection_info.config_status = ESP_AT_WIFI_STA_CONNECTING;
            at_web_update_sta_connection_info(&connection_info);
            // send a message to MCU
            esp_at_port_active_write_data((uint8_t*)s_wifi_start_connect_response, strlen(s_wifi_start_connect_response));
        }

        s_wifi_sta_connect_timer_handler = xTimerCreate("listen_sta_connect_status", ESP_AT_WEB_TIMER_POLLING_PERIOD / portTICK_PERIOD_MS, pdTRUE,
                                                        NULL, listen_sta_connect_status_timer_cb);
        xTimerStart(s_wifi_sta_connect_timer_handler, 5);
    } else {
        // if have connect to a ap, then disconnect
        at_wifi_disconnect();
        // stop reconnect if reconnect is in process(ensure ESP in disconnect status)
        at_wifi_reconnect_stop();
        s_wifi_sta_connect_event_group = xEventGroupCreate();

        s_wifi_sta_connect_timer_handler = xTimerCreate("listen_sta_connect_success", ESP_AT_WEB_TIMER_POLLING_PERIOD / portTICK_PERIOD_MS, pdTRUE,
                                                        NULL, listen_sta_connect_success_timer_cb);
        xTimerStart(s_wifi_sta_connect_timer_handler, 5);
        connection_info.config_status = ESP_AT_WIFI_STA_CONNECTING;
        at_web_update_sta_connection_info(&connection_info);
        // send a message to MCU
        esp_at_port_active_write_data((uint8_t*)s_wifi_start_connect_response, strlen(s_wifi_start_connect_response));
        // begin scan and try connect,attention, this function will block until connect successfully or reach max_scan_time or reach connect_num
        ret = at_web_start_scan_filter(s_mobile_phone_mac, connect_config->password, connect_timeout, s_wifi_sta_connect_event_group);
        // when function return, it means have finished scan and try connect.
        // clear connect resource, include Timer and EventGroup.
        if (s_wifi_sta_connect_timer_handler != NULL) {
            xTimerStop(s_wifi_sta_connect_timer_handler, portMAX_DELAY);
            xTimerDelete(s_wifi_sta_connect_timer_handler, portMAX_DELAY);
            s_wifi_sta_connect_timer_handler = NULL;
        }

        if (s_wifi_sta_connect_event_group != NULL) {
            vEventGroupDelete(s_wifi_sta_connect_event_group);
            s_wifi_sta_connect_event_group = NULL;
        }

        memset(s_mobile_phone_mac, 0, sizeof(s_mobile_phone_mac));
        at_web_clear_sta_connect_config();

        esp_netif_ip_info_t sta_ip = {0};
        esp_netif_t *sta_if = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

        // according to connect result to update or report results
        if (ret != ESP_OK) { // connect fail
            ESP_LOGW(TAG, "Scan filter fail, timeout");
            connection_info.config_status = ESP_AT_WIFI_STA_CONNECT_FAIL;
            at_wifi_reconnect_stop();
            at_web_update_sta_connection_info(&connection_info);
            esp_at_port_active_write_data((uint8_t*)s_wifi_conncet_finish_response, strlen(s_wifi_conncet_finish_response));
        } else { // connect ok
            ESP_LOGI(TAG, "Connect router success");
            ret = esp_netif_get_ip_info(sta_if, &sta_ip);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "get sta ip fail");
            }
            ESP_LOGI(TAG, "Congratulate, got ip:" IPSTR, IP2STR(&sta_ip.ip));
            ESP_LOGI(TAG, "got gw:" IPSTR, IP2STR(&sta_ip.gw)); // got gateway, Actually, it's mobile phone's ap ip.

            sprintf(connection_info.sta_ip, IPSTR, IP2STR(&sta_ip.ip));
            sprintf(gateway, IPSTR, IP2STR(&sta_ip.gw));

            sprintf(sendline, "ip=%d.%d.%d.%d&port=%d", IP2STR(&sta_ip.ip), ESP_AT_WEB_UDP_PORT_DEFAULT);
            ESP_LOGD(TAG, "udp send str is %s", sendline);

            connection_info.config_status = ESP_AT_WIFI_STA_CONNECT_OK;
            at_web_update_sta_connection_info(&connection_info);
            // creat udp to send wifi connect success to mobile phone.
            udp_socket = udp_create(ESP_AT_WEB_UDP_PORT_DEFAULT, connection_info.sta_ip);
            if (udp_socket == -1) {
                goto err;
            }

            broadcast_addr.sin_family = AF_INET;
            broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255"); // broadcast ip (*.*.*.255 can also work).
            broadcast_addr.sin_port = htons(udp_port);

            unicast_addr.sin_family = AF_INET;
            unicast_addr.sin_addr.s_addr = inet_addr(gateway); // unicast ip
            unicast_addr.sin_port = htons(udp_port);

            do {
                len = sendto(udp_socket, sendline, strlen(sendline), 0, (struct sockaddr*) &broadcast_addr, sizeof(broadcast_addr));
                if (len < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                }
                len = sendto(udp_socket, sendline, strlen(sendline), 0, (struct sockaddr*) &unicast_addr, sizeof(unicast_addr));
                if (len < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                }
                ESP_LOGI(TAG, "send ok udp");
                if (readable_check(udp_socket, 0, ESP_AT_WEB_BROADCAST_INTERVAL_DEFAULT) > 0) { // wait 500ms to check whether mobile phone send "received" ack
                    len = recvfrom(udp_socket, rx_buffer, sizeof(rx_buffer) - 1, 0, NULL, NULL);
                    if (len < 0) {
                        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                    } else { // Data received
                        rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                        ESP_LOGI(TAG, "receive %s", rx_buffer);
                        if (strcmp(rx_buffer, ESP_AT_WEB_RECEIVED_ACK_MESSAGE) == 0) {
                            ESP_LOGI(TAG, "Received expected message, finished");
                            break;
                        }
                    }
                }
                send_count--;
            } while (send_count > 0);

            if (udp_socket != -1) {
                shutdown(udp_socket, 0);
                close(udp_socket);
            }
            if (send_count == 0) { // can not received ack message from WeChat.
                ESP_LOGW(TAG, "not receive ack message from WeChat");
            } else { // received ack message from WeChat.
                esp_at_port_active_write_data((uint8_t*)s_wifi_conncet_finish_response, strlen(s_wifi_conncet_finish_response));
            }
        }
    }
    return ESP_OK;

err:
    connection_info.config_status = ESP_AT_WIFI_STA_NOT_START;
    at_web_update_sta_connection_info(&connection_info);
    return ESP_FAIL;
}

static esp_err_t at_get_wifi_info_from_json_str(char *buffer, wifi_sta_connect_config_t *config)
{
    char ssid[33] = {0}, password[65] = {0};
    int32_t ssid_len = 0, password_len = 0;
    cJSON *root = NULL, *item = NULL, *value_item = NULL;

    root = cJSON_Parse(buffer);
    if (!root) {
        ESP_LOGE(TAG, "Invalid format: [%s]", cJSON_GetErrorPtr());
        return ESP_FAIL;
    }

    int json_item_num = cJSON_GetArraySize(root);
    ESP_LOGD(TAG, "Total JSON Items:%d", json_item_num);

    item = cJSON_GetObjectItem(root, "ssid");
    if (item) {
        ssid_len = strlen(item->valuestring);
        ESP_LOGD(TAG, "ssid:%s", item->valuestring);
        if (ssid_len > 32) {
            ESP_LOGE(TAG, "ssid is too long");
            return ESP_FAIL;
        } else {
            strncpy(ssid, item->valuestring, ssid_len);
        }
    }

    item = cJSON_GetObjectItem(root, "password");
    if (item) {
        password_len = strlen(item->valuestring);
        ESP_LOGD(TAG, "password:%s", item->valuestring);
        if (password_len > 64) {
            ESP_LOGE(TAG, "password is too long");
            return ESP_FAIL;
        } else {
            strncpy(password, item->valuestring, password_len);
        }
    }
    cJSON_Delete(root);

    memcpy(config->ssid, ssid, ssid_len);
    memcpy(config->password, password, password_len);

    return ESP_OK;
}

static esp_err_t config_wifi_post_handler(httpd_req_t *req)
{
    char *buf = ((web_server_context_t*)(req->user_ctx))->scratch;
    wifi_sta_connect_config_t wifi_config = {0};
    int str_len = 0;
    int32_t udp_port = -1;
    char temp_str[32] = {0};
    bool ssid_is_null = false;
    wifi_mode_t current_wifi_mode;
    wifi_sta_connection_info_t *connection_info = at_web_get_sta_connection_info();
    memset(buf, '\0', ESP_AT_WEB_SCRATCH_BUFSIZE * sizeof(char));
    esp_wifi_get_mode(&current_wifi_mode);
    if (current_wifi_mode != WIFI_MODE_APSTA) {
        printf("Error, wifi mode is not correct\r\n");
        goto error_handle;
    }
    // only wifi config not start or have success apply one connection,allow to apply new connect
    if ((connection_info->config_status == ESP_AT_WIFI_STA_NOT_START) ||
            (connection_info->config_status == ESP_AT_WIFI_STA_CONNECT_FAIL) ||
            (connection_info->config_status == ESP_AT_WIFI_STA_CONNECT_OK)) {
        if (recv_post_data(req, buf) != ESP_OK) {
            at_web_response_error(req, HTTPD_500);
            ESP_LOGE(TAG, "recv post data error");
            goto error_handle;
        }

        if (at_get_wifi_info_from_json_str(buf, &wifi_config) != ESP_OK) {
            ESP_LOGE(TAG, "failed to parse wifi info, json str: %s", buf);
            goto error_handle;
        }
        ESP_LOGD(TAG, "ssid(%d):%s password:(%d):%s\r\n",
                 strlen((char *)wifi_config.ssid), wifi_config.ssid, strlen((char *)wifi_config.password), wifi_config.password);

        // check the validity of ssid and password
        if (strlen((char *)&wifi_config.ssid) == 0) {
            ssid_is_null = true;
        }
        if ((ssid_is_null == true) && (strlen((char *)&wifi_config.password) == 0)) {
            ESP_LOGE(TAG, "Error, ssid and password all is null");
            goto error_handle;
        }

        str_len = at_web_find_arg(buf, "udp_port", (char*)temp_str, sizeof(temp_str));
        if (str_len == -1) {
            ESP_LOGD(TAG, "udp port is null");
        } else {
            if (strlen(temp_str) == 0) {
                ESP_LOGE(TAG, "Error, udp_port is null");
                goto error_handle;
            } else {
                udp_port = atoi(temp_str); // Now, It's WeChat post data to config wifi
            }
        }

        at_web_update_sta_connect_config(&wifi_config);

        at_web_response_ok(req);
        vTaskDelay(300 / portTICK_PERIOD_MS); // to avoid wifi ap channel changed so quickly that the response can not be sent.
        // begin connect
        if (ssid_is_null != true) {
            if (at_web_apply_wifi_connect_info(udp_port) != ESP_OK) {
                ESP_LOGE(TAG, "WiFi config connect fail");
                return ESP_FAIL;
            }
        } else {
            // find mac and then start scan to try connect
            if (at_web_get_mobile_phone_mac(s_mobile_phone_mac) != ESP_OK) {
                ESP_LOGW(TAG, "Cannot find Mobile phone mac");
            }
            if (at_web_apply_wifi_connect_info(udp_port) != ESP_OK) {
                ESP_LOGE(TAG, "WiFi config connect fail");
                return ESP_FAIL;
            }
        }
        return ESP_OK;
    }
error_handle:
    at_web_response_error(req, HTTPD_400);
    return ESP_FAIL;
}

/* Simple handler for WiFi_info_get handler */
static esp_err_t config_wifi_get_handler(httpd_req_t *req)
{
    wifi_sta_connect_config_t *connect_config = at_web_get_sta_connect_config();
    wifi_sta_connection_info_t *connection_info = at_web_get_sta_connection_info();
    char temp_str[32] = {0};
    int32_t json_len = 0;
    char *temp_json_str = ((web_server_context_t*)(req->user_ctx))->scratch;

    httpd_resp_set_type(req, "application/json");

    // according wifi connect status, update state
    if (connection_info->config_status == ESP_AT_WIFI_STA_CONNECT_OK) {
        json_len += sprintf(temp_json_str + json_len, "{\"state\":1,"); // it means wifi connect success
    } else if (connection_info->config_status == ESP_AT_WIFI_STA_CONNECT_FAIL) {
        json_len += sprintf(temp_json_str + json_len, "{\"state\":2,"); // it means wifi connect fail
    } else {
        json_len += sprintf(temp_json_str + json_len, "{\"state\":0,"); // it means http context OK
    }

    // add ssid to json str
    // note: escape special non-control characters in json format, see https://www.json.org/json-en.html for more details
    json_len += sprintf(temp_json_str + json_len, "\"sta_ssid\":\"");
    int32_t ssid_len = strlen((char *)(connect_config->ssid));
    for (int i = 0; i < ssid_len; i++) {
        uint8_t c = connect_config->ssid[i];
        if (c == '\\' || c == '\"' || c == '/') {
            json_len += sprintf(temp_json_str + json_len, "\\");
        }
        json_len += sprintf(temp_json_str + json_len, "%c", c);
    }
    json_len += sprintf(temp_json_str + json_len, "\",");

    // add password to json str
    // note: escape special non-control characters in json format, see https://www.json.org/json-en.html for more details
    json_len += sprintf(temp_json_str + json_len, "\"sta_password\":\"");
    int32_t password_len = strlen((char *)(connect_config->password));
    for (int i = 0; i < password_len; i++) {
        uint8_t c = connect_config->password[i];
        if (c == '\\' || c == '\"' || c == '/') {
            json_len += sprintf(temp_json_str + json_len, "\\");
        }
        json_len += sprintf(temp_json_str + json_len, "%c", c);
    }
    json_len += sprintf(temp_json_str + json_len, "\",");

    switch (connection_info->config_status) {
    case ESP_AT_WIFI_STA_NOT_START:
        strcpy(temp_str, "waiting config");
        break;
    case ESP_AT_WIFI_STA_CONFIG_DONE:
        strcpy(temp_str, "config done");
        break;
    case ESP_AT_WIFI_STA_CONNECTING:
        strcpy(temp_str, "connecting");
        break;
    case ESP_AT_WIFI_STA_CONNECT_FAIL:
        strcpy(temp_str, "connect fail");
        break;
    case ESP_AT_WIFI_STA_CONNECT_OK:
        strcpy(temp_str, "connect OK");
        break;
    default:
        break;
    }
    json_len += sprintf(temp_json_str + json_len, "\"message\":\"%s\"}", temp_str);

    ESP_LOGD(TAG, "now wifi get json str is %s\n", temp_json_str);
    httpd_resp_send(req, temp_json_str, (temp_json_str == NULL) ? 0 : strlen(temp_json_str));

    return ESP_OK;
}

static esp_err_t at_web_version_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    char *temp_json_str = ((web_server_context_t*)(req->user_ctx))->scratch;
    sprintf(temp_json_str, "{\"version\":\"%s\"}", ESP_AT_WEB_VERSION);
    ESP_LOGD(TAG, "ready to send version: %s\n", temp_json_str);

    httpd_resp_send(req, temp_json_str, (temp_json_str == NULL) ? 0 : strlen(temp_json_str));

    return ESP_OK;
}

static esp_err_t accept_wifi_result_post_handler(httpd_req_t *req)
{
    char *buf = ((web_server_context_t*)(req->user_ctx))->scratch;
    int32_t received_flag;
    char temp[4] = {0};
    int str_len = 0;

    wifi_sta_connection_info_t wifi_connection_info = {0};
    wifi_sta_connection_info_t *connection_info = at_web_get_sta_connection_info();
    memset(buf, '\0', ESP_AT_WEB_SCRATCH_BUFSIZE * sizeof(char));
    ESP_LOGD(TAG, "detect web close pages");

    switch (connection_info->config_status) {
    case ESP_AT_WIFI_STA_NOT_START:
        break;
    case ESP_AT_WIFI_STA_CONFIG_DONE:
    case ESP_AT_WIFI_STA_CONNECTING:
        esp_at_port_active_write_data((uint8_t*)s_wifi_conncet_finish_response, strlen(s_wifi_conncet_finish_response));
        break;
    case ESP_AT_WIFI_STA_CONNECT_FAIL:
    case ESP_AT_WIFI_STA_CONNECT_OK:
        if (recv_post_data(req, buf) != ESP_OK) {
            at_web_response_error(req, HTTPD_500);
            ESP_LOGE(TAG, "recv post data error");
            return ESP_FAIL;
        }

        str_len = at_web_find_arg(buf, "received", (char*) temp, sizeof(temp));
        if (str_len == -1) {
            ESP_LOGE(TAG, "received flag is abnormal");
            goto error_handle;
        } else {
            received_flag = atoi(temp);
            ESP_LOGD(TAG, "received_flag is %d", received_flag);
        }

        // clear wifi connect config and status info
        wifi_connection_info.config_status = ESP_AT_WIFI_STA_NOT_START;

        at_web_clear_sta_connect_config();
        at_web_update_sta_connection_info(&wifi_connection_info);

        // send a message to MCU
        esp_at_port_active_write_data((uint8_t*)s_wifi_conncet_finish_response, strlen(s_wifi_conncet_finish_response));
        break;
    default:
        break;
    }
    at_web_response_ok(req);
    return ESP_OK;
error_handle:
    at_web_response_error(req, HTTPD_400);
    return ESP_FAIL;
}

static esp_err_t ap_record_get_handler(httpd_req_t *req)
{
    uint16_t ap_number = ESP_AT_WEB_AP_SCAN_NUM_DEFAULT;
    int loop = 0;
    int32_t json_len = 0;
    int valid_ap_count = 1;
    char *temp_json_str = NULL;
    wifi_ap_record_t *ap_info = (wifi_ap_record_t*) malloc(ESP_AT_WEB_AP_SCAN_NUM_DEFAULT * sizeof(wifi_ap_record_t));
    if (ap_info == NULL) {
        ESP_LOGE(TAG, "ap info malloc fail");
        return ESP_FAIL;
    }
    memset(ap_info, 0, ESP_AT_WEB_AP_SCAN_NUM_DEFAULT * sizeof(wifi_ap_record_t));

    if (at_web_wifi_scan_get_ap_records(&ap_number, ap_info) != ESP_OK) {
        at_web_response_error(req, HTTPD_500);
        goto error_handle;
    }

    temp_json_str = (char*) malloc(ESP_AT_WEB_AP_RECORD_JSON_STR_LEN * sizeof(char));
    if (temp_json_str == NULL) {
        ESP_LOGE(TAG, "temp_json_str malloc fail");
        goto error_handle;
    }
    memset(temp_json_str, 0, ESP_AT_WEB_AP_RECORD_JSON_STR_LEN * sizeof(char));

    httpd_resp_set_type(req, "application/json");
    json_len += sprintf(temp_json_str + json_len, "{\"state\":0,\"message\":\"scan done\",\"aplist\":["); // to get a json array format str

    for (loop = 0; loop < ap_number; loop++) {
        int32_t ssid_len = strlen((const char*)ap_info[loop].ssid);
        if (ssid_len != 0) { // ingore hidden ssid
            json_len += sprintf(temp_json_str + json_len, "{\"ssid\":\"");
            for (int i = 0; i < ssid_len; i++) {
                uint8_t c = ap_info[loop].ssid[i];
                // escape special non-control characters in json format, see https://www.json.org/json-en.html for more details
                if (c == '\\' || c == '\"' || c == '/') {
                    json_len += sprintf(temp_json_str + json_len, "\\");
                }
                json_len += sprintf(temp_json_str + json_len, "%c", c);
            }
            json_len += sprintf(temp_json_str + json_len, "\",\"auth_mode\":%d},", ap_info[loop].authmode);

            valid_ap_count++;
        }
    }
    free(ap_info);
    ap_info = NULL;

    json_len += sprintf(temp_json_str + json_len - 1, "]}");
    ESP_LOGD(TAG, "now, valid ap num is %d, ap records json str is %s\n", valid_ap_count, temp_json_str);
    httpd_resp_send(req, temp_json_str, (temp_json_str == NULL) ? 0 : strlen(temp_json_str));
    free(temp_json_str);
    temp_json_str = NULL;
    return ESP_OK;
error_handle:
    free(ap_info);
    ap_info = NULL;
    return ESP_FAIL;
}

const esp_partition_t *at_web_get_ota_update_partition(void)
{
    const esp_partition_t *update_partition = NULL;
    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    if (configured != running) {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                 configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);

    update_partition = esp_ota_get_next_update_partition(running);
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
             update_partition->subtype, update_partition->address);

    if (update_partition->address == running->address) {
        ESP_LOGE(TAG, "The partition to be updated is the current running partition");
        update_partition = NULL;
    }

    return update_partition;
}

esp_err_t at_web_ota_end(esp_ota_handle_t handle, const esp_partition_t *partition)
{
    esp_err_t err = esp_ota_end(handle);
    if (err != ESP_OK) {
        if (err == ESP_ERR_OTA_VALIDATE_FAILED) {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        ESP_LOGE(TAG, "esp_ota_end failed (%s)!", esp_err_to_name(err));
        return ESP_FAIL;
    }

    err = esp_ota_set_boot_partition(partition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
        return ESP_FAIL;
    }
    return err;
}

static esp_err_t ota_upgrade(httpd_req_t *req)
{
    char *buf = ((web_server_context_t*)(req->user_ctx))->scratch;
    int total_len = req->content_len;
    int remaining_len = req->content_len;
    int received_len = 0;
    esp_err_t err = ESP_FAIL;
    esp_ota_handle_t update_handle = 0;
    const esp_partition_t *update_partition = at_web_get_ota_update_partition();
    // check post data size
    if (update_partition->size < total_len) {
        ESP_LOGE(TAG, "ota data too long, partition size is %u, bin size is %d", update_partition->size, total_len);
        goto err_handler;
    }
    ESP_LOGI(TAG, "bin size is %d", total_len);
    memset(buf, 0x0, ESP_AT_WEB_SCRATCH_BUFSIZE * sizeof(char));

    // start ota
    err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ota begin failed (%s)", esp_err_to_name(err));
        goto err_handler;
    }
    // receive ota data
    while (remaining_len > 0) {
        received_len = httpd_req_recv(req, buf, MIN(remaining_len, ESP_AT_WEB_SCRATCH_BUFSIZE)); // Receive the file part by part into a buffer
        if (received_len <= 0) { // received error
            if (received_len == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry if timeout occurred */
                continue;
            }
            ESP_LOGE(TAG, "Failed to receive post ota data, err = %d", received_len);
            esp_ota_end(update_handle);
            goto err_handler;
        } else { // received successfully
            err = esp_ota_write(update_handle, buf, received_len);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "ota write failed (%s)", esp_err_to_name(err));
                esp_ota_end(update_handle);
                goto err_handler;
            }
            remaining_len -= received_len;
        }
    }
    err = at_web_ota_end(update_handle, update_partition);
    if (err != ESP_OK) {
        goto err_handler;
    }
    at_web_response_ok(req);
    esp_at_port_active_write_data((uint8_t*)s_ota_receive_success_response, strlen(s_ota_receive_success_response));
    ESP_LOGI(TAG, "ota end successfully, please restart");
    return ESP_OK;

err_handler:
    at_web_response_error(req, HTTPD_500);
    esp_at_port_active_write_data((uint8_t*)s_ota_receive_fail_response, strlen(s_ota_receive_fail_response));
    return ESP_FAIL;
}

static esp_err_t partition_upgrade(httpd_req_t *req, char *buf, const int total_len, const char *partition_name)
{
    int single_received_len = 0;
    int total_received_len = 0;
    int remaining_len = total_len;
    esp_err_t err = ESP_FAIL;

    if ((buf == NULL) || (total_len == 0) || (partition_name == NULL)) {
        return ESP_FAIL;
    }

    const esp_partition_t *at_custom_partition = esp_at_custom_partition_find(0x0, 0x0, partition_name);
    if (at_custom_partition == NULL) {
        ESP_LOGE(TAG, "no custom partition: %s", partition_name);
        return ESP_FAIL;
    }

    if (total_len > at_custom_partition->size) {
        ESP_LOGE(TAG, "received size %d is not equal %s size %d", total_len, partition_name, at_custom_partition->size);
        return ESP_FAIL;
    }

    if (esp_partition_erase_range(at_custom_partition, 0, at_custom_partition->size) != ESP_OK) {
        ESP_LOGE(TAG, "esp_partition_erase_range failed!");
        return ESP_FAIL;
    }

    for (;;) {
        single_received_len = httpd_req_recv(req, buf, MIN(remaining_len, ESP_AT_WEB_SCRATCH_BUFSIZE));
        if (single_received_len <= 0) {
            if (single_received_len == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry if timeout occurred */
                continue;
            }
            ESP_LOGE(TAG, "Failed to receive post ota data, err = %d", single_received_len);
            return ESP_FAIL;
        } else {
            if (esp_partition_write(at_custom_partition, total_received_len, (uint8_t *)buf, single_received_len) != ESP_OK) {
                ESP_LOGE(TAG, "esp_partition_write failed!");
                return ESP_FAIL;
            }

            total_received_len += single_received_len;
            remaining_len -= single_received_len;

            if (total_received_len == total_len) {
                break;
            }
        }
    }

    return ESP_OK;
}

static esp_err_t at_customize_partition_upgrade(httpd_req_t *req, const char* partition_name)
{
    esp_err_t err = ESP_OK;
    char *buf = ((web_server_context_t*)(req->user_ctx))->scratch;
    int total_len = req->content_len;

    err = partition_upgrade(req, buf, total_len, partition_name);
    if (err == ESP_OK) {
        at_web_response_ok(req);
        esp_at_port_active_write_data((uint8_t*)s_ota_receive_success_response, strlen(s_ota_receive_success_response));
        return ESP_OK;
    } else {
        at_web_response_error(req, HTTPD_500);
        esp_at_port_active_write_data((uint8_t*)s_ota_receive_fail_response, strlen(s_ota_receive_fail_response));
        ESP_LOGE(TAG, "%s partition upgrade failed", partition_name);
        return ESP_FAIL;
    }

    return ESP_FAIL;
}

extern const esp_partition_t *esp_at_custom_partition_find_next(const esp_partition_t *start_from);
static esp_err_t ota_info_get_handler(httpd_req_t *req)
{
    uint32_t version_uint32 =  esp_at_get_version();
    int32_t json_len = 0;
    uint8_t version[4] = {0};
    char *temp_json_str = ((web_server_context_t*)(req->user_ctx))->scratch;
    esp_partition_t *cur_partition = NULL;

    memcpy(version, &version_uint32, sizeof(version_uint32));

    httpd_resp_set_type(req, "application/json");

    // OTA information start
    json_len += sprintf(temp_json_str + json_len, "{");

    // OTA information
    json_len += sprintf(temp_json_str + json_len, "\"state\":0,"); // it means http context OK
    json_len += sprintf(temp_json_str + json_len, "\"fw_version\":\"%s\",", CONFIG_ESP_AT_FW_VERSION); // it means http context OK
    json_len += sprintf(temp_json_str + json_len, "\"at_core_version\":\"%d.%d.%d.%d\",", version[3], version[2], version[1], version[0]);

    // partition information array start
    json_len += sprintf(temp_json_str + json_len, "\"partitions\":");
    json_len += sprintf(temp_json_str + json_len, "[");

    // OTA partition information
    if (at_web_get_ota_update_partition()) {
        json_len += sprintf(temp_json_str + json_len, "\"ota\",");
    }

    // AT customize partition information
    for (;;) {
        const esp_partition_t *partition = esp_at_custom_partition_find_next((const esp_partition_t *)cur_partition);
        if (partition) {
            json_len += sprintf(temp_json_str + json_len, "\"%s\",", partition->label);
            cur_partition = (esp_partition_t *)partition;
        } else {
            json_len -= 1;
            break;
        }
    }

    // partition information array end
    json_len += sprintf(temp_json_str + json_len, "]");

    // OTA information end
    json_len += sprintf(temp_json_str + json_len, "}");

    ESP_LOGD(TAG, "now ota get json str is %s\n", temp_json_str);
    httpd_resp_send(req, temp_json_str, strlen(temp_json_str));

    return ESP_OK;
}

static esp_err_t ota_data_post_handler(httpd_req_t *req)
{
    char* obj_name = NULL;
    size_t obj_len = 0;
    esp_err_t err = ESP_OK;

    // now the key "Object" in header indicates operation object
    // value "ota" indicates data belong to OTA
    // others indicate data belong AT customize partition, for example "mqtt_key" represents AT customize mqtt_key partition
    obj_len = httpd_req_get_hdr_value_len(req, ESP_AT_WEB_HEADER_AUTH_NAME) + 1;
    if (obj_len) {
        obj_name = calloc(1, obj_len);

        if (httpd_req_get_hdr_value_str(req, ESP_AT_WEB_HEADER_AUTH_NAME, obj_name, obj_len) != ESP_OK) {
            ESP_LOGE(TAG, "%s get error", ESP_AT_WEB_HEADER_AUTH_NAME);

            free(obj_name);
            return ESP_FAIL;
        }

        // Send a message to MCU.
        esp_at_port_active_write_data((uint8_t*)s_ota_start_response, strlen(s_ota_start_response));

        if (strncmp(obj_name, ESP_AT_UPGRADE_PARTITION_NAME, strlen(ESP_AT_UPGRADE_PARTITION_NAME)) == 0) {
            err = ota_upgrade(req);
        } else {
            err = at_customize_partition_upgrade(req, obj_name);
        }

        free(obj_name);
    }

    return err;
}

#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
/* http 404/414 error handler that redirect all requests to the root page */
static esp_err_t http_common_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    /* Set status */
    httpd_resp_set_status(req, "302 Temporary Redirect");

    /* Redirect to the "/" root directory */
    httpd_resp_set_hdr(req, "Location", s_at_web_redirect_url);

    /* iOS require content in the response to detect a captive portal,
       simply redirecting is not sufficient.
     */
    httpd_resp_send(req, "Redirect to captive portal", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Redirecting to root");

    return ESP_OK;
}
#endif

static esp_err_t start_web_server(const char *base_path, uint16_t server_port)
{
    ESP_AT_WEB_SERVER_CHECK(base_path, "wrong base path", err);
    s_web_context = calloc(1, sizeof(web_server_context_t));
    ESP_AT_WEB_SERVER_CHECK(s_web_context, "No memory for rest context", err);
    strlcpy(s_web_context->base_path, base_path, sizeof(s_web_context->base_path));

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.max_open_sockets = 7; // It cannot be less than 7.
    config.server_port = server_port;

#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
    /* this is an important option that isn't set up by default.*/
    config.lru_purge_enable = true;
#endif

    ESP_LOGD(TAG, "Starting HTTP Server");
    ESP_AT_WEB_SERVER_CHECK(httpd_start(&s_server, &config) == ESP_OK, "Start server failed", err_start);

    httpd_uri_t httpd_uri_array[] = {
        {"/getversion", HTTP_GET, at_web_version_get_handler, s_web_context},
        {"/getstainfo", HTTP_GET, config_wifi_get_handler, s_web_context},
        {"/setstainfo", HTTP_POST, config_wifi_post_handler, s_web_context},
        {"/getresult", HTTP_POST, accept_wifi_result_post_handler, s_web_context},
        {"/getaprecord", HTTP_GET, ap_record_get_handler, s_web_context},
        {"/getotainfo", HTTP_GET, ota_info_get_handler, s_web_context},
        {"/setotadata", HTTP_POST, ota_data_post_handler, s_web_context},
        {"/", HTTP_GET, web_common_get_handler, s_web_context},
    };

    for (int i = 0; i < sizeof(httpd_uri_array) / sizeof(httpd_uri_t); i++) {
        if (httpd_register_uri_handler(s_server, &httpd_uri_array[i]) != ESP_OK) {
            ESP_LOGE(TAG, "httpd register uri_array[%d] fail", i);
        }
    }
#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
    size_t redirect_url_sz = ESP_AT_WEB_REDIRECT_URL_PREFIX_LEN + strlen(ESP_AT_WEB_ROOT_DIR_DEFAULT) + 1; /* strlen(http://255.255.255.255) + strlen("/") + 1 for \0 */
    s_at_web_redirect_url = malloc(sizeof(char) * redirect_url_sz);
    *s_at_web_redirect_url = '\0';
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"), &ip_info);
    snprintf(s_at_web_redirect_url, redirect_url_sz, "http://"IPSTR"%s", IP2STR(&ip_info.ip), ESP_AT_WEB_ROOT_DIR_DEFAULT);

    httpd_register_err_handler(s_server, HTTPD_404_NOT_FOUND, http_common_error_handler);
    httpd_register_err_handler(s_server, HTTPD_414_URI_TOO_LONG, http_common_error_handler);
    httpd_register_err_handler(s_server, HTTPD_405_METHOD_NOT_ALLOWED, http_common_error_handler);
#endif

    return ESP_OK;
err_start:
    free(s_web_context);
err:
    return ESP_FAIL;
}

static esp_err_t stop_web_server(void)
{
    ESP_AT_WEB_SERVER_CHECK(httpd_stop(s_server) == ESP_OK, "Stop server failed", err);
    free(s_web_context);
    s_web_context = NULL;
    s_server = NULL;
    ESP_LOGI(TAG, "Stop HTTP Server");
#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
    if (s_at_web_redirect_url) {
        free(s_at_web_redirect_url);
        s_at_web_redirect_url = NULL;
    }
#endif

    return ESP_OK;
err:
    return ESP_FAIL;
}

#ifdef CONFIG_AT_WEB_USE_FATFS
static esp_err_t at_web_fatfs_spiflash_mount(const char *base_path,
    const char *partition_label,
    const esp_vfs_fat_mount_config_t *mount_config,
    wl_handle_t *wl_handle)
{
    esp_err_t result = ESP_OK;

    const esp_partition_t *data_partition = (esp_partition_t*) esp_at_custom_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, partition_label);
    if (data_partition == NULL) {
        ESP_LOGE(TAG, "Failed to find FATFS partition (type='data'(%d), subtype='fat'(%d), partition_label='%s'). Check the partition table.",
            ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, partition_label);
        return ESP_ERR_NOT_FOUND;
    }

    result = wl_mount(data_partition, wl_handle);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "failed to mount wear levelling layer. result = %i", result);
        return result;
    }
    // connect driver to FATFS
    pdrv = 0xFF;
    if (ff_diskio_get_drive(&pdrv) != ESP_OK) {
        ESP_LOGD(TAG, "the maximum count of volumes is already mounted");
        return ESP_ERR_NO_MEM;
    }
    ESP_LOGD(TAG, "using pdrv=%i", pdrv);
    char drv[3] = { (char) ('0' + pdrv), ':', 0 };

    result = ff_diskio_register_wl_partition(pdrv, *wl_handle);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "ff_diskio_register_wl_partition failed pdrv=%i, error - 0x(%x)", pdrv, result);
        goto fail;
    }
    FATFS *fs;
    result = esp_vfs_fat_register(base_path, drv, mount_config->max_files, &fs);
    if (result == ESP_ERR_INVALID_STATE) {
        // it's okay, already registered with VFS
    } else if (result != ESP_OK) {
        ESP_LOGD(TAG, "esp_vfs_fat_register failed 0x(%x)", result);
        goto fail;
    }

    // Try to mount partition
    FRESULT fresult = f_mount(fs, drv, 1);
    if (fresult != FR_OK) {
        ESP_LOGW(TAG, "f_mount failed (%d)", fresult);
        result = ESP_FAIL;
        goto fail;
    }
    return ESP_OK;

fail:
    esp_vfs_fat_unregister_path(base_path);
    ff_diskio_unregister(pdrv);
    return result;
}

/*
*  Init file system, assert we can mount the fs.
*/
static esp_err_t at_web_fatfs_spiflash_init(void)
{
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 5,
        .format_if_mount_failed = false // If cannot mount fs, no need to go down
    };

    esp_err_t err = at_web_fatfs_spiflash_mount(ESP_AT_WEB_MOUNT_POINT, "fatfs", &mount_config, &s_wl_handle);

    if (err != ESP_OK) {
        printf("Failed to mount FATFS (0x%x)", err);
        return ESP_FAIL;
    }

    printf("Mount FATFS success\n");
    return ESP_OK;
}

static esp_err_t at_web_fatfs_spiflash_deinit(void)
{
    esp_err_t ret;
    ff_diskio_unregister(pdrv);
    ff_diskio_clear_pdrv_wl(s_wl_handle);
    wl_unmount(s_wl_handle);
    ret = esp_vfs_fat_unregister_path(ESP_AT_WEB_MOUNT_POINT);

    return ret;
}
#endif

static esp_err_t at_web_start(uint16_t server_port)
{
    esp_err_t err;

    if (s_server == NULL) {
        /*AT web can use fatfs to storge html or use embeded file to storge html.If use fatfs, we should enable AT FS Command support*/
#ifdef CONFIG_AT_WEB_USE_FATFS
        err = at_web_fatfs_spiflash_init();
        if (err != ESP_OK) {
            return err;
        }
#endif
        err = start_web_server(ESP_AT_WEB_MOUNT_POINT, server_port);
        if (err != ESP_OK) {
            return err;
        }
#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
        at_dns_server_start();
        esp_log_level_set("httpd_uri", ESP_LOG_ERROR);
        esp_log_level_set("httpd_txrx", ESP_LOG_ERROR);
        esp_log_level_set(TAG, ESP_LOG_INFO);
#endif
    }

    return ESP_OK;
}

static esp_err_t at_web_destory(void)
{
    esp_err_t err;

    if (s_server != NULL) {
        if ((err = stop_web_server()) != ESP_OK) {
            return err;
        }
#ifdef CONFIG_AT_WEB_CAPTIVE_PORTAL_ENABLE
        at_dns_server_stop();
#endif
#ifdef CONFIG_AT_WEB_USE_FATFS
        if ((err = at_web_fatfs_spiflash_deinit()) != ESP_OK) {
            return err;
        }
#endif
    }

    return ESP_OK;
}

/*
* AT+WEBSERVER=1, port, Connect_Timeout(Creat a Web Server with specified port)
* AT+WEBSERVER=0(deinit)
*/
static uint8_t at_setupCmdWebConf(uint8_t para_num)
{
    esp_err_t ret;
    uint8_t cnt = 0;
    int32_t enable = 0, port = -1, reconnect_timeout = 0;

    if (esp_at_get_para_as_digit(cnt++, &enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (enable < 0 || enable > 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((para_num == cnt) && (enable == 0)) {
        printf("Delete Web Server\r\n");
        if ((ret = at_web_destory()) == ESP_OK) {
            return ESP_AT_RESULT_CODE_OK;
        } else {
            printf("at web destroy fail, err = %d\r\n", ret);
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }

    if (esp_at_get_para_as_digit(cnt++, &port) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (port < 0) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit(cnt++, &reconnect_timeout) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (reconnect_timeout < ESP_AT_WEB_WIFI_MIN_RECONNECT_TIMEOUT || reconnect_timeout > ESP_AT_WEB_WIFI_MAX_RECONNECT_TIMEOUT) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((ret = at_web_start(port)) == ESP_OK) {
        at_web_update_sta_reconnect_timeout(reconnect_timeout);
        printf("start web server\r\n");
        return ESP_AT_RESULT_CODE_OK;
    } else {
        printf("at web start fail, err = %d\r\n", ret);
        return ESP_AT_RESULT_CODE_FAIL;
    }
}

static const esp_at_cmd_struct at_web_cmd[] = {
    {"+WEBSERVER", NULL, NULL,  at_setupCmdWebConf, NULL},
};

static void at_web_got_ip_cb(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    at_web_update_sta_got_ip_flag(true);
}

bool esp_at_web_server_cmd_regist(void)
{
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &at_web_got_ip_cb, NULL, NULL);
    return esp_at_custom_cmd_array_regist(at_web_cmd, sizeof(at_web_cmd) / sizeof(at_web_cmd[0]));
}

ESP_AT_CMD_SET_FIRST_INIT_FN(esp_at_web_server_cmd_regist, 25);

#endif
