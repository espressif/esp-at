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

#include "parseframe.h"
#include "esp_parse_at.h"

static const char* TAG = "CmdParse";

/* Some AT command have multi response
 * For example : AT+ CIFSR
 *  Response: +CIFSR:APIP,<SoftAP IP address>
 *            +CIFSR:APMAC,<SoftAP MAC address>
 *            +CIFSR:STAIP,<Station IP address>
 *            +CIFSR:STAMAC,<Station MAC address>
 *            OK
 *
 * This flag help us handle multiple response
 */
static bool at_multi_rsp_flag = false;

// Scan result maybe huge, just need MAX_SCAN_COUNT
static uint8_t at_scan_count = 0;

// Check whether it is BLE query command
static bool at_ble_connect_flag = false;

// Resonese massage
static at_response at_rsp_msg;

// response callback
extern rsp_cb response_cb;

// reponse queue
extern xQueueHandle at_msg_queue;

// BLE scan result
ble_scan_info* at_ble_link_head = NULL;

// The previous BLE scan results link
ble_scan_info* scan_pre = NULL;

static void at_sendinfo_rsp(char* at_rsp, size_t len)
{
    if (strstr(at_rsp, "SEND OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(at_rsp, "ERROR") != NULL || strstr(at_rsp, "SEND FAIL") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_cipsend_rsp(char* at_rsp, size_t len)
{
    if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_rsp_msg.data = NULL;
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }

    if (strstr(at_rsp, ">") != NULL) {
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_comm_rsp(char* at_rsp, size_t len)
{
    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_wait_rsp(char* at_rsp, size_t len)
{
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "CONNECT") != NULL) {
        ESP_LOGI(TAG, "Receive a client");
    } else if (strstr(at_rsp, "CLOSED") != NULL) {
        ESP_LOGI(TAG, "Client closed");
    }
}

static void at_cwjap_rsp(char* at_rsp, size_t len)
{
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "+CWJAP:") != NULL) {
        at_rsp_msg.data = (char*)malloc(30 * sizeof(char));
        assert(at_rsp_msg.data);
        ESP_LOGI(TAG, "CWJAP :%s\n", at_rsp + 7);
        strcpy(at_rsp_msg.data, at_rsp + 7);
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }

}

static void at_cwlap_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if ((data = strstr(at_rsp, "+CWLAP:")) != NULL) {
        if (!at_multi_rsp_flag) {
            at_rsp_msg.data = (char*)malloc(10 * 1024 * sizeof(char));
            assert(at_rsp_msg.data);
            at_multi_rsp_flag = true;
            strcpy((char*)at_rsp_msg.data, data);
        } else {
            if (at_rsp_msg.data != NULL) {
                strcat((char*)at_rsp_msg.data, data);
            } else {
                printf("Error\n");
            }
        }
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(at_rsp, "OK\r\n") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_multi_rsp_flag = false;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

// AT+FS OPERN response parse
static void at_fsopen_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(at_rsp, "+FS:") != NULL) {
        at_rsp_msg.data = (char*)malloc(20 * sizeof(char));
        data = strstr(at_rsp, "+FS:");
        ESP_LOGD(TAG, "Recv file len:%s\n", data + 4);

        if (at_rsp_msg.data != NULL) {
            strcpy((char*)at_rsp_msg.data, data + 4);
        }
    } else if (strstr(at_rsp, "OK") != NULL) {

        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_fsread_rsp(char* at_rsp, size_t len)
{
    char cmp_data[21] = {0};

// No need to compare all data, the first 20 byte enough
    if (len > 20) {
        memcpy(cmp_data, at_rsp, 20);
    } else {
        memcpy(cmp_data, at_rsp, len);
    }

    ESP_LOGD(TAG, "%s receiv :%s,cmp_data: %s\n", __func__, at_rsp, cmp_data);

    if (strstr(cmp_data, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(cmp_data, "+FS:") != NULL) {
        at_rsp_msg.data = malloc((len + 1) * sizeof(char));
        memcpy((char*)at_rsp_msg.data, at_rsp, len);
    } else if (strstr(cmp_data, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_sysram_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+SYSRAM:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(20 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data + 8);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_cifsr_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CIFSR:")) != NULL) {
        start_ptr += 7;   // skip "+CIFSR:"
        end_ptr = strstr(at_rsp, "\r\n");

        if (strstr(start_ptr, "APIP") != NULL) {
            start_ptr += 6;   // skip "APIP,\""
            at_rsp_msg.data = (cifsr_info*)malloc(sizeof(cifsr_info));
            assert(at_rsp_msg.data);
            memset(at_rsp_msg.data, '\0', sizeof(cifsr_info));
            memcpy(((cifsr_info*)at_rsp_msg.data)->ap_ip, start_ptr, end_ptr - start_ptr - 1); // skip "\""
        } else if (strstr(start_ptr, "APMAC") != NULL) {
            start_ptr += 7;   // skip "APIP,\""
            memcpy(((cifsr_info*)at_rsp_msg.data)->ap_mac, start_ptr, end_ptr - start_ptr - 1);
        } else if (strstr(start_ptr, "STAIP") != NULL) {
            start_ptr += 7;   // skip "STAIP,\""
            memcpy(((cifsr_info*)at_rsp_msg.data)->sta_ip, start_ptr, end_ptr - start_ptr - 1);
        } else if (strstr(start_ptr, "STAMAC") != NULL) {
            start_ptr += 8;   // skip "STAIP,\""
            memcpy(((cifsr_info*)at_rsp_msg.data)->sta_mac, start_ptr, end_ptr - start_ptr - 1);
        }

    } else if (strstr(at_rsp, "OK") != NULL) {
        ESP_LOGD(TAG, "Cifsr cmd ip infor :%s\n", (char*)at_rsp_msg.data);
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_cwlif_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CWLIF:")) != NULL) {
        if (!at_multi_rsp_flag) {
            at_rsp_msg.data = (char*)malloc(400 * sizeof(char));
            assert(at_rsp_msg.data);
            at_multi_rsp_flag = true;
            strcpy((char*)at_rsp_msg.data, data);
        } else {
            strcat((char*)at_rsp_msg.data, data);
        }

    } else if (strstr(at_rsp, "OK") != NULL) {
        if (at_rsp_msg.data != NULL) {
            ESP_LOGI(TAG, "%s cmd ip infor :%s\n", __func__, (char*)at_rsp_msg.data);
        } else {
            ESP_LOGI(TAG, "No ip data");
        }

        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_multi_rsp_flag = false;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_ping_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGI(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        //at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+PING:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);

    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_cipstatus_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CIPSTATUS:")) != NULL) {  // In case compare "STATUS"
        strcat((char*)at_rsp_msg.data, data);
    } else if ((data = strstr(at_rsp, "STATUS:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(500 * sizeof(char));
        assert(at_rsp_msg.data);
        strcat((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

/*
* +CWJAP:<ssid>,<bssid>,<channel>,<rssi>
*/
static void at_queryjap_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char channel[5];
    char rssi[5];
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CWJAP:")) != NULL) {
        at_rsp_msg.data = (at_cmd_arg*)malloc(sizeof(at_cmd_arg));
        assert(at_rsp_msg.data);
        memset(at_rsp_msg.data, '\0', sizeof(at_cmd_arg));
        // copy ssid info
        start_ptr += 8;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)at_rsp_msg.data)->queryjap.ssid, start_ptr, end_ptr - start_ptr - 1);

        // copy bssid info
        start_ptr = end_ptr + 2;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)at_rsp_msg.data)->queryjap.bssid, start_ptr, end_ptr - start_ptr - 1);

        // copy channel info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(channel, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)at_rsp_msg.data)->queryjap.channel = atoi(channel);

        // copy rssi info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, "\r\n");
        memcpy(rssi, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)at_rsp_msg.data)->queryjap.rssi = atoi(rssi);

    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

// +CWDHCPS:<lease>,<start IP>,<end IP>
static void at_querydhcps_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char lease[10];

    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CWDHCPS:")) != NULL) {
        at_rsp_msg.data = (at_cmd_arg*)malloc(sizeof(at_cmd_arg));
        assert(at_rsp_msg.data);
        memset(at_rsp_msg.data, '\0', sizeof(at_cmd_arg));

        ((at_cmd_arg*)at_rsp_msg.data)->cwdhcps.enable = 1;   // +CWJAP means we have set dhcps

        // copy lease info
        start_ptr += 9;
        end_ptr = strstr(start_ptr, ",");

        memcpy(lease, start_ptr, end_ptr - start_ptr);

        ((at_cmd_arg*)at_rsp_msg.data)->cwdhcps.lease = atoi(lease);

        // copy start ip info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)at_rsp_msg.data)->cwdhcps.start_ip, start_ptr, end_ptr - start_ptr);

        // copy end ip info
        start_ptr = end_ptr + 1;

        if ((end_ptr = strstr(start_ptr, "\r\n")) == NULL) {
            end_ptr = strstr(start_ptr, "\0");
        }

        strncpy(((at_cmd_arg*)at_rsp_msg.data)->cwdhcps.end_ip, start_ptr, sizeof(((at_cmd_arg*)at_rsp_msg.data)->cwdhcps.end_ip));

    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

/*
* +CWSAP:<ssid>,<pwd>,<chl>,<ecn>,<max conn>,<ssid hidden>
*/
static void at_querysap_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char chl[5];
    char ecn[5];
    char max_conn[5];
    char ssid_hidden[5];
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CWSAP:")) != NULL) {
        at_rsp_msg.data = (at_cmd_arg*)malloc(sizeof(at_cmd_arg));
        assert(at_rsp_msg.data);
        memset(at_rsp_msg.data, '\0', sizeof(at_cmd_arg));
        // copy ssid info
        start_ptr += 8;  // skip +CWSAP:\"
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)at_rsp_msg.data)->cwsap.ssid, start_ptr, end_ptr - start_ptr - 1);

        // copy pwd info
        start_ptr = end_ptr + 2;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)at_rsp_msg.data)->cwsap.pwd, start_ptr, end_ptr - start_ptr - 1);

        // copy channel info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(chl, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)at_rsp_msg.data)->cwsap.chl = atoi(chl);

        // copy encryption info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(ecn, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)at_rsp_msg.data)->cwsap.ecn = atoi(ecn);

        // copy max connect info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(max_conn, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)at_rsp_msg.data)->cwsap.max_conn = atoi(max_conn);

        // copy ssid hidden info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, "\r\n");
        memcpy(ssid_hidden, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)at_rsp_msg.data)->cwsap.ssid_hidden = atoi(ssid_hidden);

    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_queryip_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (((start_ptr = strstr(at_rsp, "+CIPSTA:")) != NULL) || ((start_ptr = strstr(at_rsp, "+CIPAP:")) != NULL)) {
//	start_ptr += 7;    // skip 7 byte("+CIPAP:")
        end_ptr = strstr(at_rsp, "\r\n");

        if ((start_ptr = strstr(at_rsp, "ip:")) != NULL) {
            at_rsp_msg.data = (ip_info*)malloc(sizeof(ip_info));
            assert(at_rsp_msg.data);
            memset(at_rsp_msg.data, '\0', sizeof(ip_info));
            start_ptr += 4;
            memcpy(((ip_info*)at_rsp_msg.data)->ip, start_ptr, end_ptr - start_ptr - 1);
        } else if ((start_ptr = strstr(at_rsp, "gateway:")) != NULL) {
            start_ptr += 9;
            memcpy(((ip_info*)at_rsp_msg.data)->gateway, start_ptr, end_ptr - start_ptr - 1);
        } else if ((start_ptr = strstr(at_rsp, "netmask:")) != NULL) {
            start_ptr += 9;
            memcpy(((ip_info*)at_rsp_msg.data)->netmask, start_ptr, end_ptr - start_ptr - 1);
        }

    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_querydns_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CIPDNS:")) != NULL) {
        if (!at_multi_rsp_flag) {
            at_rsp_msg.data = (char*)malloc(200 * sizeof(char));
            assert(at_rsp_msg.data);
            at_multi_rsp_flag = true;
            strcpy((char*)at_rsp_msg.data, data);
        } else {
            strcat((char*)at_rsp_msg.data, data);
        }
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_multi_rsp_flag = false;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_querycwdhcp_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CWDHCP:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_multi_rsp_flag = false;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_querybleinit_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLEINIT:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_querybleaddr_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLEADDR:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_queryblename_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLENAME:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_saveblescanrst(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char rssi[10] = {0};
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "+BLESCAN:") != NULL && at_scan_count < MAX_SCAN_COUNT) {
        ble_scan_info* scan_info_ptr = (ble_scan_info*)malloc(sizeof(ble_scan_info));
        assert(scan_info_ptr);
        memset(scan_info_ptr, '\0', sizeof(ble_scan_info));

        // addr
        start_ptr = strstr(at_rsp, "+BLESCAN:") + 9;
        end_ptr = strstr(start_ptr, ",");
        memcpy(scan_info_ptr->addr, start_ptr, end_ptr - start_ptr);

        // rssi
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(rssi, start_ptr, end_ptr - start_ptr);
        scan_info_ptr->rssi = atoi(rssi);

        // adv_data
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(scan_info_ptr->adv_data, start_ptr, end_ptr - start_ptr);

        // scan_rsp_data
        start_ptr = end_ptr + 1;

        if ((end_ptr = strstr(start_ptr, "\r\n")) == NULL) { // some response data not end with "\r\n"
            end_ptr = strstr(start_ptr, "\0");         // '\0' must is the end
        }

        if (start_ptr != end_ptr) {
            memcpy(scan_info_ptr->scan_rsp_data, start_ptr, end_ptr - start_ptr);
        }

        ESP_LOGD(TAG, "scan info:addr: %s, rssi:%d, adv_data:%s, scan_rsp_data:%s\n", scan_info_ptr->addr, scan_info_ptr->rssi, scan_info_ptr->adv_data, scan_info_ptr->scan_rsp_data);

        if (!at_multi_rsp_flag) {
            at_ble_link_head = scan_info_ptr;
            scan_pre = scan_info_ptr;
            at_scan_count = 1;
            at_multi_rsp_flag = true;
        } else {
            ble_scan_info* scan_tmp = at_ble_link_head;

            while (scan_tmp) {
                if (strcmp(scan_tmp->addr, scan_info_ptr->addr) == 0) {
                    ESP_LOGD(TAG, "Repeat data address:%s, %s", scan_tmp->addr, scan_info_ptr->addr);
                    free(scan_info_ptr);
                    break;
                }

                scan_tmp = scan_tmp->next;
            }

            if (!scan_tmp) {   // Not repeat data
                scan_pre->next = scan_info_ptr;
                scan_pre = scan_info_ptr;
                at_scan_count ++;
            }
        }
    }
}

static void at_blescan_rsp(char* at_rsp, size_t len)
{

    if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        at_rsp_msg.data = NULL;
        at_multi_rsp_flag = false;    // rescan, init head point

        at_scan_count = 0;        // first scan or rescan, scan count reset
        response_cb = at_saveblescanrst;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_queryblescanparam_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLESCANPARAM:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(100 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

static void at_querybleconn_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "AT+BLECONN?") != NULL) {
        ESP_LOGI(TAG, "Query BLE connection");
        at_ble_connect_flag = true;
    }

    if (at_ble_connect_flag) {
        if (strstr(at_rsp, "ERROR") != NULL) {
            at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
            at_rsp_msg.data = NULL;
            xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
        } else if ((data = strstr(at_rsp, "+BLECONN:")) != NULL) {
            if (!at_multi_rsp_flag) {
                at_rsp_msg.data = (char*)malloc(200 * sizeof(char));
                assert(at_rsp_msg.data);
                at_multi_rsp_flag = true;
                strcpy((char*)at_rsp_msg.data, data);
            } else {
                strcat((char*)at_rsp_msg.data, data);
            }
        } else if (strstr(at_rsp, "OK") != NULL) {
            at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
            at_multi_rsp_flag = false;
            at_ble_connect_flag = false;
            xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
        }
    }
}

static void at_querybleadvparam_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        at_rsp_msg.data = NULL;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLEADVPARAM:")) != NULL) {
        at_rsp_msg.data = (char*)malloc(100 * sizeof(char));
        assert(at_rsp_msg.data);
        strcpy((char*)at_rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        at_rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(at_msg_queue, (void*)&at_rsp_msg, 0);
    }
}

/* ------------------------------PACK---------------------------------------------------  */
static int8_t at_cwmode_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[30];
    uint8_t cmd_len = 0;

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWMODE=%d\r\n", cmd_arg->cwmode.mode);
    ESP_LOGD(TAG, "%s command :%s\n", __func__, cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cwsap_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[200];
    uint8_t cmd_len = 0;

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWSAP=\"%s\",\"%s\",%d,%d", cmd_arg->cwsap.ssid, cmd_arg->cwsap.pwd, cmd_arg->cwsap.chl, cmd_arg->cwsap.ecn);

    if (cmd_arg->cwsap.max_conn != -1 && cmd_arg->cwsap.max_conn != 0) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",%d", cmd_arg->cwsap.max_conn);

        if (cmd_arg->cwsap.ssid_hidden != -1) {
            cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",%d", cmd_arg->cwsap.ssid_hidden);
        }
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGI(TAG, "cwsap command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipmux_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[30];
    uint8_t cmd_len = 0;
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPMUX=%d\r\n", cmd_arg->cipmux.mode);

    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipserver_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[100];
    uint8_t cmd_len = 0;
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSERVER=%d", cmd_arg->cipserver.mode);

    if (cmd_arg->cipserver.port != -1) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",%d", cmd_arg->cipserver.port);

        if (cmd_arg->cipserver.ssl != NULL) {
            cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cipserver.ssl);

            if (cmd_arg->cipserver.ca_enable != -1) {
                cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",%d", cmd_arg->cipserver.ca_enable);
            }
        }
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGI(TAG, "cipserver command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipsend_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[50];
    uint32_t cmd_len = 0;

    if (cmd_arg->cipsend.link_id == -1) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSEND=%d\r\n", cmd_arg->cipsend.length);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSEND=%d,%d\r\n", cmd_arg->cipsend.link_id, cmd_arg->cipsend.length);
    }

    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_sendinfo_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)cmd_arg->sendinfo.data, cmd_arg->sendinfo.data_len);
}

static int8_t at_cwjap_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[200];
    uint8_t cmd_len = 0;

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWJAP=\"%s\",\"%s\"", cmd_arg->cwjap.ssid, cmd_arg->cwjap.pwd);

    if (strlen(cmd_arg->cwjap.bssid) != 0) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cwjap.bssid);
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGI(TAG, "cwjap command :%s\n", cmd_tmp);

    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cwqap_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CWQAP\r\n", strlen("AT+CWQAP\r\n"));
}

static int8_t at_cwlap_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CWLAP\r\n", strlen("AT+CWLAP\r\n"));
}

static int8_t at_fsopen_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+FS=0,3,\"%s\"\r\n", cmd_arg->fsopen.filename);
    ESP_LOGD(TAG, "fsopen command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_fsread_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+FS=0,2,\"%s\",%d,%d\r\n", cmd_arg->fsread.filename, cmd_arg->fsread.offset, cmd_arg->fsread.length);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_sysram_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+SYSRAM?\r\n", strlen("AT+SYSRAM?\r\n"));
}

static int8_t at_cifsr_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CIFSR\r\n", strlen("AT+CIFSR\r\n"));
}

static int8_t at_cwlif_pack(at_cmd_arg* cmd_arg)
{
    at_rsp_msg.data = NULL;   // If no connect device, we need make sure no data
    return at_call_lib((uint8_t*)"AT+CWLIF\r\n", strlen("AT+CWLIF\r\n"));
}

static int8_t at_ping_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];
    at_rsp_msg.data = NULL;
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+PING=\"%s\"\r\n", cmd_arg->ping.ip);
    ESP_LOGI(TAG, "ping command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_autoconn_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWAUTOCONN=%d\r\n", cmd_arg->autoconn.enable);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cwdhcp_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWDHCP=%d,%d\r\n", cmd_arg->cwdhcp.operate, cmd_arg->cwdhcp.mode);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cwdhcps_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];

    if (cmd_arg->cwdhcps.enable == 0) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWDHCPS=%d\r\n", cmd_arg->cwdhcps.enable);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWDHCPS=%d,%d,\"%s\",\"%s\"\r\n", cmd_arg->cwdhcps.enable, cmd_arg->cwdhcps.lease, cmd_arg->cwdhcps.start_ip, cmd_arg->cwdhcps.end_ip);
    }

    ESP_LOGI(TAG, "cwdhcps command :%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipsta_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTA=\"%s\"", cmd_arg->cipsta.ip);

    if (cmd_arg->cipsta.gateway != NULL && (strlen(cmd_arg->cipsta.gateway) > 0)) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cipsta.gateway);

        if (cmd_arg->cipsta.netmask != NULL && (strlen(cmd_arg->cipsta.netmask) > 0)) {
            cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cipsta.netmask);
        }
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGI(TAG, "cipsta command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipap_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPAP=\"%s\"", cmd_arg->cipap.ip);

    if (cmd_arg->cipap.gateway != NULL && (strlen(cmd_arg->cipap.gateway) > 0)) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cipap.gateway);

        if (cmd_arg->cipap.netmask != NULL && (strlen(cmd_arg->cipap.netmask) > 0)) {
            cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cipap.netmask);
        }
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGD(TAG, "cipap command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_rst_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+RST\r\n", strlen("AT+RST\r\n"));
}

static int8_t at_restore_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
}

static int8_t at_cipstart_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];

    if (strcmp(cmd_arg->cipstart.type, "UDP") == 0) {
        if (cmd_arg->cipstart.udp_mode != -1 && cmd_arg->cipstart.local_port != -1) {
            cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d,%d\r\n", cmd_arg->cipstart.link_id, cmd_arg->cipstart.type, cmd_arg->cipstart.remote_ip, cmd_arg->cipstart.remote_port, cmd_arg->cipstart.local_port, cmd_arg->cipstart.udp_mode);
        } else {
            cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTART=%d,\"%s\",\"%s\",%d\r\n", cmd_arg->cipstart.link_id, cmd_arg->cipstart.type, cmd_arg->cipstart.remote_ip, cmd_arg->cipstart.remote_port);
        }

    } else if (strcmp(cmd_arg->cipstart.type, "TCP") == 0 || strcmp(cmd_arg->cipstart.type, "SSL") == 0) {
        if (cmd_arg->cipstart.keep_alive > 0) {
            cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d\r\n", cmd_arg->cipstart.link_id, cmd_arg->cipstart.type, cmd_arg->cipstart.remote_ip, cmd_arg->cipstart.remote_port, cmd_arg->cipstart.keep_alive);
        } else {
            cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTART=%d,\"%s\",\"%s\",%d\r\n", cmd_arg->cipstart.link_id, cmd_arg->cipstart.type, cmd_arg->cipstart.remote_ip, cmd_arg->cipstart.remote_port);
        }
    } else {
        ESP_LOGE(TAG, "Unknow type %s\n", cmd_arg->cipstart.type);
        return -1;
    }

    ESP_LOGI(TAG, "cipstart command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipclose_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[30];

    if (cmd_arg->cipclose.link_id != -1) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPCLOSE=%d\r\n", cmd_arg->cipclose.link_id);
    } else {
        strcpy(cmd_tmp, "AT+CIPCLOSE\r\n");
        cmd_len = strlen(cmd_tmp);
    }

    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipstatus_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CIPSTATUS\r\n", strlen("AT+CIPSTATUS\r\n"));
}

static int8_t at_querycipsta_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CIPSTA?\r\n", strlen("AT+CIPSTA?\r\n"));
}

static int8_t at_cipstamac_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTAMAC=\"%s\"\r\n", cmd_arg->cipmac.mac);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_cipapmac_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPAPMAC=\"%s\"\r\n", cmd_arg->cipmac.mac);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_querycipap_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CIPAP?\r\n", strlen("AT+CIPAP?\r\n"));
}

static int8_t at_querycwjap_pack(at_cmd_arg* cmd_arg)
{
    at_rsp_msg.data = NULL;   // If no connect AP, we need make sure no data
    return at_call_lib((uint8_t*)"AT+CWJAP?\r\n", strlen("AT+CWJAP?\r\n"));
}

static int8_t at_querycwsap_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CWSAP?\r\n", strlen("AT+CWSAP?\r\n"));
}

static int8_t at_querycipdns_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CIPDNS?\r\n", strlen("AT+CIPDNS?\r\n"));
}

static int8_t at_querycwdhcp_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CWDHCP?\r\n", strlen("AT+CWDHCP?\r\n"));
}

static int8_t at_querydhcps_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+CWDHCPS?\r\n", strlen("AT+CWDHCPS?\r\n"));
}

static int8_t at_querybleinit_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLEINIT?\r\n", strlen("AT+BLEINIT?\r\n"));
}

static int8_t at_bleinit_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEINIT=%d\r\n", cmd_arg->bleinit.init_mode);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_bleaddr_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADDR=%d,\"%s\"\r\n", cmd_arg->bleaddr.addr_type, cmd_arg->bleaddr.random_addr);

    ESP_LOGD(TAG, "Command :%s\n", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_blename_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLENAME=\"%s\"\r\n", cmd_arg->blename.device_name);

    ESP_LOGD(TAG, "Command :%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_blescanparam_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLESCANPARAM=%d,%d,%d,%d,%d\r\n", cmd_arg->blescanparam.scan_type, cmd_arg->blescanparam.addr_type, cmd_arg->blescanparam.filter_policy, cmd_arg->blescanparam.scan_interval, cmd_arg->blescanparam.scan_window);

    ESP_LOGD(TAG, "Command blescanparam:%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_bleadvdata_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADVDATA=\"%s\"\r\n", cmd_arg->bleadvdata.adv_data);

    ESP_LOGD(TAG, "Command :%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_blescan_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    if (cmd_arg->blescan.interval != -1) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLESCAN=%d,%d\r\n", cmd_arg->blescan.enable, cmd_arg->blescan.interval);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLESCAN=%d\r\n", cmd_arg->blescan.enable);
    }

    ESP_LOGD(TAG, "Command :%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_bleconn_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLECONN=%d,\"%s\"\r\n", cmd_arg->bleconn.conn_index, cmd_arg->bleconn.remote_address);

    ESP_LOGI(TAG, "Command bleconn:%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_bledisconn_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[30];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEDISCONN=%d\r\n", cmd_arg->bleconn.conn_index);

    ESP_LOGI(TAG, "Command bledisconn:%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_bleadvparam_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[60];

    if (cmd_arg->bleadvparam.adv_filter_policy != -1 && cmd_arg->bleadvparam.peer_addr_type != -1 && strlen(cmd_arg->bleadvparam.peer_address) > 0) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADVPARAM=%d,%d,%d,%d,%d,%d,%d,%s\r\n", cmd_arg->bleadvparam.adv_int_min, cmd_arg->bleadvparam.adv_int_max, cmd_arg->bleadvparam.adv_type, cmd_arg->bleadvparam.addr_type, cmd_arg->bleadvparam.channel, cmd_arg->bleadvparam.adv_filter_policy, cmd_arg->bleadvparam.peer_addr_type, cmd_arg->bleadvparam.peer_address);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADVPARAM=%d,%d,%d,%d,%d\r\n", cmd_arg->bleadvparam.adv_int_min, cmd_arg->bleadvparam.adv_int_max, cmd_arg->bleadvparam.adv_type, cmd_arg->bleadvparam.addr_type, cmd_arg->bleadvparam.channel);
    }

    ESP_LOGI(TAG, "Command :%s", cmd_tmp);
    return at_call_lib((uint8_t*)cmd_tmp, cmd_len);
}

static int8_t at_bleadvstart_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLEADVSTART\r\n", strlen("AT+BLEADVSTART\r\n"));
}

static int8_t at_bleadvstop_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLEADVSTOP\r\n", strlen("AT+BLEADVSTOP\r\n"));
}

static int8_t at_querybleaddr_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLEADDR?\r\n", strlen("AT+BLEADDR?\r\n"));
}

static int8_t at_queryblename_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLENAME?\r\n", strlen("AT+BLENAME?\r\n"));
}

static int8_t at_queryblescanparam_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLESCANPARAM?\r\n", strlen("AT+BLESCANPARAM?\r\n"));
}

static int8_t at_querybleconn_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLECONN?\r\n", strlen("AT+BLECONN?\r\n"));
}

static int8_t at_querybleadvparam_pack(at_cmd_arg* cmd_arg)
{
    return at_call_lib((uint8_t*)"AT+BLEADVPARAM?\r\n", strlen("AT+BLEADVPARAM?\r\n"));
}

at_cmd_func cmd_func_list[] = {
    {NULL, at_wait_rsp},
    {at_cwmode_pack, at_comm_rsp},
    {at_cwsap_pack, at_comm_rsp},
    {at_cipmux_pack, at_comm_rsp},
    {at_cipserver_pack, at_comm_rsp},
    {at_cipsta_pack, at_comm_rsp},
    {at_cipap_pack, at_comm_rsp},
    {at_cipsend_pack, at_cipsend_rsp},
    {at_sendinfo_pack, at_sendinfo_rsp},
    {at_cwjap_pack, at_comm_rsp},
    {at_cwqap_pack, at_comm_rsp},
    {at_cwlap_pack, at_cwlap_rsp},
    {at_fsopen_pack, at_fsopen_rsp},
    {at_fsread_pack, at_fsread_rsp},
    {at_sysram_pack, at_sysram_rsp},
    {at_cifsr_pack, at_cifsr_rsp},
    {at_cwlif_pack, at_cwlif_rsp},
    {at_ping_pack, at_ping_rsp},
    {at_autoconn_pack, at_comm_rsp},
    {at_cwdhcp_pack, at_comm_rsp},
    {at_cwdhcps_pack, at_comm_rsp},
    {at_rst_pack, at_comm_rsp},
    {at_restore_pack, at_comm_rsp},
    {at_cipstart_pack, at_comm_rsp},
    {at_cipclose_pack, at_comm_rsp},
    {at_cipstatus_pack, at_cipstatus_rsp},
    {at_cipstamac_pack, at_comm_rsp},
    {at_cipapmac_pack, at_comm_rsp},
    {at_querycwjap_pack, at_queryjap_rsp},
    {at_querycwsap_pack, at_querysap_rsp},
    {at_querydhcps_pack, at_querydhcps_rsp},
    {at_querycipsta_pack, at_queryip_rsp},
    {at_querycipap_pack, at_queryip_rsp},
    {at_querycipdns_pack, at_querydns_rsp},
    {at_querycwdhcp_pack, at_querycwdhcp_rsp},
    {at_bleinit_pack, at_comm_rsp},
    {at_bleaddr_pack, at_comm_rsp},
    {at_blename_pack, at_comm_rsp},
    {at_blescanparam_pack, at_comm_rsp},
    {at_blescan_pack, at_blescan_rsp},
    {at_bleconn_pack, at_comm_rsp},
    {at_bledisconn_pack, at_comm_rsp},
    {at_bleadvparam_pack, at_comm_rsp},
    {at_bleadvdata_pack, at_comm_rsp},
    {at_bleadvstart_pack, at_comm_rsp},
    {at_bleadvstop_pack, at_comm_rsp},
    {at_querybleinit_pack, at_querybleinit_rsp},
    {at_querybleaddr_pack, at_querybleaddr_rsp},
    {at_queryblename_pack, at_queryblename_rsp},
    {at_queryblescanparam_pack, at_queryblescanparam_rsp},
    {at_querybleconn_pack, at_querybleconn_rsp},
    {at_querybleadvparam_pack, at_querybleadvparam_rsp},
};

/*
*   Register the at command, all AT command can add by the API
*/
at_response* at_add_cmd(at_command_list cmd_list, at_cmd_arg* cmd_arg)
{
    at_response* rsp_tmp = malloc(sizeof(at_response));
    assert(rsp_tmp);
    rsp_tmp->rsp_flag = AT_ERROR;    // Init AT command flag
    at_multi_rsp_flag = false;    // Make sure is false

    // register the response func to process the AT command result
    response_cb = cmd_func_list[cmd_list].response;

    if (cmd_func_list[cmd_list].pack(cmd_arg) != 0) {
        ESP_LOGE(TAG, "pack command error.");
        at_rsp_error_msg(rsp_tmp, (void*)"cannot pack command");

        return rsp_tmp;
    }

    // wait the response callback inform us continue
    if (xQueueReceive(at_msg_queue, rsp_tmp, portMAX_DELAY) != pdPASS)  {
        ESP_LOGE(TAG, "%s xQueueReceive error.", __func__);
        at_rsp_error_msg(rsp_tmp, (void*)"Queue Receive error");
        response_cb = NULL;
        return rsp_tmp;
    }

    return rsp_tmp;
}

/*
* Every AT command must be free after get the AT result
*/
void at_cmd_free(at_response* rsp)
{
    if (rsp == NULL) {
        return;
    }

    if (rsp->data != NULL) {
        free(rsp->data);
    }

    free(rsp);
}
