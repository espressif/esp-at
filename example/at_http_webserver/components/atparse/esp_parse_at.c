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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_system.h"
#include "esp_parse_at.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "ParseAt";

// Considering that the BLE scan implement asynchronously, we need to
// get the list after BLE scan return success.
extern ble_scan_info* at_ble_link_head;

esp_err_t esp_at_join_ap(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (strlen(arg->cwjap.pwd) == 0 || strlen(arg->cwjap.ssid) == 0) {
            ESP_LOGE(TAG, "Error pwd or ssid\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_CWJAP, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_QUERTCWJAP, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        if (at_result->data == NULL) {
            memset(arg, '\0', sizeof(esp_at_arg));
        } else {
            memcpy(arg, (esp_at_arg*)(at_result->data), sizeof(esp_at_arg));
        }

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_auto_connect(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;

    if (method < 0 || method > 1) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        at_result = at_add_cmd(AT_AUTOCONN, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_AUTOCONN, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        memcpy(arg, (esp_at_arg*)(at_result->data), sizeof(esp_at_arg));

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_close_ap(void)
{
    at_response* at_result;
    at_result = at_add_cmd(AT_CWQAP, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}


esp_err_t esp_at_set_ap(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (strlen(arg->cwsap.pwd) == 0 || strlen(arg->cwsap.ssid) == 0 || arg->cwsap.chl < 0 || arg->cwsap.ecn < 0) {
            ESP_LOGE(TAG, "Error parameter\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_CWSAP, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_QUERTCWSAP, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        memcpy(arg, (esp_at_arg*)(at_result->data), sizeof(esp_at_arg));

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_set_mac(esp_wifi_mode mode, esp_at_arg* arg)
{
    at_response* at_result = NULL;

    if (strlen(arg->cipmac.mac) == 0) {
        ESP_LOGE(TAG, "Error arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (mode < 0 || mode > 1) {
        ESP_LOGE(TAG, "Error WiFi mode arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (mode == STATION) {
        at_result = at_add_cmd(AT_CIPSTAMAC, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }
    } else if (mode == SOFTAP) {
        at_result = at_add_cmd(AT_CIPAPMAC, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }
    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

esp_err_t esp_at_wifi_scan(wifi_scan_cb cb)
{
    uint32_t n = 0;
    at_response* at_result = NULL;
    scan_ap_link* dev_head = NULL;

    scan_ap_link* ptr_pre = NULL;
    scan_ap_link* scan_ptr = NULL;

    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;

    char ech[5] = {0};
    char rssi[5] = {0};
    char channel[5] = {0};
    at_result = at_add_cmd(AT_CWLAP, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGD(TAG, "Scan info:%s", (char*)at_result->data);

    char* p = (char*)at_result->data;

    // parse ip status
    while (strstr(p, "+CWLAP:") != NULL) {
        n++;
        scan_ptr = malloc(sizeof(scan_ap_link));
        assert(scan_ptr);
        memset(scan_ptr, '\0', sizeof(scan_ap_link));

        if (n == 1) {
            dev_head = scan_ptr;
            ptr_pre = scan_ptr;
        }

        // encryption method
        ip_start_ptr = strstr(p, "+CWLAP:") + 8;   // skip +CWLAP:(
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(ech, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        scan_ptr->ecn =  atoi(ech);

        // ssid
        ip_start_ptr = ip_end_ptr + 2;  // skip \"
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(scan_ptr->ssid, ip_start_ptr, ip_end_ptr - ip_start_ptr - 1); // skip \"

        // rssi
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(rssi, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        scan_ptr->rssi = atoi(rssi);

        // mac
        ip_start_ptr = ip_end_ptr + 2;  // skip \"
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(scan_ptr->mac, ip_start_ptr, ip_end_ptr - ip_start_ptr - 1); // skip \"

        // channel
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, "\r\n");
        memcpy(channel, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        scan_ptr->channel = atoi(channel);

        memset(ech, '\0', sizeof(ech));
        memset(rssi, '\0', sizeof(rssi));
        memset(channel, '\0', sizeof(channel));
        p = ip_end_ptr;

        if (n != 1) {
            ptr_pre->next = scan_ptr;
            ptr_pre = scan_ptr;
        }
    }

    at_cmd_free(at_result);

    cb(dev_head);

    return ESP_OK;
}

esp_err_t esp_at_set_dhcps(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (arg->cwdhcps.enable < 0 || arg->cwdhcps.lease < 0 || strlen(arg->cwdhcps.start_ip) == 0 || strlen(arg->cwdhcps.end_ip) == 0) {
            ESP_LOGE(TAG, "Error parameter\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_CWDHCPS, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_QUERTDHCPS, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        memcpy(arg, (esp_at_arg*)(at_result->data), sizeof(esp_at_arg));

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_get_ip(esp_wifi_mode mode, esp_at_ip_info* at_ip_info)
{
    at_response* at_result = NULL;

    if (mode < STATION || mode > SOFTAP) {
        ESP_LOGE(TAG, "Error mode arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    memset(at_ip_info, '\0', sizeof(esp_at_ip_info));

    at_result = at_add_cmd(AT_CIFSR, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    if (mode == STATION) {
        memcpy(at_ip_info->ip, ((cifsr_info*)at_result->data)->sta_ip, sizeof(at_ip_info->ip));
        memcpy(at_ip_info->mac, ((cifsr_info*)at_result->data)->sta_mac, sizeof(at_ip_info->mac));
        at_cmd_free(at_result);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        at_result = at_add_cmd(AT_QUERTCIPSTA, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }
    } else if (mode == SOFTAP) {
        memcpy(at_ip_info->ip, ((cifsr_info*)at_result->data)->ap_ip, sizeof(at_ip_info->ip));
        memcpy(at_ip_info->mac, ((cifsr_info*)at_result->data)->ap_mac, sizeof(at_ip_info->mac));
        at_cmd_free(at_result);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        at_result = at_add_cmd(AT_QUERTCIPAP, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }
    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_ARG;
    }

    memcpy(at_ip_info->gateway, ((ip_info*)at_result->data)->gateway, sizeof(at_ip_info->gateway));
    memcpy(at_ip_info->netmask, ((ip_info*)at_result->data)->netmask, sizeof(at_ip_info->netmask));

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_set_ip(esp_wifi_mode mode, esp_at_arg* arg)
{
    at_response* at_result = NULL;
    at_cmd_arg cmd_arg;

    if (strlen(arg->ipinfo.ip) == 0) {
        ESP_LOGE(TAG, "Error arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (mode < 0 || mode > 1) {
        ESP_LOGE(TAG, "Error WiFi mode arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (mode == STATION) {
        cmd_arg.cipsta.ip = arg->ipinfo.ip;
        cmd_arg.cipsta.gateway = arg->ipinfo.gateway;
        cmd_arg.cipsta.netmask = arg->ipinfo.netmask;
        at_result = at_add_cmd(AT_CIPSTA, &cmd_arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }
    } else if (mode == SOFTAP) {
        cmd_arg.cipap.ip = arg->ipinfo.ip;
        cmd_arg.cipap.gateway = arg->ipinfo.gateway;
        cmd_arg.cipap.netmask = arg->ipinfo.netmask;
        at_result = at_add_cmd(AT_CIPAP, &cmd_arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }
    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

conn_dev_link* esp_at_connected_device(void)
{
    conn_dev_link* conn_dev_head = NULL;
    conn_dev_link* conn_dev_ptr = NULL;
    conn_dev_link* ptr_pre = NULL;
    uint8_t n = 0;
    char* p = NULL;
    at_response* at_result = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;

    at_result = at_add_cmd(AT_CWLIF, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return NULL;
    }

    if (at_result->data == NULL) {
        ESP_LOGI(TAG, "No connected device");
        return NULL;
    }

    p = (char*)at_result->data;


    while (strstr(p, "+CWLIF") != NULL) {
        n += 1;
        conn_dev_ptr = malloc(sizeof(conn_dev_link));

        if (conn_dev_ptr == NULL) {
            ESP_LOGE(TAG, "malloc error");
            return NULL;
        }

        memset(conn_dev_ptr, '\0', sizeof(conn_dev_link));

        if (n == 1) {
            conn_dev_head = conn_dev_ptr;
            ptr_pre = conn_dev_ptr;
        }

        ip_start_ptr = strstr(p, "+CWLIF") + 7;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(conn_dev_ptr->ip, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, "\r\n");
        memcpy(conn_dev_ptr->mac, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        p = ip_end_ptr;

        if (n != 1) {
            ptr_pre->next = conn_dev_ptr;
            ptr_pre = conn_dev_ptr;
        }
    }

    conn_dev_ptr->next = NULL;
    return conn_dev_head;

}

esp_err_t esp_at_restart(void)
{
    at_response* at_result;
    at_result = at_add_cmd(AT_RST, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_restore(void)
{
    at_response* at_result;
    at_result = at_add_cmd(AT_RESTORE, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_establish_connect(esp_at_arg* arg)
{
    at_response* at_result;
    at_result = at_add_cmd(AT_CIPSTART, (at_cmd_arg*)arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_close_connect(int8_t link_id)
{
    at_response* at_result;
    at_cmd_arg cmd_arg;

    if (link_id < 0) {
        ESP_LOGE(TAG, "error parameter");
        return ESP_ERR_INVALID_ARG;
    }

    cmd_arg.cipclose.link_id = link_id;
    at_result = at_add_cmd(AT_CIPCLOSE, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

conn_stat* esp_at_get_connect_status(void)
{
    at_response* at_result;
    uint8_t n = 0;
    conn_stat* st = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;
    char* p = NULL;

    conn_status_link* status_link_ptr = NULL;
    conn_status_link* ptr_pre = NULL;
    char ip_stat[10] = {0};
    char link_id[10] = {0};
    char remote_port[10] = {0};
    char local_port[10] = {0};
    char tetype[10] = {0};

    at_result = at_add_cmd(AT_CIPSTATUS, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return NULL;
    }

    st = malloc(sizeof(conn_stat));
    assert(st);
    p = (char*)at_result->data;
    ip_start_ptr = strstr(p, "STATUS:") + 7;
    ip_end_ptr = strstr(ip_start_ptr, "\r\n");
    memcpy(ip_stat, ip_start_ptr, ip_end_ptr - ip_start_ptr);

    st->interface_status = atoi(ip_stat);

    if (st->interface_status != CREAT_TRAN) {
        ESP_LOGI(TAG, "No establish TCP/UDP transmission");
        st->status_link = NULL;
        return st;
    }

    // parse ip status
    while (strstr(p, "+CIPSTATUS:") != NULL) {
        n++;
        status_link_ptr = malloc(sizeof(conn_status_link));
        assert(status_link_ptr);
        memset(status_link_ptr, '\0', sizeof(conn_status_link));

        if (n == 1) {
            st->status_link = status_link_ptr;
            ptr_pre = status_link_ptr;
        }

        // link_id
        ip_start_ptr = strstr(p, "+CIPSTATUS:") + 11;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(link_id, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        status_link_ptr->link_id =  atoi(link_id);

        // type
        ip_start_ptr = ip_end_ptr + 2;  // skip \"
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(status_link_ptr->type, ip_start_ptr, ip_end_ptr - ip_start_ptr - 1); // skip \"

        // remote_ip
        ip_start_ptr = ip_end_ptr + 2; // skip \"
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(status_link_ptr->remote_ip, ip_start_ptr, ip_end_ptr - ip_start_ptr - 1); // skip \"

        // remote port
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(remote_port, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        status_link_ptr->remote_port = atoi(remote_port);

        // local port
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(local_port, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        status_link_ptr->local_port = atoi(local_port);

        // tetype
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, "\r\n");
        memcpy(tetype, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        status_link_ptr->tetype = atoi(tetype);

        memset(link_id, '\0', sizeof(link_id));
        memset(remote_port, '\0', sizeof(remote_port));
        memset(local_port, '\0', sizeof(local_port));
        memset(tetype, '\0', sizeof(tetype));
        p = ip_end_ptr;

        if (n != 1) {
            ptr_pre->next = status_link_ptr;
            ptr_pre = status_link_ptr;
        }
    }

    status_link_ptr->next = NULL;
    return st;
}

esp_err_t esp_at_dhcp_status(esp_at_method method, esp_dhcp_status* dhcp_status)
{
    at_response* at_result;
    at_cmd_arg cmd_arg;
    esp_dhcp_status current_status;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;
    char dhcp_st[10] = {0};

    if (method == QUERY) {
        ESP_LOGI(TAG, "Query DHCP");
        at_result = at_add_cmd(AT_QUERTDHCP, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        ip_start_ptr = strstr((char*)at_result->data, "+CWDHCP:") + 8;

        // This is a AT core problem, it not end with \r\n.
        if ((ip_end_ptr = strstr(ip_start_ptr, "\r\n")) == NULL) {
            ip_end_ptr = strstr(ip_start_ptr, "\0");
        }

        memcpy(dhcp_st, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        ESP_LOGI(TAG, "dhcp status:%s", dhcp_st);
        *dhcp_status = atoi(dhcp_st) & 0x3;    // Only need the last 2 bit

    } else if (method == SET) {

        esp_err_t ret = esp_at_dhcp_status(QUERY, &current_status);

        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Query dhcp status error");
            return ESP_ERR_INVALID_STATE;
        }

        ESP_LOGI(TAG, "current status:%d", current_status);

        /*
        *  This may need reconsider.
        */
        cmd_arg.cwdhcp.operate = 1;
        cmd_arg.cwdhcp.mode = *dhcp_status;
        at_result = at_add_cmd(AT_CWDHCP, &cmd_arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

int32_t esp_at_getramsize(void)
{
    at_response* at_result;
    int32_t sysram_size = 0;
    at_result = at_add_cmd(AT_SYSRAM, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return -1;
    }

    sysram_size = atoi((char*)at_result->data);
    at_cmd_free(at_result);
    return sysram_size;
}

int32_t esp_at_ping(char* addr)
{
    int32_t ping_time = 0;
    at_cmd_arg cmd_arg;
    at_response* at_result;

    if (addr  == NULL) {
        ESP_LOGE(TAG, "Error argument");
        return -2;
    }

    cmd_arg.ping.ip = addr;

    at_result = at_add_cmd(AT_PING, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        if (at_result->data != NULL) {
            if (strstr((char*)(at_result->data), "TIMEOUT") != NULL) {
                ESP_LOGI(TAG, "Ping timeout");
                at_cmd_free(at_result);
                return 0xffff;
            }
        } else {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return -1;
        }
    }

    ESP_LOGD(TAG, "Ping time:%s", (char*)at_result->data);

    ping_time = atoi((char*)at_result->data + 6);
    at_cmd_free(at_result);

    return ping_time;
}

int32_t esp_at_fatfs_open(char* filename)
{
    int32_t file_len = 0;
    at_response* at_result;
    at_cmd_arg cmd_arg;

    if (filename == NULL) {
        ESP_LOGE(TAG, "File name is NULL\n");
        return -1;
    }

    cmd_arg.fsopen.filename = filename;
    at_result = at_add_cmd(AT_FSOPEN, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return -1;
    }

    file_len = atoi((char*)at_result->data);
    at_cmd_free(at_result);
    return file_len;
}

char* esp_at_fatfs_read(char* filename, uint32_t offset, uint32_t length)
{
    int32_t read_len = 0;
    char* read_data = NULL;
    char* tmp_data = NULL;
    at_response* at_result;
    at_cmd_arg cmd_arg;

    if (filename == NULL) {
        ESP_LOGE(TAG, "File name is NULL\n");
        return NULL;
    }

    cmd_arg.fsread.filename = filename;
    cmd_arg.fsread.offset = offset;
    cmd_arg.fsread.length = length;
    at_result = at_add_cmd(AT_FSREAD, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return NULL;
    }

    // parse response data  "+FS:len,XXXXX"
    read_data = strstr((char*)at_result->data, "+FS:");

    tmp_data = strchr(read_data, ',');
    *tmp_data = '\0';
    read_len = atoi((read_data + 4));

    if (read_len != length) {
        ESP_LOGI(TAG, "Read len:%d, len:%d\n", read_len, length);
    }

    read_data = malloc(read_len + 1);
    memcpy(read_data, tmp_data + 1, read_len);
    at_cmd_free(at_result);
    return read_data;
}

esp_err_t esp_at_ble_init(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;
    char init_status[10] = {0};

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (arg->bleinit.init_mode < 0 || arg->bleinit.init_mode > 2) {
            ESP_LOGE(TAG, "Error parameter\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_BLEINIT, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_QUERTBLEINIT, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        ESP_LOGI(TAG, "Ble init status:%s", (char*)(at_result->data));
        ip_start_ptr = strstr((char*)(at_result->data), "+BLEINIT:") + 9;
        ip_end_ptr = strstr((char*)(at_result->data), "\r\n");
        memcpy(init_status, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        ESP_LOGI(TAG, "Ble init status:%d", atoi(init_status));
        arg->bleinit.init_mode = atoi(init_status);

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_ble_address(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (arg->bleaddr.addr_type < 0 || arg->bleaddr.addr_type > 1 || strlen(arg->bleaddr.random_addr) == 0) {
            ESP_LOGE(TAG, "Error parameter\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_BLEADDR, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_QUERTBLEADDR, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        ESP_LOGI(TAG, "Ble addr:%s", (char*)(at_result->data));
        ip_start_ptr = strstr((char*)(at_result->data), "+BLEADDR:") + 9;
        ip_end_ptr = strstr((char*)(at_result->data), "\r\n");
        memcpy(arg->bleaddr.random_addr, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        arg->bleaddr.addr_type = 1;    // random
    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}


esp_err_t esp_at_ble_scan(esp_at_arg* arg, ble_scan_cb cb)
{
    at_response* at_result = NULL;

    //uint32_t wait_time = 0;
    if (arg->blescan.interval == -1 || arg->blescan.interval == 0) {
        arg->blescan.interval = 1;
    }

    arg->blescan.interval = 1; // Just is the temporary test

    at_result = at_add_cmd(AT_BLESCAN, (at_cmd_arg*)arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    vTaskDelay(1100 / portTICK_RATE_MS);   // wait some time

    cb(at_ble_link_head);

    return ESP_OK;
}

esp_err_t esp_at_ble_name(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (strlen(arg->blename.device_name) == 0) {
            ESP_LOGE(TAG, "Error parameter\n");
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_BLENAME, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        memset(arg, '\0', sizeof(esp_at_arg));
        at_result = at_add_cmd(AT_QUERTBLENAME, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        ESP_LOGI(TAG, "Ble name:%s", (char*)(at_result->data));
        ip_start_ptr = strstr((char*)(at_result->data), "+BLENAME:") + 9;
        ip_end_ptr = strstr((char*)(at_result->data), "\r\n");
        memcpy(arg->blename.device_name, ip_start_ptr, ip_end_ptr - ip_start_ptr);

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_ble_scan_param(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;

    char scan_type[10] = {0};
    char addr_type[10] = {0};
    char filter_policy[10] = {0};
    char scan_interval[10] = {0};
    char scan_window[10] = {0};

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        if (arg->blescanparam.scan_window > arg->blescanparam.scan_interval) {
            ESP_LOGE(TAG, "scan window :%d cannot large than scan interval:%d", arg->blescanparam.scan_window, arg->blescanparam.scan_interval);
            return ESP_ERR_INVALID_ARG;
        }

        at_result = at_add_cmd(AT_BLESCANPARAM, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        memset(arg, '\0', sizeof(esp_at_arg));
        at_result = at_add_cmd(AT_QUERTBLESCANPARAM, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        ESP_LOGI(TAG, "Ble scan param:%s", (char*)(at_result->data));
        ip_start_ptr = strstr((char*)(at_result->data), "+BLESCANPARAM:") + 14;
        ip_end_ptr = strstr((char*)(at_result->data), ",");
        memcpy(scan_type, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(addr_type, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(filter_policy, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(scan_interval, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, "\r\n");
        memcpy(scan_window, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        arg->blescanparam.scan_type = atoi(scan_type);
        arg->blescanparam.addr_type = atoi(addr_type);
        arg->blescanparam.filter_policy = atoi(filter_policy);
        arg->blescanparam.scan_interval = atoi(scan_interval);
        arg->blescanparam.scan_window = atoi(scan_window);

    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_ble_adv_param(esp_at_method method, esp_at_arg* arg)
{
    at_response* at_result = NULL;
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;

    char adv_int_min[10] = {0};
    char adv_int_max[10] = {0};
    char adv_type[10] = {0};
    char addr_type[10] = {0};
    char channel[10] = {0};
    char adv_filter_policy[10] = {0};
    char peer_addr_type[10] = {0};
    // char peer_address[30]={0};

    if (method < QUERY || method > SET) {
        ESP_LOGE(TAG, "Error method arg\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (method == SET) {
        at_result = at_add_cmd(AT_BLEADVPARAM, (at_cmd_arg*)arg);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        at_cmd_free(at_result);
    } else if (method == QUERY) {
        if (arg == NULL) {
            ESP_LOGE(TAG, "Error arg\n");
            return ESP_ERR_INVALID_ARG;
        }

        memset(arg, '\0', sizeof(esp_at_arg));
        at_result = at_add_cmd(AT_QUERTBLEADVPARAM, NULL);

        if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
            ESP_LOGE(TAG, "command execution error");
            at_cmd_free(at_result);
            return ESP_ERR_INVALID_STATE;
        }

        ESP_LOGI(TAG, "Ble adv param:%s", (char*)(at_result->data));
        ip_start_ptr = strstr((char*)(at_result->data), "+BLEADVPARAM:") + 13;
        ip_end_ptr = strstr((char*)(at_result->data), ",");
        memcpy(adv_int_min, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        ESP_LOGD(TAG, "adv_int_min:%s", adv_int_min);
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(adv_int_max, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(adv_type, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(addr_type, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        ESP_LOGD(TAG, "addr_type:%s", addr_type);
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(channel, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(adv_filter_policy, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(peer_addr_type, ip_start_ptr, ip_end_ptr - ip_start_ptr);

        ip_start_ptr = ip_end_ptr + 1;
        strncpy(arg->bleadvparam.peer_address, ip_start_ptr, sizeof(arg->bleadvparam.peer_address));
        ESP_LOGD(TAG, "peer_address:%s\n", arg->bleadvparam.peer_address);
        arg->bleadvparam.adv_int_min = atoi(adv_int_min);
        arg->bleadvparam.adv_int_max = atoi(adv_int_max);
        arg->bleadvparam.adv_type = atoi(adv_type);
        arg->bleadvparam.addr_type = atoi(addr_type);
        arg->bleadvparam.channel = atoi(channel);
        arg->bleadvparam.adv_filter_policy = atoi(adv_filter_policy);
        at_cmd_free(at_result);
    } else {
        ESP_LOGE(TAG, "Unsupport method\n");
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

esp_err_t esp_at_ble_set_adv_data(char* adv_data)
{
    at_response* at_result = NULL;
    at_cmd_arg arg;

    if (adv_data == NULL) {
        ESP_LOGE(TAG, "Error adv data\n");
        return ESP_ERR_INVALID_ARG;
    }

    arg.bleadvdata.adv_data = adv_data;

    at_result = at_add_cmd(AT_BLEADVDATA, &arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_ble_adv_start()
{
    at_response* at_result = NULL;
    at_result = at_add_cmd(AT_BLEADVSTART, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_ble_adv_stop()
{
    at_response* at_result = NULL;
    at_result = at_add_cmd(AT_BLEADVSTOP, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_ble_connect(esp_at_arg* arg)
{
    at_response* at_result = NULL;
    at_result = at_add_cmd(AT_BLECONN, (at_cmd_arg*)arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

ble_conn_dev* esp_at_get_ble_connect(void)
{
    at_response* at_result = NULL;
    ble_conn_dev* conn_dev_head = NULL;
    char conn_id[5] = {0};
    char* ip_start_ptr = NULL;
    char* ip_end_ptr = NULL;
    at_result = at_add_cmd(AT_QUERTBLECONN, NULL);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return NULL;
    }

    ESP_LOGI(TAG, "Ble connect:%s", (char*)(at_result->data));

    ip_start_ptr = strstr((char*)(at_result->data), "+BLECONN:") + 9;
    ip_end_ptr = strstr(ip_start_ptr, "\r\n");

    if (ip_start_ptr == ip_end_ptr) {
        ESP_LOGI(TAG, "NO ble connect");
        return NULL;
    }

    ble_conn_dev* conn_dev = malloc(sizeof(ble_conn_dev));
    assert(conn_dev);
    memset(conn_dev, 0, sizeof(ble_conn_dev));
    ip_end_ptr = strstr(ip_start_ptr, ",");
    memcpy(conn_id, ip_start_ptr, ip_end_ptr - ip_start_ptr);
    ESP_LOGI(TAG, "Conn_id:%s\n", conn_id);
    conn_dev->conn_index = atoi(conn_id);
    ip_start_ptr = ip_end_ptr + 1;
    ip_end_ptr = strstr(ip_start_ptr, "\r\n");
    memcpy(conn_dev->addr, ip_start_ptr, ip_end_ptr - ip_start_ptr);

    conn_dev->next = NULL;

    ip_start_ptr = ip_end_ptr + 2;

    // Only one ble connect dev
    if ((ip_start_ptr = strstr(ip_start_ptr, "+BLECONN:")) == NULL) {
        return conn_dev;
    }

    // over 1 connection , using link
    conn_dev_head = conn_dev;
    ble_conn_dev* conn_pre = conn_dev;

    while ((ip_start_ptr = strstr(ip_start_ptr, "+BLECONN:")) != NULL) {
        ble_conn_dev* conn_dev = malloc(sizeof(ble_conn_dev));
        assert(conn_dev);
        memset(conn_dev, 0, sizeof(ble_conn_dev));
        ip_start_ptr += 9;
        ip_end_ptr = strstr(ip_start_ptr, ",");
        memcpy(conn_id, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        ESP_LOGI(TAG, "Link Conn_id:%s\n", conn_id);
        conn_dev->conn_index = atoi(conn_id);
        ip_start_ptr = ip_end_ptr + 1;
        ip_end_ptr = strstr(ip_start_ptr, "\r\n");
        memcpy(conn_dev->addr, ip_start_ptr, ip_end_ptr - ip_start_ptr);
        conn_dev->next = NULL;
        ip_start_ptr = ip_end_ptr + 2;
        conn_pre->next = conn_dev;
        conn_pre = conn_dev;
    }

    return conn_dev_head;
}

esp_err_t esp_at_ble_close_connect(uint8_t conn_id)
{
    at_response* at_result = NULL;
    at_cmd_arg cmd_arg;
    cmd_arg.bledisconn.conn_id = conn_id;
    at_result = at_add_cmd(AT_BLECLOSECONN, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
    return ESP_OK;
}

esp_err_t esp_at_init_parse(void)
{
    at_cmd_arg cmd_arg;
    at_response* at_result = NULL;

    at_init_parse_frame();

// set station + softAP mode
    cmd_arg.cwmode.mode = 3;
    at_result = at_add_cmd(AT_CWMODE, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command cwmode execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);

// enable multiple connections
    cmd_arg.cipmux.mode = 1;
    at_result = at_add_cmd(AT_CIPMUX, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command cipmux execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);

#if 0
    /*
     *  Test connect AP first
     */
    memset(&cmd_arg, 0x0, sizeof(cmd_arg));
    strcpy(cmd_arg.cwjap.ssid , "HUAWEI001");
    strcpy(cmd_arg.cwjap.pwd , "12345678");
    at_result = at_add_cmd(AT_CWJAP, &cmd_arg);

    if (at_result->rsp_flag != AT_CMD_RETURN_SUCCESS) {
        ESP_LOGE(TAG, "command cipmux execution error");
        at_cmd_free(at_result);
        return ESP_ERR_INVALID_STATE;
    }

    at_cmd_free(at_result);
#endif

    ESP_LOGI(TAG, "init AT parse frame seccess\n");
    return ESP_OK;
}
