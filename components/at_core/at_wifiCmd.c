/*
 * File : at_wifiCmd.c
 * This file is part of Espressif's AT+ command set program.
 * Copyright (C) 2013 - 2016, Espressif Systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//#include "user_interface.h"

#include "at.h"
#include "at_wifiCmd.h"
#include "at_nvm.h"

#include "lwip/ip4_addr.h"
#include "esp_wps.h"
#include "esp_system.h"
#include "esp_log.h"

#include "tcpip_adapter.h"

#include "esp_smartconfig.h"

#include <time.h>
#include <sys/time.h>
#include "apps/sntp/sntp.h"
#include "string.h"

static const char *TAG = "at_wifiCmd.c";

char ap_list_out[MAX_AP_LIST_LENGTH] = {0};

static bool at_wifi_cmd_need_save_flag = FALSE;
static bool at_wifi_cmd_read_from_flash_flag = FALSE;

#define AP_PRINT_MASK   (0x1F)
static uint32_t ap_sort_flag = 0;
static uint32_t ap_print_mask = AP_PRINT_MASK;

extern bool at_wifi_auto_reconnect_flag;

// AP list related
#define STX_CHAR    0x02
#define ETX_CHAR    0x03
extern uint32_t ap_list_idx;

/** @defgroup AT_WSIFICMD_Functions
 * @{
 */

bool at_str_2_mac(uint8_t *str, uint8_t *dest)
{
    uint8_t loop = 0;
    uint8_t tmp = 0;
    uint8_t *src_p = str;
    if (at_strlen(src_p) != 17) { // must be like 12:34:56:78:90:AB
        return FALSE;
    }
    for (loop = 0; loop < 17 ; loop++) {
        if (loop % 3 == 2) {
            if (src_p[loop] != ':') {
                return FALSE;
            }
            continue;
        }

        if ((src_p[loop] >= '0') && (src_p[loop] <= '9')) {
            tmp = tmp * 16 + src_p[loop] - '0';
        } else if ((src_p[loop] >= 'A') && (src_p[loop] <= 'F')) {
            tmp = tmp * 16 + src_p[loop] - 'A' + 10;
        } else if ((src_p[loop] >= 'a') && (src_p[loop] <= 'f')) {
            tmp = tmp * 16 + src_p[loop] - 'a' + 10;
        } else {
            return FALSE;
        }

        if (loop % 3 == 1) {
            *dest++ = tmp;
            tmp = 0;
        }
    }
    return TRUE;
}
/**
 * @brief  Test commad of set wifi mode.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_testCmdCwmode(uint8_t *cmd_name)
{
    uint8_t temp[32];
    at_sprintf(temp, "%s:(1-3)\r\n", cmd_name);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Query commad of set wifi mode.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_queryCmdCwmode(uint8_t *cmd_name)
{
    uint8_t temp[32];
    wifi_mode_t opmode = WIFI_MODE_NULL;

    if (at_wifi_cmd_read_from_flash_flag) {
        //opmode = wifi_get_opmode_default();
    } else {
        if (esp_wifi_get_mode(&opmode) != ESP_OK) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }
    at_sprintf(temp, "%s:%d\r\n", cmd_name, opmode);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Setup commad of set wifi mode.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCwmode(uint8_t para_num)
{
    int32_t mode = 0;
    uint8_t cnt = 0;
    uint8_t ret = ESP_AT_RESULT_CODE_ERROR;
    bool result = FALSE;

    if (para_num != 1) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1, para_num));
        return ret;
    }
    if (esp_at_get_para_as_digit (cnt++, &mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ret;
    }
    if ((mode >= 1) && (mode <= 3)) {
        result = esp_wifi_set_mode(mode);
        if ((mode == 1) || (mode == 3)) {
            mdState = m_idle;
        } else {
            mdState = m_inactive;
        }

        if (result == ESP_OK) {
            return ESP_AT_RESULT_CODE_OK;
        } else {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    } else {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(1));
        return ESP_AT_RESULT_CODE_ERROR;
    }
}

uint8_t at_queryCmdCwmodeDef(uint8_t *cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCwmode(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}
/**
 * @brief  Setup commad of set wifi mode.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCwmodeDef(uint8_t para_num)
{
    uint8_t ret = 0;
    esp_wifi_set_storage(WIFI_STORAGE_FLASH);
    ret = at_setupCmdCwmode(para_num);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    return ret;
}
#if 0
void ICACHE_FLASH_ATTR
at_cmd_ping_recv_cb(void *arg, void *pdata)
{
    struct ping_resp *resp = (struct ping_resp *)pdata;
    uint8_t tmp[16] = {0};
    if (resp == NULL) {
        return;
    }

    if (resp->ping_err == -1) {
        at_port_print((uint8_t *)"+timeout\r\n");
    } else {
        at_sprintf(tmp, "+%d\r\n", resp->resp_time);
        at_port_print(tmp);
    }
}
void ICACHE_FLASH_ATTR
at_cmd_ping_send_cb(void *arg, void *pdata)
{
    struct ping_option *ping_opt = (struct ping_option *)arg;
    struct ping_resp *resp = (struct ping_resp *)pdata;
    if (ping_opt) {
        at_free(ping_opt);
    }
    if (resp == NULL) {
        at_response_error();
        return;
    }
    if (resp->timeout_count == resp->total_count) {
        at_response_error();
    } else {
        at_response_ok();
    }
}


void ICACHE_FLASH_ATTR
at_ping_start(struct ping_option *ping_opt, uint32_t ip, int32_t times, int32_t interval)
{
    ping_opt->ip = ip;
    ping_opt->count = times;
    ping_opt->coarse_time = interval;
    ping_start(ping_opt);
}
void ICACHE_FLASH_ATTR
at_ping_dns_found_cb(const char *name, ip_addr_t *ipaddr, void *callback_arg)
{
    struct ping_option *ping_opt = (struct ping_option *)callback_arg;
    if (ping_opt == NULL) {
        at_response_error();
        return ;
    }

    if (ipaddr == NULL) {
        at_free(ping_opt);
        at_response_error();
        return ;
    }
    at_ping_start(ping_opt, ipaddr->addr, ping_opt->count, ping_opt->coarse_time);
}

//AT+PING="192.168.1.5"
void ICACHE_FLASH_ATTR
at_setupCmdPing(uint8_t id, char *pPara)
{
    uint8_t hostname[128] = {0};
    int32_t tmp = 0;
    int32_t interval = 1;
    int32_t times = 1;
    ip_addr_t ip_tmp;
    int ret = 0;
    struct ping_option *ping_opt = NULL;

    pPara++;
    ret = at_data_str_copy(hostname, &pPara, 128);
    if ((ret == 0) || (ret == -1)) {
        at_response_error();
        return ;
    }

    if (*pPara != '\r') {
        at_response_error();
        return ;
    }
    ping_opt = (struct ping_option *)os_zalloc(sizeof(struct ping_option));
    if (ping_opt == NULL) {
        at_response_error();
        return ;
    }
    ping_opt->coarse_time = times;
    ping_opt->count = interval;
    ping_regist_recv(ping_opt, at_cmd_ping_recv_cb);
    ping_regist_sent(ping_opt, at_cmd_ping_send_cb);

    // parse hostname
    ret = espconn_gethostbyname((struct espconn *)ping_opt, hostname, &ip_tmp, at_ping_dns_found_cb);
    if (ESPCONN_OK == ret) {
        at_ping_start(ping_opt, ip_tmp.addr, times, interval);
    } else if (ESPCONN_INPROGRESS == ret) {
    } else {
        at_response_error();
        at_free(ping_opt);
        return ;
    }
}
#endif

#ifdef AT_SCAN_SUPPORT
/**
 * @brief  Wifi ap scan over callback to display.
 * @param  arg: contain the aps information
 * @param  status: scan over status
 * @retval None
 */
#define SCAN_DONE_FORMAT_PACKET(flag,buf,format,par)    \
    do {                                                \
        if (mask & 0x01) {  \
            if(flag == FALSE){    \
                at_sprintf((buf), format,par);    \
                flag = TRUE;    \
            } else {  \
                at_sprintf((buf), ","format,par);    \
            }\
        }                   \
        mask = mask >>1;    \
    } while(0)

static bool at_cwlap_response(void)
{
    uint8_t ssid[33];
    uint16_t ap_num = 0;
    uint16_t loop = 0;
    bool comma_flag = FALSE;
    uint32_t mask = ap_print_mask;
    wifi_ap_record_t *ap_list = NULL;
    wifi_ap_record_t temp_wifi_ap;
    uint16_t begin_index, max_index, cur_index;

    if (esp_wifi_scan_get_ap_num(&ap_num) != ESP_OK) {
        return false;
    }

    if (ap_num == 0) {
        return true;
    }

    ESP_LOGI(TAG, "Found %d AP records.", ap_num);

    ap_list = (wifi_ap_record_t *)at_malloc(ap_num * sizeof(wifi_ap_record_t));

    if (ap_list == NULL) {
        return false;
    }

    if (esp_wifi_scan_get_ap_records(&ap_num, ap_list) == ESP_OK) {
        uint8_t *temp = (uint8_t *)at_malloc(MAX_AP_RECORD_LENGTH);
#ifdef AT_SCAN_SORT_SUPPORT
        if (ap_sort_flag == 1) {

            for (begin_index = 0; begin_index < ap_num; begin_index++) {
                for (cur_index = begin_index, max_index = cur_index; cur_index < ap_num; cur_index++) {
                    if (ap_list[cur_index].rssi > ap_list[max_index].rssi) {
                        max_index = cur_index;
                    }
                }

                if (max_index != begin_index) {
                    at_memcpy(&temp_wifi_ap, &ap_list[begin_index], sizeof(temp_wifi_ap));
                    at_memcpy(&ap_list[begin_index], &ap_list[max_index], sizeof(ap_list[begin_index]));
                    at_memcpy(&ap_list[max_index], &temp_wifi_ap, sizeof(ap_list[max_index]));
                }
            }
        }
#endif

        // NULL out array
        at_memset(ap_list_out, 0x00, sizeof(ap_list_out));

        for (loop = 0; loop < ap_num; loop++) {
            at_memset(ssid, 0, 33);
            at_strncpy(ssid, ap_list[loop].ssid, sizeof(ap_list[loop].ssid));

            comma_flag = FALSE;
            mask = ap_print_mask;

            at_sprintf(temp, "%c%d: %s", STX_CHAR, loop, "+CWLAP:(");

            SCAN_DONE_FORMAT_PACKET(comma_flag, temp + at_strlen(temp), "%d", ap_list[loop].authmode);
            SCAN_DONE_FORMAT_PACKET(comma_flag, temp + at_strlen(temp), "\"%s\"", ssid);
            SCAN_DONE_FORMAT_PACKET(comma_flag, temp + at_strlen(temp), "%d", ap_list[loop].rssi);
            SCAN_DONE_FORMAT_PACKET(comma_flag, temp + at_strlen(temp), "\""MACSTR"\"",MAC2STR(ap_list[loop].bssid));
            SCAN_DONE_FORMAT_PACKET(comma_flag,temp + at_strlen(temp), "%d",ap_list[loop].primary);

            // Offset so we don't print STX_CHAR to AT output
            at_port_print(temp + 1);
            at_port_print((unsigned char*)")\r\n");

            // Add AP record + ETX to BT transmission list
            at_sprintf(temp + at_strlen(temp), "%s%c",")", ETX_CHAR);
            strncpy( ap_list_out + strnlen((char*)ap_list_out, MAX_AP_LIST_LENGTH-MAX_AP_RECORD_LENGTH), (char*)temp, MAX_AP_RECORD_LENGTH);
        }


        // Signal bt_provisioning..
        ap_list_idx = 0;

        at_free(ap_list);
        at_free(temp);
        return true;
    } else {
        at_free(ap_list);
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return false;
    }
}


uint8_t at_setupCmdCwlap(uint8_t para_num)
{
    wifi_scan_config_t config;
    uint8_t bssid[6] = {0};
    int32_t channel = 0;
    uint8_t cnt = 0;
    uint8_t ret = ESP_AT_RESULT_CODE_ERROR;

    ESP_LOGI(TAG, "at_wifiCmd::at_setupCmdCwlap, para_num: %d", para_num);

    at_memset(&config,0x0,sizeof(config));

    if (para_num == 0)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(3,para_num));
        return ret;
    }

    if(esp_at_get_para_as_str (cnt++,(uint8_t**)&config.ssid) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ret;
    }
    if((config.ssid != NULL) && (at_strlen(config.ssid) > 32))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ret;
    }
    if(para_num != cnt)  // bssid
    {
        if(esp_at_get_para_as_str (cnt++,&config.bssid) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ret;
        }

        if (config.bssid != NULL) {
            if (at_strlen(config.bssid) == 17) {
                if(at_str_2_mac(config.bssid,bssid) != TRUE)
                {
                    esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
                    return ret;
                }
                config.bssid = bssid;
            } else if (at_strlen(config.bssid) == 0) {
                config.bssid = NULL;
            } else {
                esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
                return ret;
            }
        }
    }

    if(para_num != cnt)  // channel
    {
        if(esp_at_get_para_as_digit (cnt++,&channel) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ret;
        }

        if((channel > 13) || (channel < 0))
        {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ret;
        }
        config.channel = channel;
    }

    if(para_num != cnt)
    {
        return ret;
    }

    config.show_hidden = true;
    if(esp_wifi_scan_start(&config, true) != ESP_OK) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return ESP_AT_RESULT_CODE_FAIL;
    }

    if (at_cwlap_response()) {
        return ESP_AT_RESULT_CODE_OK;
    } else {
        return ESP_AT_RESULT_CODE_FAIL;
    }
}

/**
 * @brief  Execution commad of list wifi aps.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdCwlap(uint8_t* cmd_name)
{
    wifi_mode_t mode = WIFI_MODE_NULL;
    wifi_scan_config_t config;

    at_memset(&config,0x0,sizeof(config));
    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }
    
    if ((mode == WIFI_MODE_AP) || (mode == WIFI_MODE_NULL))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    config.show_hidden = true;
    if (esp_wifi_scan_start(&config,true) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }

    if (at_cwlap_response()) {
        return ESP_AT_RESULT_CODE_OK;
    } else {
        return ESP_AT_RESULT_CODE_FAIL;
    }
}
#endif
uint8_t at_setupCmdCwlapOpt(uint8_t para_num)
{
    int32_t mask = 0;
    int32_t flag = 0;
    uint8_t cnt = 0;

    if(para_num != 2) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(2,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (cnt++,&flag) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((flag != 0) && (flag != 1)) {
        at_port_print((uint8_t*)"flag error\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (cnt++,&mask) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (mask > 0x1F) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    ap_print_mask = mask;
    ap_sort_flag = flag;

    return ESP_AT_RESULT_CODE_OK;
}
/**
 * @brief  Query commad of join to wifi ap.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_queryCmdCwjap(uint8_t* cmd_name)
{

    uint8_t temp[64];
    uint8_t ssid[33];
    wifi_ap_record_t config;

    at_memset(&config,0x0,sizeof(config));

    if(wifi_station_get_connect_status() != STATION_GOT_IP)
    {
        at_port_print((uint8_t*)"No AP\r\n");
        return ESP_AT_RESULT_CODE_OK;
    }

    if(at_wifi_cmd_read_from_flash_flag)
    {
        // wifi_station_get_config_default(&stationConf);
    }
    else
    {
        if (esp_wifi_sta_get_ap_info(&config) != ESP_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

    }

    if(at_strlen(config.ssid) != 0)
    {
    	at_snprintf(ssid,sizeof(config.ssid),"%s",config.ssid);
        at_sprintf(temp, "%s:\"%s\",\""MACSTR"\",%d,%d\r\n", cmd_name, ssid,MAC2STR(config.bssid),config.primary,config.rssi);
        at_port_print(temp);
        return ESP_AT_RESULT_CODE_OK;
    }
    else
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return ESP_AT_RESULT_CODE_ERROR;
    }
}

uint8_t at_queryCmdCwjapDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCwjap(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

/**
 * @brief  Setup commad of join to wifi ap.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCwjap(uint8_t para_num)
{
    wifi_config_t config;
    wifi_mode_t mode = WIFI_MODE_NULL;
    uint8_t cnt = 0;
    uint32_t len = 0;
    uint8_t* pStr = NULL;
    uint8_t status;
    uint8_t wait_time;
    uint8_t temp[32];

    //  stationConf /////////
    at_memset(&config,0x0, sizeof(config));

    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_LENGTH(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    len = at_strlen(pStr);
    if((len == 0) || (len > sizeof(config.sta.ssid)))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_LENGTH(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_memcpy(config.sta.ssid,pStr,len);

    if(esp_at_get_para_as_str (cnt++,&pStr) == ESP_AT_PARA_PARSE_RESULT_FAIL)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr != NULL)
    {
        len = at_strlen(pStr);
    }
    else
    {
        len = 0;
    }
    if(len > sizeof(config.sta.password))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_LENGTH(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_memcpy(config.sta.password, pStr, len);

    if(para_num != cnt) {
        if(esp_at_get_para_as_str (cnt++,&pStr) == ESP_AT_PARA_PARSE_RESULT_FAIL)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if (at_str_2_mac(pStr,config.sta.bssid) == false) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_LENGTH(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        config.sta.bssid_set = 1;
    }
    
    if(para_num != cnt)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(cnt,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    if ((mode == WIFI_MODE_AP) || (mode == WIFI_MODE_NULL))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_wifi_auto_reconnect_flag = false;
    esp_wifi_disconnect();

    if (esp_wifi_set_config(WIFI_IF_STA,&config) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }
    wifi_station_init_connect_status();
    at_wifi_auto_reconnect_flag = true;
    esp_wifi_connect();

    wait_time = 16;
    
    while(wait_time--) {
        vTaskDelay(1000/(1000 /xPortGetTickRateHz()));  // delay 1000 ms
        status = wifi_station_get_connect_status();

        if (status == STATION_GOT_IP) {
            at_wifi_auto_reconnect_flag = false;
            return ESP_AT_RESULT_CODE_OK;
        }
    }
    at_wifi_auto_reconnect_flag = false;
    esp_wifi_disconnect();
    at_sprintf(temp,"+CWJAP:%d\r\n",status);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_FAIL;
}

uint8_t at_setupCmdCwjapDef(uint8_t para_num)
{
    uint8_t ret = 0;
    esp_wifi_set_storage(WIFI_STORAGE_FLASH);
    ret = at_setupCmdCwjap(para_num);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    return ret;
}

/**
 * @brief  Test commad of quit wifi ap.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_testCmdCwqap(uint8_t* cmd_name)
{
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Execution commad of quit wifi ap.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdCwqap(uint8_t* cmd_name)
{
    at_wifi_auto_reconnect_flag = false;
    esp_wifi_disconnect();
//    mdState = m_wdact;
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Query commad of module as wifi ap.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_queryCmdCwsap(uint8_t* cmd_name)
{
    wifi_config_t config;
    wifi_mode_t mode = WIFI_MODE_NULL;
    uint8_t* temp = NULL;

    at_memset(&config,0x0,sizeof(config));
    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    if (mode == WIFI_MODE_STA)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(at_wifi_cmd_read_from_flash_flag)
    {
        //wifi_softap_get_config_default(&apConfig);
    }
    else
    {
        //wifi_softap_get_config(&apConfig);
    }

    if (esp_wifi_get_config(WIFI_IF_AP,&config) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }
    
    temp = (uint8_t*)at_malloc(128);
    at_sprintf(temp,"%s:\"%s\",\"%s\",%d,%d,%d,%d\r\n",
            cmd_name,
            config.ap.ssid,
            config.ap.password,
            config.ap.channel,
            config.ap.authmode,
            config.ap.max_connection,
            config.ap.ssid_hidden);
    at_port_print(temp);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCwsapDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCwsap(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

/**
 * @brief  Setup commad of module as wifi ap.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCwsap(uint8_t para_num)
{
    uint8_t len = 0;
    int32_t authmode = 0,channel = 0,max_connection = 0,ssid_hidden = 0;;
    wifi_config_t config;
    uint8_t* pStr = NULL;
    uint8_t cnt = 0;
    uint8_t password_len = 0;
    wifi_mode_t mode = WIFI_MODE_NULL;

    at_memset(&config,0x00,sizeof(config));
    if (esp_wifi_get_config(WIFI_IF_AP,&config) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }
    // ssid
    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    len = at_strlen(pStr);
    if((len == 0) || (len > 32))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_memset(config.ap.ssid,0x0,sizeof(config.ap.ssid));
    at_memcpy(config.ap.ssid, pStr, len);
    config.ap.ssid_len = len;
    // password
    if(esp_at_get_para_as_str (cnt++,&pStr) == ESP_AT_PARA_PARSE_RESULT_FAIL)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        password_len = 0;
    }
    else
    {
        password_len = at_strlen(pStr);
        if(password_len > 64)
        {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        at_memset(config.ap.password,0x0,sizeof(config.ap.password));
        at_memcpy(config.ap.password, pStr, password_len);
    }

    // channel
    if(esp_at_get_para_as_digit (cnt++,&channel) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if((channel < 1) || (channel > 13))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    config.ap.channel = channel;
    // authmode
    if(esp_at_get_para_as_digit (cnt++,&authmode) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if((authmode > WIFI_AUTH_WPA_WPA2_PSK) || (authmode < WIFI_AUTH_OPEN) || (authmode == WIFI_AUTH_WEP))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    config.ap.authmode = authmode;
    if((config.ap.authmode != WIFI_AUTH_OPEN) && (password_len < 8))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // max connection
    if (cnt != para_num) {
        if(esp_at_get_para_as_digit (cnt++,&max_connection) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if((max_connection <= 0) || (max_connection > ESP_WIFI_MAX_CONN_NUM)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        config.ap.max_connection = (uint8_t)max_connection; //OK
    } else {
        config.ap.max_connection = ESP_WIFI_MAX_CONN_NUM;
    }

    // hidden ssid
    if (cnt != para_num) {
        if(esp_at_get_para_as_digit (cnt++,&ssid_hidden) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if((ssid_hidden != 0) && (ssid_hidden != 1)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        config.ap.ssid_hidden = (uint8_t)ssid_hidden; //OK
    } else {
        config.ap.ssid_hidden = 0;
    }


    if(para_num != cnt)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(4,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((mode == WIFI_MODE_STA) || (mode == WIFI_MODE_NULL))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_wifi_set_config(WIFI_IF_AP,&config) != ESP_OK) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0x0));
        return ESP_AT_RESULT_CODE_FAIL;
    }

    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCwsapDef(uint8_t para_num)
{
    uint8_t ret = 0;
    esp_wifi_set_storage(WIFI_STORAGE_FLASH);
    ret = at_setupCmdCwsap(para_num);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    return ret;
}

#ifdef AT_LIST_IF_SUPPORT
uint8_t at_exeCmdCwlif(uint8_t* cmd_name)
{
    wifi_sta_list_t *station = NULL;
    tcpip_adapter_sta_list_t *ip_list = NULL;
	uint32_t loop = 0;
    uint8_t* temp = NULL;
    wifi_mode_t mode = WIFI_MODE_NULL;

    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    if(mode == WIFI_MODE_STA)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_ERROR;
    }

	station = (wifi_sta_list_t *)at_malloc(sizeof(wifi_sta_list_t));

	if (station == NULL) {
		return ESP_AT_RESULT_CODE_FAIL;
	}

    if (esp_wifi_ap_get_sta_list(station) != ESP_OK) {
		at_free(station);
        return ESP_AT_RESULT_CODE_FAIL;
    }

	ip_list = (tcpip_adapter_sta_list_t *)at_malloc(sizeof(tcpip_adapter_sta_list_t));
	if (ip_list == NULL) {
		at_free(station);
        return ESP_AT_RESULT_CODE_FAIL;
	}

    if (tcpip_adapter_get_sta_list(station,ip_list) != ESP_OK) {
        at_free(ip_list);
		at_free(station);
        return ESP_AT_RESULT_CODE_FAIL;
    }
    temp = (uint8_t*)at_malloc(128);
    for(loop = 0;loop < ip_list->num;loop++)
    {
        at_sprintf(temp, IPSTR","MACSTR"\r\n",IP2STR(&ip_list->sta[loop].ip), MAC2STR(ip_list->sta[loop].mac));
        at_port_print(temp);
    }
	at_free(ip_list);
    at_free(station);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}
#endif


uint8_t get_dhcp_status(void)
{
    uint8_t dhcp_mode = 0x0;
    tcpip_adapter_dhcp_status_t dhcp_status;

    if (tcpip_adapter_dhcpc_get_status(TCPIP_ADAPTER_IF_STA,&dhcp_status) != ESP_OK) {
        return dhcp_mode;
    }

    if ((dhcp_status == TCPIP_ADAPTER_DHCP_STARTED) 
		|| (dhcp_status == TCPIP_ADAPTER_DHCP_INIT))
    {
        dhcp_mode |= 0x01;
    }

    if (tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_AP,&dhcp_status) != ESP_OK) {
        return dhcp_mode;
    }
    if ((dhcp_status == TCPIP_ADAPTER_DHCP_STARTED) 
		|| (dhcp_status == TCPIP_ADAPTER_DHCP_INIT))
    {
        dhcp_mode |= 0x02;
    }

    return dhcp_mode;
}

#ifdef AT_DHCP_SUPPORT
uint8_t at_queryCmdCwdhcp(uint8_t* cmd_name)
{
    uint8_t temp[64];
    uint8_t dhcp_mode = 0x0;
    at_nvm_struct nvm;

    if (at_wifi_cmd_read_from_flash_flag) {
        if (at_nvm_dhcp_config_get(&nvm.dhcp_config) == true) {
            dhcp_mode = nvm.dhcp_config.dhcp_mode;
        }
    } else {
        dhcp_mode = get_dhcp_status();
    }

    at_sprintf(temp,"%s:%d", cmd_name,dhcp_mode);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

bool at_set_dhcp(uint8_t dhcp_enable,uint32_t dhcp_mode)
{
    bool ret = true;
    esp_err_t err = ESP_OK;

    switch (dhcp_enable) {
        case 0:
            if (dhcp_mode & 0x01) {     // station
                err = tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
                if ((err != ESP_OK) && (err != ESP_ERR_TCPIP_ADAPTER_DHCP_ALREADY_STOPPED)) {
                    ret = false;
                }
            }

            if (dhcp_mode & 0x02) { //softap
                err = tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
                if ((err != ESP_OK) && (err != ESP_ERR_TCPIP_ADAPTER_DHCP_ALREADY_STOPPED)) {
                    ret = false;
                }
            }
            break;
        case 1:
            if (dhcp_mode & 0x01) {     // station
                err = tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
                if ((err != ESP_OK) && (err != ESP_ERR_TCPIP_ADAPTER_DHCP_ALREADY_STARTED)) {
                    ret = false;
                }
            }

            if (dhcp_mode & 0x02) { //softap
                err = tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
                if ((err != ESP_OK) && (err != ESP_ERR_TCPIP_ADAPTER_DHCP_ALREADY_STARTED)) {
                    ret = false;
                }
            }
            break;
        default:
            ret = false;
            break;
    }    

    return ret;
}

uint8_t at_setupCmdCwdhcp(uint8_t para_num)
{
    int32_t mode = 0,opt = 0;
    int8_t  ret = 0;
    uint8_t cnt = 0;
    tcpip_adapter_ip_info_t adapter_ip_info;
    at_nvm_struct nvm;

    at_memset(&adapter_ip_info,0x00,sizeof(adapter_ip_info));

    if(para_num != 2)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    // opt
    if(esp_at_get_para_as_digit (cnt++,&opt) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if((opt != 0) && (opt != 1))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // mode
    if(esp_at_get_para_as_digit (cnt++,&mode) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if((mode > 0x03) || (mode < 0x01))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    ret = at_set_dhcp(opt,mode);
    mode = get_dhcp_status();

    if((mode & 0x01) != 0x01)  // sta dhcp disable,we should set static ip
    {
        at_memset(&nvm,0x0,sizeof(nvm));
        nvm.ip_config.ip_addr = 0;
        nvm.ip_config.gateway = 0;
        nvm.ip_config.netmask = 0;
        if (at_nvm_station_ip_config_get(&nvm.ip_config) == false) {
            at_printf("dhcp disable and no static ip!\r\n");
            // return ESP_AT_RESULT_CODE_FAIL;
        }

        if ((nvm.ip_config.ip_addr != IPADDR_NONE) && (nvm.ip_config.ip_addr != IPADDR_ANY)) {
            if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &adapter_ip_info) == ESP_OK){
                adapter_ip_info.ip.addr = nvm.ip_config.ip_addr;
                adapter_ip_info.gw.addr = nvm.ip_config.gateway;
                adapter_ip_info.netmask.addr = nvm.ip_config.netmask;

                tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &adapter_ip_info);
            }
        }
    }

    if(ret) {
        if(at_wifi_cmd_need_save_flag) {
            at_memset(&nvm,0x0,sizeof(nvm));
            nvm.dhcp_config.dhcp_mode = mode;
            if (at_nvm_dhcp_config_set(&nvm.dhcp_config) == false) {
                return ESP_AT_RESULT_CODE_FAIL;
            }
        }
        return ESP_AT_RESULT_CODE_OK;
    }
    else
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_FAIL;
    }
}

uint8_t at_queryCmdCwdhcpDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCwdhcp(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

uint8_t at_setupCmdCwdhcpDef(uint8_t para_num)
{
    uint8_t ret = 0;
    at_wifi_cmd_need_save_flag = TRUE;
    ret = at_setupCmdCwdhcp(para_num);
    at_wifi_cmd_need_save_flag = FALSE;
    return ret;
}

uint8_t at_queryCmdCwdhcps(uint8_t* cmd_name)
{
    tcpip_adapter_dhcps_lease_t lease;
    uint32_t lease_time = 0;
    at_nvm_struct nvm;
    
    if (at_wifi_cmd_read_from_flash_flag) {
        at_memset(&nvm,0x0,sizeof(nvm));
        if (at_nvm_dhcps_config_get(&nvm.dhcps_config) == true) {
            lease.enable = nvm.dhcps_config.enable;
            lease.start_ip.addr = nvm.dhcps_config.start_ip;
            lease.end_ip.addr = nvm.dhcps_config.end_ip;
            lease_time = nvm.dhcps_config.lease_time;
        } else {
            at_memset(&lease,0x0,sizeof(lease));
        }
    } else {
        if (tcpip_adapter_dhcps_option(TCPIP_ADAPTER_OP_GET,TCPIP_ADAPTER_IP_ADDRESS_LEASE_TIME,&lease_time,sizeof(lease_time)) != ESP_OK) {
            return ESP_AT_RESULT_CODE_FAIL;
        }

        if (tcpip_adapter_dhcps_option(TCPIP_ADAPTER_OP_GET,TCPIP_ADAPTER_REQUESTED_IP_ADDRESS,&lease,sizeof(lease)) != ESP_OK) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }

    uint8_t* temp = (uint8_t*)at_malloc(64);
    at_sprintf(temp, "%s:%u," IPSTR "," IPSTR, cmd_name, lease_time,
            IP2STR(&lease.start_ip),IP2STR(&lease.end_ip));
    at_port_print(temp);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCwdhcps(uint8_t para_num)
{
    int32_t lease_time = 0,enable = 0;
    tcpip_adapter_dhcps_lease_t lease;
    tcpip_adapter_dhcp_status_t status = TCPIP_ADAPTER_DHCP_INIT;
    uint8_t cnt = 0;
    uint8_t* pStr = NULL;
    at_nvm_struct nvm;
    wifi_mode_t mode = WIFI_MODE_NULL;

    at_memset(&lease,0x0,sizeof(lease));
    if (para_num == 0) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (esp_at_get_para_as_digit (cnt++,&enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

	
    if(enable == 1) {
        if (para_num != 4) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // lease time
        if (esp_at_get_para_as_digit (cnt++, &lease_time) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if ((lease_time <= 0) || (lease_time > 2880)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        // start ip
        if (esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        lease.start_ip.addr = ipaddr_addr((char*)pStr);
        if((lease.start_ip.addr == IPADDR_NONE) || (lease.start_ip.addr == IPADDR_ANY)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        //end ip
        if (esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        lease.end_ip.addr = ipaddr_addr((char*)pStr);
        if((lease.end_ip.addr == IPADDR_NONE) || (lease.end_ip.addr == IPADDR_ANY)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        lease.enable = TRUE;

        esp_wifi_get_mode(&mode);
        if ((mode != WIFI_MODE_AP) && (mode != WIFI_MODE_APSTA)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    } else if(enable == 0){
        lease.enable = false;
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(para_num != cnt) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(cnt,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_AP,&status);

    if(status == TCPIP_ADAPTER_DHCP_STARTED) {
        if(ESP_OK != tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP)) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }
    if (tcpip_adapter_dhcps_option(TCPIP_ADAPTER_OP_SET,TCPIP_ADAPTER_REQUESTED_IP_ADDRESS,&lease,sizeof(lease)) != ESP_OK) {
        if(status == TCPIP_ADAPTER_DHCP_STARTED) {
            tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
        }
        return ESP_AT_RESULT_CODE_FAIL;
    }

    if (tcpip_adapter_dhcps_option(TCPIP_ADAPTER_OP_SET,TCPIP_ADAPTER_IP_ADDRESS_LEASE_TIME,&lease_time,sizeof(lease_time)) != ESP_OK) {
        if(status == TCPIP_ADAPTER_DHCP_STARTED) {
            tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
        }
        return ESP_AT_RESULT_CODE_FAIL;
    }
    if(status == TCPIP_ADAPTER_DHCP_STARTED) {
        if(ESP_OK != tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP)) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }

    if (at_wifi_cmd_need_save_flag) {
        nvm.dhcps_config.enable = enable;
        nvm.dhcps_config.start_ip = lease.start_ip.addr;
        nvm.dhcps_config.end_ip = lease.end_ip.addr;
        nvm.dhcps_config.lease_time = lease_time;

        if (at_nvm_dhcps_config_set(&nvm.dhcps_config) == false) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }

    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCwdhcpsDef(uint8_t* cmd_name)
{
    uint8_t ret;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCwdhcps(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

uint8_t at_setupCmdCwdhcpsDef(uint8_t para_num)
{
    uint8_t ret;
    at_wifi_cmd_need_save_flag = TRUE;
    ret = at_setupCmdCwdhcps(para_num);
    at_wifi_cmd_need_save_flag = FALSE;
    return ret;
}
#endif
uint8_t at_queryCmdCipstamac(uint8_t* cmd_name)
{
    uint8_t* temp = NULL;
    uint8_t mac[6];
    at_nvm_struct nvm;
    
    at_memset(&nvm,0x0,sizeof(nvm));
    if(at_wifi_cmd_read_from_flash_flag)
    {
        if (at_nvm_station_mac_config_get(&nvm.mac_conig) == false) {
            at_memset(mac,0x0,sizeof(mac));
        } else {
            at_memcpy(mac,nvm.mac_conig.mac,sizeof(mac));
        }
    }
    else
    {
        esp_wifi_get_mac(WIFI_IF_STA, mac);
    }

    temp = (uint8_t*)at_malloc(64);
    at_sprintf(temp, "%s:\""MACSTR"\"\r\n", cmd_name,MAC2STR(mac));
    at_port_print(temp);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipstamac(uint8_t para_num)
{
    uint8_t len = 0;
    uint8_t loop = 0;
    uint8_t cnt = 0;
    uint8_t* pStr = NULL;
    uint8_t sta_mac[6];
    at_nvm_struct nvm;

    if(para_num != 1)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_memset(&sta_mac,0x0,sizeof(sta_mac));
    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    len = at_strlen(pStr);
    if(len != 17)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(at_str_2_mac(pStr,sta_mac) != TRUE)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_printf(MACSTR"\r\n", MAC2STR(sta_mac));
    for(loop = 0;loop < 6;loop++)
    {
        if(sta_mac[loop] != 0xFF)  // FF:FF:FF:FF:FF:FF
        {
            break;
        }
    }
    if(loop == 0)
    {
        for(loop = 0;loop < 6;loop++)
        {
            if(sta_mac[loop] != 0x00)  // 00:00:00:00:00:00
            {
                break;
            }
        }
    }
    if(loop >= 6)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(esp_wifi_set_mac(WIFI_IF_STA, sta_mac) == ESP_OK)
    {
        if(at_wifi_cmd_need_save_flag)
        {
            at_memset(&nvm,0x0,sizeof(nvm));
            at_memcpy(&nvm.mac_conig.mac,sta_mac,sizeof(nvm.mac_conig.mac));
            if (at_nvm_station_mac_config_set(&nvm.mac_conig) == false) {
                return ESP_AT_RESULT_CODE_FAIL;
            }
        }
        return ESP_AT_RESULT_CODE_OK;
    }
    else
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_FAIL;
    }
}

uint8_t at_queryCmdCipstamacDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCipstamac(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

uint8_t at_setupCmdCipstamacDef(uint8_t para_num)
{
    uint8_t ret = 0;
    at_wifi_cmd_need_save_flag = TRUE;
    ret = at_setupCmdCipstamac(para_num);
    at_wifi_cmd_need_save_flag = FALSE;
    return ret;
}

uint8_t at_queryCmdCipapmac(uint8_t* cmd_name)
{
    uint8_t* temp = NULL;
    uint8_t mac[6];
    at_nvm_struct nvm;

    at_memset(&nvm,0x0,sizeof(nvm));
    if(at_wifi_cmd_read_from_flash_flag)
    {
        if (at_nvm_softap_mac_config_get(&nvm.mac_conig) == false) {
            at_memset(mac,0x0,sizeof(mac));
        } else {
            at_memcpy(mac,nvm.mac_conig.mac,sizeof(mac));
        }
    }
    else
    {
        esp_wifi_get_mac(WIFI_IF_AP, mac);
    }
    temp = (uint8_t*)at_malloc(64);
    at_sprintf(temp, "%s:\""MACSTR"\"\r\n", cmd_name,MAC2STR(mac));
    at_port_print(temp);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipapmac(uint8_t para_num)
{
    uint8_t len = 0;
    uint8_t loop = 0;
    uint8_t cnt = 0;
    uint8_t* pStr = NULL;
    uint8_t ap_mac[6];
    at_nvm_struct nvm;

    if(para_num != 1)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_memset(&ap_mac,0x0,sizeof(ap_mac));
    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    len = at_strlen(pStr);
    if(len != 17)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(at_str_2_mac(pStr,ap_mac) != TRUE)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_printf(MACSTR"\r\n", MAC2STR(ap_mac));
    for(loop = 0;loop < 6;loop++)
    {
        if(ap_mac[loop] != 0xFF)  // FF:FF:FF:FF:FF:FF
        {
            break;
        }
    }
    if(loop == 0)
    {
        for(loop = 0;loop < 6;loop++)
        {
            if(ap_mac[loop] != 0x00)  // 00:00:00:00:00:00
            {
                break;
            }
        }
    }
    if(loop >= 6)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(esp_wifi_set_mac(WIFI_IF_AP, ap_mac) == ESP_OK)
    {
        if(at_wifi_cmd_need_save_flag)
        {
            at_memset(&nvm,0x0,sizeof(nvm));
            at_memcpy(nvm.mac_conig.mac,ap_mac,sizeof(ap_mac));
            if (at_nvm_softap_mac_config_set(&nvm.mac_conig) != true) {
                return ESP_AT_RESULT_CODE_FAIL;
            }
        }
        return ESP_AT_RESULT_CODE_OK;
    }
    else
    {
        return ESP_AT_RESULT_CODE_FAIL;
    }
}


uint8_t at_queryCmdCipapmacDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCipapmac(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

uint8_t at_setupCmdCipapmacDef(uint8_t para_num)
{
    uint8_t ret = 0;
    at_wifi_cmd_need_save_flag = TRUE;
    ret = at_setupCmdCipapmac(para_num);
    at_wifi_cmd_need_save_flag = FALSE;
    return ret;
}


uint8_t at_queryCmdCipsta(uint8_t* cmd_name)
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t* temp = NULL;
    at_nvm_struct nvm;
    at_memset(&adapter_ip_info,0x0,sizeof(adapter_ip_info));
    if(at_wifi_cmd_read_from_flash_flag)
    {
        at_memset(&nvm,0x0,sizeof(nvm));
        if (at_nvm_station_ip_config_get(&nvm.ip_config) == true) {
            adapter_ip_info.ip.addr = nvm.ip_config.ip_addr;
            adapter_ip_info.gw.addr = nvm.ip_config.gateway;
            adapter_ip_info.netmask.addr = nvm.ip_config.netmask;
        }
    }
    else
    {
        tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA,&adapter_ip_info);
    }

    temp = (uint8_t*)at_malloc(64);
    at_sprintf(temp, "%s:ip:\""IPSTR"\"\r\n", cmd_name,IP2STR(&adapter_ip_info.ip));
    at_port_print(temp);
    at_sprintf(temp, "%s:gateway:\""IPSTR"\"\r\n", cmd_name,IP2STR(&adapter_ip_info.gw));
    at_port_print(temp);
    at_sprintf(temp, "%s:netmask:\""IPSTR"\"\r\n", cmd_name,IP2STR(&adapter_ip_info.netmask));
    at_port_print(temp);
    at_free(temp);

    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipsta(uint8_t para_num)
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t  len = 0;
    uint8_t cnt = 0;
    uint8_t* pStr = NULL;
    uint32_t dhcp_mode = 0;
    at_nvm_struct nvm;

    at_memset(&adapter_ip_info,0x0,sizeof(adapter_ip_info));
    // ip
    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    len = at_strlen(pStr);
    if((len == 0) || (len > 15))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    adapter_ip_info.ip.addr = ipaddr_addr((char*)pStr);
    if ((adapter_ip_info.ip.addr == ((uint32_t)0xFFFFFFFFUL)) || (adapter_ip_info.ip.addr == ((uint32_t)0x0UL)))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_printf(IPSTR"\r\n",IP2STR(&adapter_ip_info.ip));

    // gateway
    if (cnt != para_num) {
        if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        len = at_strlen(pStr);
        if((len == 0) || (len > 15))
        {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        adapter_ip_info.gw.addr = ipaddr_addr((char*)pStr);


        if ((adapter_ip_info.gw.addr == ((uint32_t)0xFFFFFFFFUL)) || (adapter_ip_info.gw.addr == ((uint32_t)0x0UL))) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }

    }

    if ((adapter_ip_info.gw.addr == 0x0) && (adapter_ip_info.ip.addr != 0x0)) {
        adapter_ip_info.gw.addr = ((adapter_ip_info.ip.addr & ~(((uint32_t)0xFF) << 24)) | (0x01 << 24));
    }

    // netmask
    if (cnt != para_num) {
        if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        len = at_strlen(pStr);
        if((len == 0) || (len > 15))
        {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        adapter_ip_info.netmask.addr = ipaddr_addr((char*)pStr);
    }
    if ((adapter_ip_info.netmask.addr == 0x0) && (adapter_ip_info.ip.addr != 0x0)) {
        adapter_ip_info.netmask.addr = 0xFFFFFFFF & ~(0xFF000000);
    }
    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
    if(ESP_OK != tcpip_adapter_set_ip_info(WIFI_IF_STA, &adapter_ip_info))
    {
        tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(at_wifi_cmd_need_save_flag)
    {
        // dhcp config
        dhcp_mode = get_dhcp_status();

        at_memset(&nvm,0x0,sizeof(nvm));
        nvm.dhcp_config.dhcp_mode = dhcp_mode;
        if (at_nvm_dhcp_config_set(&nvm.dhcp_config) != true) {
            return ESP_AT_RESULT_CODE_FAIL;
        }

        // sta.ip ap.gw ap.mask
        at_memset(&nvm,0x0,sizeof(nvm));
        nvm.ip_config.ip_addr = adapter_ip_info.ip.addr;
        nvm.ip_config.gateway = adapter_ip_info.gw.addr;
        nvm.ip_config.netmask = adapter_ip_info.netmask.addr;
        if (at_nvm_station_ip_config_set(&nvm.ip_config) != true) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCipstaDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCipsta(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

uint8_t at_setupCmdCipstaDef(uint8_t para_num)
{
    uint8_t ret = 0;
    at_wifi_cmd_need_save_flag = TRUE;
    ret = at_setupCmdCipsta(para_num);
    at_wifi_cmd_need_save_flag = FALSE;
    return ret;
}

uint8_t at_queryCmdCipap(uint8_t* cmd_name)
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t* temp = 0;
    at_nvm_ip_config_struct nvm_ip;

    at_memset(&adapter_ip_info,0x0,sizeof(adapter_ip_info));
    if(at_wifi_cmd_read_from_flash_flag)
    {
        if (at_nvm_softap_ip_config_get(&nvm_ip)) {
            adapter_ip_info.gw.addr = nvm_ip.gateway;
            adapter_ip_info.ip.addr = nvm_ip.ip_addr;
            adapter_ip_info.netmask.addr = nvm_ip.netmask;
        }
    }
    else
    {
        tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info);
    }

    temp = (uint8_t*)at_malloc(64);
    at_sprintf(temp, "%s:ip:\""IPSTR"\"\r\n", cmd_name,IP2STR(&adapter_ip_info.ip));
    at_port_print(temp);
    at_sprintf(temp, "%s:gateway:\""IPSTR"\"\r\n", cmd_name,IP2STR(&adapter_ip_info.gw));
    at_port_print(temp);
    at_sprintf(temp, "%s:netmask:\""IPSTR"\"\r\n", cmd_name,IP2STR(&adapter_ip_info.netmask));
    at_port_print(temp);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipap(uint8_t para_num)
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t  len = 0;
    uint8_t cnt = 0;
    uint8_t* pStr = NULL;
    uint32_t dhcp_mode = 0;
    at_nvm_struct nvm;

    at_memset(&adapter_ip_info,0x0,sizeof(adapter_ip_info));

    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    len = at_strlen(pStr);
    if((len == 0) || (len > 32))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info);
    adapter_ip_info.ip.addr = ipaddr_addr((char*)pStr);

    if((adapter_ip_info.ip.addr == ((uint32_t)0xffffffffUL))
            || (adapter_ip_info.ip.addr == ((uint32_t)0x00000000UL)))
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_printf(IPSTR"\r\n",IP2STR(&adapter_ip_info.ip));

    // gateway
    if (cnt != para_num) {
        if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        len = at_strlen(pStr);
        if((len == 0) || (len > 15))
        {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        adapter_ip_info.gw.addr = ipaddr_addr((char*)pStr);


        if ((adapter_ip_info.gw.addr == ((uint32_t)0xFFFFFFFFUL)) || (adapter_ip_info.gw.addr == ((uint32_t)0x0UL))) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }

    }

    if ((adapter_ip_info.gw.addr == 0x0) && (adapter_ip_info.ip.addr != 0x0)) {
        adapter_ip_info.gw.addr = ((adapter_ip_info.ip.addr & ~(((uint32_t)0xFF) << 24)) | (0x01 << 24));
    }

    // netmask
    if (cnt != para_num) {
        if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        len = at_strlen(pStr);
        if((len == 0) || (len > 15))
        {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        adapter_ip_info.netmask.addr = ipaddr_addr((char*)pStr);
    }
    if ((adapter_ip_info.netmask.addr == 0x0) && (adapter_ip_info.ip.addr != 0x0)) {
        adapter_ip_info.netmask.addr = 0xFFFFFFFF & ~(0xFF000000);
    }
    if (cnt != para_num) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(cnt,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
    if(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info) != ESP_OK)
    {
        tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(0));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);

    if(at_wifi_cmd_need_save_flag)
    {
        // dhcp config
        dhcp_mode = get_dhcp_status();

        at_memset(&nvm,0x0,sizeof(nvm));
        nvm.dhcp_config.dhcp_mode = dhcp_mode;
        if (at_nvm_dhcp_config_set(&nvm.dhcp_config) != true) {
            return ESP_AT_RESULT_CODE_FAIL;
        }

        // ap.ip ap.gw ap.mask
        at_memset(&nvm,0x0,sizeof(nvm));
        nvm.ip_config.ip_addr = adapter_ip_info.ip.addr;
        nvm.ip_config.gateway = adapter_ip_info.gw.addr;
        nvm.ip_config.netmask = adapter_ip_info.netmask.addr;
        if (at_nvm_softap_ip_config_set(&nvm.ip_config) != true) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCipapDef(uint8_t* cmd_name)
{
    uint8_t ret = 0;
    at_wifi_cmd_read_from_flash_flag = TRUE;
    ret = at_queryCmdCipap(cmd_name);
    at_wifi_cmd_read_from_flash_flag = FALSE;
    return ret;
}

uint8_t at_setupCmdCipapDef(uint8_t para_num)
{
    uint8_t ret = 0;
    at_wifi_cmd_need_save_flag = TRUE;
    ret = at_setupCmdCipap(para_num);
    at_wifi_cmd_need_save_flag = FALSE;
    return ret;
}

uint8_t at_queryCmdCwAutoConn(uint8_t* cmd_name)
{
    uint8_t temp[64];
    bool enable = false;
    if (esp_wifi_get_auto_connect(&enable) != ESP_OK) {
        at_printf( "esp_wifi_get_auto_connect fail\r\n");
        return ESP_AT_RESULT_CODE_FAIL;
    }
    at_sprintf(temp, "%s:%d", cmd_name,enable);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCwAutoConn(uint8_t para_num)
{
    int32_t data = 0;
    uint32_t cnt = 0;
    bool enable = false;
    esp_err_t ret = ESP_FAIL;

    if(para_num != 1)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (cnt++,&data) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (data == 0) {
        enable = false;
    } else if (data == 1) {
        enable = true;
    } else {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    esp_wifi_set_storage(WIFI_STORAGE_FLASH);
    ret = esp_wifi_set_auto_connect(enable);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    
    if(ret == ESP_OK)
    {
        return ESP_AT_RESULT_CODE_OK;
    }
    else
    {
        return ESP_AT_RESULT_CODE_FAIL;
    }
}

#ifdef AT_SMARTCONFIG_SUPPORT
void smartconfig_done(smartconfig_status_t status,void *data)
{
    smartconfig_type_t sc = SC_TYPE_ESPTOUCH;
    switch (status) {
    case SC_STATUS_GETTING_SSID_PSWD:
        if(data == NULL) {
            return;
        }
        sc = *(smartconfig_type_t*)data;
        if(sc == SC_TYPE_ESPTOUCH) {
            at_port_print((uint8_t*)"smartconfig type:ESPTOUCH\r\n");
        } else if(sc == SC_TYPE_AIRKISS) {
            at_port_print((uint8_t*)"smartconfig type:AIRKISS\r\n");
        } else {
            at_port_print((uint8_t*)"smartconfig type:UNKNOWN\r\n");
        }
        break;
    case SC_STATUS_LINK:
    {
        wifi_config_t sta_conf;

        memset(&sta_conf, 0x00, sizeof(sta_conf));
        uint8_t* tmp = (uint8_t*)at_malloc(64);

        esp_wifi_disconnect();
    
        esp_wifi_get_config(WIFI_IF_STA, &sta_conf);
        at_memcpy(&(sta_conf.sta), data, sizeof(wifi_sta_config_t));
        at_port_print((uint8_t*)"Smart get wifi info\r\n");
        at_sprintf(tmp,"ssid:%s\r\n", (char*)(sta_conf.sta.ssid));
        at_port_print(tmp);
        at_sprintf(tmp,"password:%s\r\n", (char*)(sta_conf.sta.password));
        at_port_print(tmp);
    
        if (esp_wifi_set_config(WIFI_IF_STA,&sta_conf) != ESP_OK) {
            at_port_print((uint8_t*)"smartconfig set config fail\r\n");
        }
        if(esp_wifi_connect() != ESP_OK) {
            at_port_print((uint8_t*)"smartconfig connect fail\r\n");
        }
        at_free(tmp);
    }
    break;
    case SC_STATUS_LINK_OVER:
        at_port_print((uint8_t*)"smartconfig connected wifi\r\n");
        break;
    case SC_STATUS_WAIT:
        at_printf("%s","smart wait\r\n");
        break;
    case SC_STATUS_FIND_CHANNEL:
        at_printf("%s","smart find channel\r\n");
        break;
    }
}

uint8_t at_exeCmdSmartStart(uint8_t* cmd_name)
{
    if (esp_smartconfig_set_type(SC_TYPE_ESPTOUCH_AIRKISS) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }

    if (esp_smartconfig_start(smartconfig_done) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }

    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdSmartStart(uint8_t para_num)
{
    int result = 0;
    smartconfig_type_t sc = SC_TYPE_ESPTOUCH;

    if (para_num != 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (0,&result) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    // digit

    if(result == 1) {
        sc = SC_TYPE_ESPTOUCH;
    } else if(result == 2) {
        sc = SC_TYPE_AIRKISS;
    } else if(result == 3) {
        sc = SC_TYPE_ESPTOUCH_AIRKISS;
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // esp_wifi_disconnect();

    if (esp_smartconfig_set_type(sc) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }

    if(esp_smartconfig_start(smartconfig_done) != ESP_OK) {
        return ESP_AT_RESULT_CODE_FAIL;
    }

    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_exeCmdSmartStop(uint8_t* cmd_name)
{
    if(esp_smartconfig_stop() == ESP_OK) {
        return ESP_AT_RESULT_CODE_OK;
    } else {
        return ESP_AT_RESULT_CODE_FAIL;
    }
}
#endif

#ifdef AT_WPS_SUPPORT
// AT+WPS=start/stop (1/0)
uint8_t at_setupCmdWps(uint8_t para_num)
{
    wifi_mode_t opmode = WIFI_MODE_NULL;
    int32_t enable = 0;

    if (esp_wifi_get_mode(&opmode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (opmode != WIFI_MODE_STA) {
        at_port_print((uint8_t*)"opmode mismatch when wps\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (1 != para_num) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (0,&enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(enable == 1) {
        esp_wifi_wps_disable();
        if(esp_wifi_wps_enable(WPS_TYPE_PBC) != ESP_OK) {
            at_port_print((uint8_t*)"wps ebable fail\r\n");
            return ESP_AT_RESULT_CODE_FAIL;
        }

        if(esp_wifi_wps_start(0) != ESP_OK) {
            at_port_print((uint8_t*)"wps start fail\r\n");
            return ESP_AT_RESULT_CODE_FAIL;
        }
        at_port_print((uint8_t*)"wps started\r\n");
    } else if(enable == 0) {
        if(esp_wifi_wps_disable() != ESP_OK) {
            at_port_print((uint8_t*)"wps stop fail\r\n");
            return ESP_AT_RESULT_CODE_FAIL;
        }
        at_port_print((uint8_t*)"wps stoped\r\n");
    } else {
        at_port_print((uint8_t*)"wps command error\r\n");
        return ESP_AT_RESULT_CODE_FAIL;
    }

    return ESP_AT_RESULT_CODE_OK;
}
#endif


uint8_t at_queryCmdCwHostname(uint8_t* cmd_name)
{
	uint8_t* buf = NULL;
	uint8_t* hostname = NULL;

	buf = (uint8_t*)at_malloc(64);

	if (buf) {
		if (tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_STA,(const char**)&hostname) != ESP_OK) {
			at_free(buf);
			return ESP_AT_RESULT_CODE_FAIL;
		}

		at_snprintf(buf,64,"%s:%s\r\n",cmd_name,hostname);
		at_port_print(buf);
		at_free(buf);
		return ESP_AT_RESULT_CODE_OK;
	}


	return ESP_AT_RESULT_CODE_FAIL;
}


uint8_t at_setupCmdCwHostname(uint8_t para_num)
{
	uint8_t* hostname = NULL;
	if (1 != para_num) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

	if(esp_at_get_para_as_str (0,&hostname) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

	if ((hostname == NULL) || (at_strlen(hostname) == 0)) {
		return ESP_AT_RESULT_CODE_ERROR;
	}

	if (tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA,(char*)hostname) == ESP_OK) {
		return ESP_AT_RESULT_CODE_OK;
	}

	return ESP_AT_RESULT_CODE_FAIL;
}

uint8_t at_queryCmdCipsntpcfg(uint8_t* cmd_name)
{
	uint8_t* buf = NULL;
	uint8_t* p = NULL;
	uint8_t loop = 0;
	bool flag = false;
	int32_t timezone = 0;

	buf = (uint8_t*)at_malloc(32*3+32);

	if (buf) {
		p = (uint8_t*)getenv("TZ");
		if (p) {
			p = (uint8_t*)at_strstr(p,"GMT");

			if (p) {
				p += 3;
				timezone = atoi((char*)p);
			}
		}
		at_sprintf(buf,"%s:1,%d",cmd_name,timezone);
		
		for (loop = 0;loop < SNTP_MAX_SERVERS;loop++) {
			p = (uint8_t*)sntp_getservername(loop);

			if (p && at_strlen(p)) {
				at_sprintf(buf + at_strlen(buf),",\"%s\"",p);
				flag = true;
			}
		}

		if (flag == false) {
			at_sprintf(buf,"%s:0\r\n",cmd_name);
		} else {
			at_sprintf(buf + at_strlen(buf),"%s","\r\n");
		}

		at_port_print(buf);
		at_free(buf);
		return ESP_AT_RESULT_CODE_OK;
	}


	return ESP_AT_RESULT_CODE_FAIL;
}

static uint8_t* at_sntp_server[SNTP_MAX_SERVERS];
uint8_t at_setupCmdCipsntpcfg(uint8_t para_num)
{
    int32_t timezone = 0;
    uint32_t cnt = 0;
    uint8_t* server[SNTP_MAX_SERVERS];
    uint8_t count = 0;
    uint8_t* p = NULL;
    uint8_t buf[8];
	
    at_memset(server,0x0,sizeof(server));
    if(para_num == cnt)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (cnt++,&timezone) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

	if ((timezone < -11) || (timezone > 13)) {
		return ESP_AT_RESULT_CODE_ERROR;
	}

	for(count = 0;count < SNTP_MAX_SERVERS;count++) {
	    if(para_num != cnt) {
	    	// sntp server
	        if(esp_at_get_para_as_str (cnt++,&p) != ESP_AT_PARA_PARSE_RESULT_OK) {
		        return ESP_AT_RESULT_CODE_ERROR;
		    }

			if ((p == NULL) || (at_strlen(p) == 0) || (at_strlen(p) > 32)) {
				return ESP_AT_RESULT_CODE_ERROR;
			}

			server[count] = p;
	    } else {
			break;
		}
	}

    if (cnt != para_num) {
		return ESP_AT_RESULT_CODE_ERROR;
	}

	if (count == 0) {
#if (SNTP_MAX_SERVERS > 0)
		server[0] = (uint8_t*)"cn.ntp.org.cn";
#endif

#if (SNTP_MAX_SERVERS > 1)
			server[1] = (uint8_t*)"ntp.sjtu.edu.cn";
#endif

#if (SNTP_MAX_SERVERS > 2)
			server[2] = (uint8_t*)"us.pool.ntp.org";
#endif
	}
	
	at_snprintf(buf,sizeof(buf),"GMT%+d",timezone);

	sntp_stop();
    setenv("TZ", (char*)buf, 1);
	tzset();
	
    for(count = 0;count < SNTP_MAX_SERVERS;count++) {
        if (at_sntp_server[count] != NULL) {
            at_free(at_sntp_server[count]);
            at_sntp_server[count] = NULL;
        }

        if (server[count] != NULL) {
            at_sntp_server[count] = at_malloc(at_strlen(server[count]) + 1);
            
            if (at_sntp_server[count] == NULL) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            
            at_strcpy(at_sntp_server[count],server[count]);
        }

        sntp_setservername(count,(char*)at_sntp_server[count]);
	}
	sntp_init();

	return ESP_AT_RESULT_CODE_OK;
	
}

uint8_t at_exeCmdCipsntpcfg(uint8_t* cmd_name)
{
	uint8_t count = 0;
    sntp_stop();
	for(count = 0;count < SNTP_MAX_SERVERS;count++) {
        if (at_sntp_server[count] != NULL) {
            at_free(at_sntp_server[count]);
            at_sntp_server[count] = NULL;
        }
        sntp_setservername(count,"");
	}

	return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCipsntptime(uint8_t* cmd_name)
{
	uint8_t* buf = NULL;
	time_t now;    
	struct tm timeinfo;

	time(&now);    
	localtime_r(&now, &timeinfo);

	buf = (uint8_t*)at_malloc(64);

	if (buf) {
		at_sprintf(buf,"%s:%s",cmd_name,asctime(&timeinfo));
		at_port_print(buf);
		
		at_free(buf);
		return ESP_AT_RESULT_CODE_OK;
	}


	return ESP_AT_RESULT_CODE_FAIL;
}

/**
 * @}
 */
