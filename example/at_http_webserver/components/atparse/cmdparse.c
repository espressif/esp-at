#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "parseframe.h"
#include "esp_parse_at.h"

static const char* TAG = "CmdParse";

bool multi_rsp_flag = false;

bool malloc_dup = false;

uint8_t scan_count = 0;

bool ble_connect_flag = false;

static at_response rsp_msg;

extern rsp_cb response_cb;

extern xQueueHandle MsgQueue;

ble_scan_info* ble_link_head = NULL;
ble_scan_info* scan_pre = NULL;

void sendinfo_rsp(char* at_rsp, size_t len)
{
    if (strstr(at_rsp, "SEND OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(at_rsp, "ERROR") != NULL || strstr(at_rsp, "SEND FAIL") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }

}

void cipsend_rsp(char* at_rsp, size_t len)
{
    if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        rsp_msg.data = NULL;
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }

    if (strstr(at_rsp, ">") != NULL) {
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void comm_rsp(char* at_rsp, size_t len)
{
    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void wait_rsp(char* at_rsp, size_t len)
{
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "CONNECT") != NULL) {
        ESP_LOGI(TAG, "Receive a client");
    } else if (strstr(at_rsp, "CLOSED") != NULL) {
        ESP_LOGI(TAG, "Client closed");
    }
}

void cwjap_rsp(char* at_rsp, size_t len)
{
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "+CWJAP:") != NULL) {
        rsp_msg.data = (char*)malloc(30 * sizeof(char));
        assert(rsp_msg.data);
        ESP_LOGI(TAG, "CWJAP :%s\n", at_rsp + 7);
        strcpy(rsp_msg.data, at_rsp + 7);
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }

}

void cwlap_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if ((data = strstr(at_rsp, "+CWLAP:")) != NULL) {
        if (!multi_rsp_flag) {
            rsp_msg.data = (char*)malloc(10 * 1024 * sizeof(char));
            assert(rsp_msg.data);
            multi_rsp_flag = true;
            strcpy((char*)rsp_msg.data, data);
        } else {
            if (rsp_msg.data != NULL) {
                strcat((char*)rsp_msg.data, data);
            } else {
                printf("Error\n");
            }
        }
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(at_rsp, "OK\r\n") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        multi_rsp_flag = false;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }

}

void fsopen_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(at_rsp, "+FS:") != NULL) {
        data = strstr(at_rsp, "+FS:");
        ESP_LOGD(TAG, "Recv file len:%s\n", data + 4);
        strcpy((char*)rsp_msg.data, data + 4);

    } else if (strstr(at_rsp, "OK") != NULL) {

        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void fsread_rsp(char* at_rsp, size_t len)
{
    char* data;
    char* cmp_data = malloc(12 * sizeof(char));
    assert(rsp_msg.data);
    memset(cmp_data, '\0', 12);

    if (len > 10) {
        memcpy(cmp_data, at_rsp, 10);
    } else {
        memcpy(cmp_data, at_rsp, len);
    }

    ESP_LOGD(TAG, "%s receiv :%s,cmp_data: %s\n", __func__, at_rsp, cmp_data);

    if (strstr(cmp_data, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(cmp_data, "+FS:") != NULL) {
        data = strchr(at_rsp, ',');
        //rsp_msg.data = rsp_data;
        strcpy((char*)rsp_msg.data, data + 1);
    } else if (strstr(cmp_data, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }

    free(cmp_data);
}

void sysram_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+SYSRAM:")) != NULL) {
        rsp_msg.data = (char*)malloc(20 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data + 8);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void cifsr_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CIFSR:")) != NULL) {
        start_ptr += 7;   // skip "+CIFSR:"
        end_ptr = strstr(at_rsp, "\r\n");

        if (strstr(start_ptr, "APIP") != NULL) {
            start_ptr += 6;   // skip "APIP,\""
            rsp_msg.data = (cifsr_info*)malloc(sizeof(cifsr_info));
            assert(rsp_msg.data);
            memset(rsp_msg.data, '\0', sizeof(cifsr_info));
            memcpy(((cifsr_info*)rsp_msg.data)->ap_ip, start_ptr, end_ptr - start_ptr - 1); // skip "\""
        } else if (strstr(start_ptr, "APMAC") != NULL) {
            start_ptr += 7;   // skip "APIP,\""
            memcpy(((cifsr_info*)rsp_msg.data)->ap_mac, start_ptr, end_ptr - start_ptr - 1);
        } else if (strstr(start_ptr, "STAIP") != NULL) {
            start_ptr += 7;   // skip "STAIP,\""
            memcpy(((cifsr_info*)rsp_msg.data)->sta_ip, start_ptr, end_ptr - start_ptr - 1);
        } else if (strstr(start_ptr, "STAMAC") != NULL) {
            start_ptr += 8;   // skip "STAIP,\""
            memcpy(((cifsr_info*)rsp_msg.data)->sta_mac, start_ptr, end_ptr - start_ptr - 1);
        }

    } else if (strstr(at_rsp, "OK") != NULL) {
        ESP_LOGD(TAG, "Cifsr cmd ip infor :%s\n", (char*)rsp_msg.data);
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void cwlif_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CWLIF:")) != NULL) {
        if (!multi_rsp_flag) {
            rsp_msg.data = (char*)malloc(400 * sizeof(char));
            assert(rsp_msg.data);
            multi_rsp_flag = true;
            strcpy((char*)rsp_msg.data, data);
        } else {
            strcat((char*)rsp_msg.data, data);
        }

    } else if (strstr(at_rsp, "OK") != NULL) {
        if (rsp_msg.data != NULL) {
            ESP_LOGI(TAG, "%s cmd ip infor :%s\n", __func__, (char*)rsp_msg.data);
        } else {
            ESP_LOGI(TAG, "No ip data");
        }

        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        multi_rsp_flag = false;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void ping_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGI(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        //rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+PING:")) != NULL) {
        rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);

    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void cipstatus_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CIPSTATUS:")) != NULL) {  // In case compare "STATUS"
        strcat((char*)rsp_msg.data, data);
    } else if ((data = strstr(at_rsp, "STATUS:")) != NULL) {
        rsp_msg.data = (char*)malloc(500 * sizeof(char));
        assert(rsp_msg.data);
        strcat((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}


/*
* +CWJAP:<ssid>,<bssid>,<channel>,<rssi>
*/
void queryjap_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char channel[5];
    char rssi[5];
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CWJAP:")) != NULL) {
        rsp_msg.data = (at_cmd_arg*)malloc(sizeof(at_cmd_arg));
        assert(rsp_msg.data);
        memset(rsp_msg.data, '\0', sizeof(at_cmd_arg));
        // copy ssid info
        start_ptr += 8;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)rsp_msg.data)->queryjap.ssid, start_ptr, end_ptr - start_ptr - 1);

        // copy bssid info
        start_ptr = end_ptr + 2;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)rsp_msg.data)->queryjap.bssid, start_ptr, end_ptr - start_ptr - 1);

        // copy channel info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(channel, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)rsp_msg.data)->queryjap.channel = atoi(channel);

        // copy rssi info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, "\r\n");
        memcpy(rssi, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)rsp_msg.data)->queryjap.rssi = atoi(rssi);

    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

// +CWDHCPS:<lease>,<start IP>,<end IP>
void querydhcps_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char lease[10];

    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CWDHCPS:")) != NULL) {
        rsp_msg.data = (at_cmd_arg*)malloc(sizeof(at_cmd_arg));
        assert(rsp_msg.data);
        memset(rsp_msg.data, '\0', sizeof(at_cmd_arg));

        ((at_cmd_arg*)rsp_msg.data)->cwdhcps.enable = 1;   // +CWJAP means we have set dhcps

        // copy lease info
        start_ptr += 9;
        end_ptr = strstr(start_ptr, ",");

        memcpy(lease, start_ptr, end_ptr - start_ptr);

        ((at_cmd_arg*)rsp_msg.data)->cwdhcps.lease = atoi(lease);

        // copy start ip info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)rsp_msg.data)->cwdhcps.start_ip, start_ptr, end_ptr - start_ptr);

        // copy end ip info
        start_ptr = end_ptr + 1;

        if ((end_ptr = strstr(start_ptr, "\r\n")) == NULL) {
            end_ptr = strstr(start_ptr, "\0");
        }

        strncpy(((at_cmd_arg*)rsp_msg.data)->cwdhcps.end_ip, start_ptr, sizeof(((at_cmd_arg*)rsp_msg.data)->cwdhcps.end_ip));

    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

/*
* +CWSAP:<ssid>,<pwd>,<chl>,<ecn>,<max conn>,<ssid hidden>
*/
void querysap_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char chl[5];
    char ecn[5];
    char max_conn[5];
    char ssid_hidden[5];
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((start_ptr = strstr(at_rsp, "+CWSAP:")) != NULL) {
        rsp_msg.data = (at_cmd_arg*)malloc(sizeof(at_cmd_arg));
        assert(rsp_msg.data);
        memset(rsp_msg.data, '\0', sizeof(at_cmd_arg));
        // copy ssid info
        start_ptr += 8;  // skip +CWSAP:\"
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)rsp_msg.data)->cwsap.ssid, start_ptr, end_ptr - start_ptr - 1);

        // copy pwd info
        start_ptr = end_ptr + 2;
        end_ptr = strstr(start_ptr, ",");
        memcpy(((at_cmd_arg*)rsp_msg.data)->cwsap.pwd, start_ptr, end_ptr - start_ptr - 1);

        // copy channel info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(chl, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)rsp_msg.data)->cwsap.chl = atoi(chl);

        // copy encryption info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(ecn, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)rsp_msg.data)->cwsap.ecn = atoi(ecn);

        // copy max connect info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, ",");
        memcpy(max_conn, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)rsp_msg.data)->cwsap.max_conn = atoi(max_conn);

        // copy ssid hidden info
        start_ptr = end_ptr + 1;
        end_ptr = strstr(start_ptr, "\r\n");
        memcpy(ssid_hidden, start_ptr, end_ptr - start_ptr);
        ((at_cmd_arg*)rsp_msg.data)->cwsap.ssid_hidden = atoi(ssid_hidden);

    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}


void queryip_rsp(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (((start_ptr = strstr(at_rsp, "+CIPSTA:")) != NULL) || ((start_ptr = strstr(at_rsp, "+CIPAP:")) != NULL)) {
//	start_ptr += 7;    // skip 7 byte("+CIPAP:")
        end_ptr = strstr(at_rsp, "\r\n");

        if ((start_ptr = strstr(at_rsp, "ip:")) != NULL) {
            rsp_msg.data = (ip_info*)malloc(sizeof(ip_info));
            assert(rsp_msg.data);
            memset(rsp_msg.data, '\0', sizeof(ip_info));
            start_ptr += 4;
            memcpy(((ip_info*)rsp_msg.data)->ip, start_ptr, end_ptr - start_ptr - 1);
        } else if ((start_ptr = strstr(at_rsp, "gateway:")) != NULL) {
            start_ptr += 9;
            memcpy(((ip_info*)rsp_msg.data)->gateway, start_ptr, end_ptr - start_ptr - 1);
        } else if ((start_ptr = strstr(at_rsp, "netmask:")) != NULL) {
            start_ptr += 9;
            memcpy(((ip_info*)rsp_msg.data)->netmask, start_ptr, end_ptr - start_ptr - 1);
        }

    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void querydns_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CIPDNS:")) != NULL) {
        if (!multi_rsp_flag) {
            rsp_msg.data = (char*)malloc(200 * sizeof(char));
            assert(rsp_msg.data);
            multi_rsp_flag = true;
            strcpy((char*)rsp_msg.data, data);
        } else {
            strcat((char*)rsp_msg.data, data);
        }
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        multi_rsp_flag = false;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void querycwdhcp_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+CWDHCP:")) != NULL) {
        rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        multi_rsp_flag = false;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void querybleinit_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLEINIT:")) != NULL) {
        rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void querybleaddr_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLEADDR:")) != NULL) {
        rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void queryblename_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLENAME:")) != NULL) {
        rsp_msg.data = (char*)malloc(50 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void saveblescanrst(char* at_rsp, size_t len)
{
    char* start_ptr = NULL;
    char* end_ptr = NULL;
    char rssi[10] = {0};
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "+BLESCAN:") != NULL && scan_count < MAX_SCAN_COUNT) {
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

        if (!multi_rsp_flag) {
            ble_link_head = scan_info_ptr;
            scan_pre = scan_info_ptr;
            scan_count = 1;
            multi_rsp_flag = true;
        } else {
            ble_scan_info* scan_tmp = ble_link_head;

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
                scan_count ++;
            }

        }

    }
}

void blescan_rsp(char* at_rsp, size_t len)
{

    if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        rsp_msg.data = NULL;
        multi_rsp_flag = false;    // rescan, init head point

        scan_count = 0;        // first scan or rescan, scan count reset
        response_cb = saveblescanrst;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}
/*
void bleconn_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGI(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLECONN:")) != NULL) {
        rsp_msg.data = (char*)malloc(100 * sizeof(char));
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
	rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}
*/
void queryblescanparam_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLESCANPARAM:")) != NULL) {
        rsp_msg.data = (char*)malloc(100 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

void querybleconn_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "AT+BLECONN?") != NULL) {
        ESP_LOGI(TAG, "Query BLE connection");
        ble_connect_flag = true;
    }

    if (ble_connect_flag) {
        if (strstr(at_rsp, "ERROR") != NULL) {
            rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
            rsp_msg.data = NULL;
            xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
        } else if ((data = strstr(at_rsp, "+BLECONN:")) != NULL) {
            if (!multi_rsp_flag) {
                rsp_msg.data = (char*)malloc(200 * sizeof(char));
                assert(rsp_msg.data);
                multi_rsp_flag = true;
                strcpy((char*)rsp_msg.data, data);
            } else {
                strcat((char*)rsp_msg.data, data);
            }
        } else if (strstr(at_rsp, "OK") != NULL) {
            rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
            multi_rsp_flag = false;
            ble_connect_flag = false;
            xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
        }
    }
}

void querybleadvparam_rsp(char* at_rsp, size_t len)
{
    char* data;
    ESP_LOGD(TAG, "%s receiv :%s\n", __func__, at_rsp);

    if (strstr(at_rsp, "ERROR") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_FAIL;
        rsp_msg.data = NULL;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    } else if ((data = strstr(at_rsp, "+BLEADVPARAM:")) != NULL) {
        rsp_msg.data = (char*)malloc(100 * sizeof(char));
        assert(rsp_msg.data);
        strcpy((char*)rsp_msg.data, data);
    } else if (strstr(at_rsp, "OK") != NULL) {
        rsp_msg.rsp_flag = AT_CMD_RETURN_SUCCESS;
        xQueueSend(MsgQueue, (void*)&rsp_msg, 0);
    }
}

/* ------------------------------PACK---------------------------------------------------  */
int8_t cwmode_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[30];
    uint8_t cmd_len = 0;

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWMODE=%d\r\n", cmd_arg->cwmode.mode);
    ESP_LOGD(TAG, "%s command :%s\n", __func__, cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cwsap_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[200];
    uint8_t cmd_len = 0;

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWSAP=\"%s\",\"%s\",%d,%d", cmd_arg->cwsap.ssid, cmd_arg->cwsap.pwd, cmd_arg->cwsap.chl, cmd_arg->cwsap.ecn);

    if (cmd_arg->cwsap.max_conn != -1) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",%d", cmd_arg->cwsap.max_conn);

        if (cmd_arg->cwsap.ssid_hidden != -1) {
            cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",%d", cmd_arg->cwsap.ssid_hidden);
        }
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGI(TAG, "cwsap command :%s\n", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipmux_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[30];
    uint8_t cmd_len = 0;
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPMUX=%d\r\n", cmd_arg->cipmux.mode);

    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipserver_pack(at_cmd_arg* cmd_arg)
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
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipsend_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[50];
    uint32_t cmd_len = 0;

    if (cmd_arg->cipsend.link_id == -1) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSEND=%d\r\n", cmd_arg->cipsend.length);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSEND=%d,%d\r\n", cmd_arg->cipsend.link_id, cmd_arg->cipsend.length);
    }

    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t sendinfo_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)cmd_arg->sendinfo.data, cmd_arg->sendinfo.data_len);
}

int8_t cwjap_pack(at_cmd_arg* cmd_arg)
{
    char cmd_tmp[200];
    uint8_t cmd_len = 0;

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWJAP=\"%s\",\"%s\"", cmd_arg->cwjap.ssid, cmd_arg->cwjap.pwd);

    if (strlen(cmd_arg->cwjap.bssid) != 0) {
        cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, ",\"%s\"", cmd_arg->cwjap.bssid);
    }

    cmd_len += snprintf(cmd_tmp + cmd_len, sizeof(cmd_tmp) - cmd_len, "\r\n");

    ESP_LOGI(TAG, "cwjap command :%s\n", cmd_tmp);

    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cwqap_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CWQAP\r\n", strlen("AT+CWQAP\r\n"));
}

int8_t cwlap_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CWLAP\r\n", strlen("AT+CWLAP\r\n"));
}

int8_t fsopen_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+FS=0,3,\"%s\"\r\n", cmd_arg->fsopen.filename);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t fsread_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+FS=0,2,\"%s\",%d,%d\r\n", cmd_arg->fsread.filename, cmd_arg->fsread.offset, cmd_arg->fsread.length);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t sysram_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+SYSRAM?\r\n", strlen("AT+SYSRAM?\r\n"));
}

int8_t cifsr_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CIFSR\r\n", strlen("AT+CIFSR\r\n"));
}

int8_t cwlif_pack(at_cmd_arg* cmd_arg)
{
    rsp_msg.data = NULL;   // If no connect device, we need make sure no data
    return call_lib((uint8_t*)"AT+CWLIF\r\n", strlen("AT+CWLIF\r\n"));
}

int8_t ping_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];
    rsp_msg.data = NULL;
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+PING=\"%s\"\r\n", cmd_arg->ping.ip);
    ESP_LOGI(TAG, "ping command :%s\n", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t autoconn_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWAUTOCONN=%d\r\n", cmd_arg->autoconn.enable);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cwdhcp_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWDHCP=%d,%d\r\n", cmd_arg->cwdhcp.operate, cmd_arg->cwdhcp.mode);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cwdhcps_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[200];

    if (cmd_arg->cwdhcps.enable == 0) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWDHCPS=%d\r\n", cmd_arg->cwdhcps.enable);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CWDHCPS=%d,%d,\"%s\",\"%s\"\r\n", cmd_arg->cwdhcps.enable, cmd_arg->cwdhcps.lease, cmd_arg->cwdhcps.start_ip, cmd_arg->cwdhcps.end_ip);
    }

    ESP_LOGI(TAG, "cwdhcps command :%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipsta_pack(at_cmd_arg* cmd_arg)
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
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipap_pack(at_cmd_arg* cmd_arg)
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
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t rst_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+RST\r\n", strlen("AT+RST\r\n"));
}

int8_t restore_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
}

int8_t cipstart_pack(at_cmd_arg* cmd_arg)
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
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipclose_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[30];

    if (cmd_arg->cipclose.link_id != -1) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPCLOSE=%d\r\n", cmd_arg->cipclose.link_id);
    } else {
        strcpy(cmd_tmp, "AT+CIPCLOSE\r\n");
        cmd_len = strlen(cmd_tmp);
    }

    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipstatus_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CIPSTATUS\r\n", strlen("AT+CIPSTATUS\r\n"));
}

int8_t querycipsta_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CIPSTA?\r\n", strlen("AT+CIPSTA?\r\n"));
}

int8_t cipstamac_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPSTAMAC=\"%s\"\r\n", cmd_arg->cipmac.mac);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t cipapmac_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];
    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+CIPAPMAC=\"%s\"\r\n", cmd_arg->cipmac.mac);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t querycipap_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CIPAP?\r\n", strlen("AT+CIPAP?\r\n"));
}

int8_t querycwjap_pack(at_cmd_arg* cmd_arg)
{
    rsp_msg.data = NULL;   // If no connect AP, we need make sure no data
    return call_lib((uint8_t*)"AT+CWJAP?\r\n", strlen("AT+CWJAP?\r\n"));
}

int8_t querycwsap_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CWSAP?\r\n", strlen("AT+CWSAP?\r\n"));
}

int8_t querycipdns_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CIPDNS?\r\n", strlen("AT+CIPDNS?\r\n"));
}

int8_t querycwdhcp_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CWDHCP?\r\n", strlen("AT+CWDHCP?\r\n"));
}

int8_t querydhcps_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+CWDHCPS?\r\n", strlen("AT+CWDHCPS?\r\n"));
}

int8_t querybleinit_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLEINIT?\r\n", strlen("AT+BLEINIT?\r\n"));
}

int8_t bleinit_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[50];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEINIT=%d\r\n", cmd_arg->bleinit.init_mode);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t bleaddr_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADDR=%d,\"%s\"\r\n", cmd_arg->bleaddr.addr_type, cmd_arg->bleaddr.random_addr);

    ESP_LOGD(TAG, "Command :%s\n", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t blename_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLENAME=\"%s\"\r\n", cmd_arg->blename.device_name);

    ESP_LOGD(TAG, "Command :%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t blescanparam_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLESCANPARAM=%d,%d,%d,%d,%d\r\n", cmd_arg->blescanparam.scan_type, cmd_arg->blescanparam.addr_type, cmd_arg->blescanparam.filter_policy, cmd_arg->blescanparam.scan_interval, cmd_arg->blescanparam.scan_window);

    ESP_LOGD(TAG, "Command blescanparam:%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t bleadvdata_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADVDATA=\"%s\"\r\n", cmd_arg->bleadvdata.adv_data);

    ESP_LOGD(TAG, "Command :%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t blescan_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    if (cmd_arg->blescan.interval != -1) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLESCAN=%d,%d\r\n", cmd_arg->blescan.enable, cmd_arg->blescan.interval);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLESCAN=%d\r\n", cmd_arg->blescan.enable);
    }

    ESP_LOGD(TAG, "Command :%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t bleconn_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[100];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLECONN=%d,\"%s\"\r\n", cmd_arg->bleconn.conn_index, cmd_arg->bleconn.remote_address);

    ESP_LOGI(TAG, "Command bleconn:%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t bledisconn_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[30];

    cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEDISCONN=%d\r\n", cmd_arg->bleconn.conn_index);

    ESP_LOGI(TAG, "Command bledisconn:%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t bleadvparam_pack(at_cmd_arg* cmd_arg)
{
    uint32_t cmd_len = 0;
    char cmd_tmp[30];

    if (cmd_arg->bleadvparam.adv_filter_policy != -1 && cmd_arg->bleadvparam.peer_addr_type != -1 && strlen(cmd_arg->bleadvparam.peer_address) > 0) {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADVPARAM=%d,%d,%d,%d,%d,%d,%d,%s\r\n", cmd_arg->bleadvparam.adv_int_min, cmd_arg->bleadvparam.adv_int_max, cmd_arg->bleadvparam.adv_type, cmd_arg->bleadvparam.addr_type, cmd_arg->bleadvparam.channel, cmd_arg->bleadvparam.adv_filter_policy, cmd_arg->bleadvparam.peer_addr_type, cmd_arg->bleadvparam.peer_address);
    } else {
        cmd_len = snprintf(cmd_tmp, sizeof(cmd_tmp), "AT+BLEADVPARAM=%d,%d,%d,%d,%d\r\n", cmd_arg->bleadvparam.adv_int_min, cmd_arg->bleadvparam.adv_int_max, cmd_arg->bleadvparam.adv_type, cmd_arg->bleadvparam.addr_type, cmd_arg->bleadvparam.channel);
    }

    ESP_LOGI(TAG, "Command :%s", cmd_tmp);
    return call_lib((uint8_t*)cmd_tmp, cmd_len);
}

int8_t bleadvstart_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLEADVSTART\r\n", strlen("AT+BLEADVSTART\r\n"));
}

int8_t bleadvstop_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLEADVSTOP\r\n", strlen("AT+BLEADVSTOP\r\n"));
}


int8_t querybleaddr_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLEADDR?\r\n", strlen("AT+BLEADDR?\r\n"));
}

int8_t queryblename_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLENAME?\r\n", strlen("AT+BLENAME?\r\n"));
}

int8_t queryblescanparam_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLESCANPARAM?\r\n", strlen("AT+BLESCANPARAM?\r\n"));
}

int8_t querybleconn_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLECONN?\r\n", strlen("AT+BLECONN?\r\n"));
}

int8_t querybleadvparam_pack(at_cmd_arg* cmd_arg)
{
    return call_lib((uint8_t*)"AT+BLEADVPARAM?\r\n", strlen("AT+BLEADVPARAM?\r\n"));
}

at_cmd_func cmd_func_list[] = {
    {NULL, wait_rsp},
    {cwmode_pack, comm_rsp},
    {cwsap_pack, comm_rsp},
    {cipmux_pack, comm_rsp},
    {cipserver_pack, comm_rsp},
    {cipsta_pack, comm_rsp},
    {cipap_pack, comm_rsp},
    {cipsend_pack, cipsend_rsp},
    {sendinfo_pack, sendinfo_rsp},
    {cwjap_pack, comm_rsp},
    {cwqap_pack, comm_rsp},
    {cwlap_pack, cwlap_rsp},
    {fsopen_pack, fsopen_rsp},
    {fsread_pack, fsread_rsp},
    {sysram_pack, sysram_rsp},
    {cifsr_pack, cifsr_rsp},
    {cwlif_pack, cwlif_rsp},
    {ping_pack, ping_rsp},
    {autoconn_pack, comm_rsp},
    {cwdhcp_pack, comm_rsp},
    {cwdhcps_pack, comm_rsp},
    {rst_pack, comm_rsp},
    {restore_pack, comm_rsp},
    {cipstart_pack, comm_rsp},
    {cipclose_pack, comm_rsp},
    {cipstatus_pack, cipstatus_rsp},
    {cipstamac_pack, comm_rsp},
    {cipapmac_pack, comm_rsp},
    {querycwjap_pack, queryjap_rsp},
    {querycwsap_pack, querysap_rsp},
    {querydhcps_pack, querydhcps_rsp},
    {querycipsta_pack, queryip_rsp},
    {querycipap_pack, queryip_rsp},
    {querycipdns_pack, querydns_rsp},
    {querycwdhcp_pack, querycwdhcp_rsp},
    {bleinit_pack, comm_rsp},
    {bleaddr_pack, comm_rsp},
    {blename_pack, comm_rsp},
    {blescanparam_pack, comm_rsp},
    {blescan_pack, blescan_rsp},
    {bleconn_pack, comm_rsp},
    {bledisconn_pack, comm_rsp},
    {bleadvparam_pack, comm_rsp},
    {bleadvdata_pack, comm_rsp},
    {bleadvstart_pack, comm_rsp},
    {bleadvstop_pack, comm_rsp},
    {querybleinit_pack, querybleinit_rsp},
    {querybleaddr_pack, querybleaddr_rsp},
    {queryblename_pack, queryblename_rsp},
    {queryblescanparam_pack, queryblescanparam_rsp},
    {querybleconn_pack, querybleconn_rsp},
    {querybleadvparam_pack, querybleadvparam_rsp},
};


/*
*   Register the at command, all AT command can add by the API
*/
at_response* at_add_cmd(at_command_list cmd_list, at_cmd_arg* cmd_arg)
{
    at_response* rsp_tmp = malloc(sizeof(at_response));
    assert(rsp_tmp);
    rsp_tmp->rsp_flag = AT_ERROR;    // Init AT command flag
    multi_rsp_flag = false;    // Make sure is false

    // register the response func to process the AT command result
    response_cb = cmd_func_list[cmd_list].response;

    if (cmd_func_list[cmd_list].pack(cmd_arg) != 0) {
        ESP_LOGE(TAG, "pack command error.");
        at_rsp_error_msg(rsp_tmp, (void*)"cannot pack command");

        return rsp_tmp;
    }

    // wait the response callback inform us continue
    if (xQueueReceive(MsgQueue, rsp_tmp, portMAX_DELAY) != pdPASS)  {
        ESP_LOGE(TAG, "%s xQueueReceive error.", __func__);
        at_rsp_error_msg(rsp_tmp, (void*)"Queue Receive error");
        response_cb = NULL;
        return rsp_tmp;

    }

    //response_cb = NULL;
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

