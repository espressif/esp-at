/*
 * File : at_ipCmd.c
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
#include "at_version.h"
#include "at.h"
#include "at_ipCmd.h"
#include "at_nvm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

#include "lwip/err.h"
#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"

#include "lwip/netdb.h"

#include "esp_system.h"

volatile at_mdStateType mdState = m_inactive;

volatile bool at_ip_mode = FALSE;

volatile bool at_ipMux = FALSE;

uint8_t at_netconn_max_num = 5;//MEMP_NUM_NETCONN - 1;
at_link_info_t *pLink = NULL;
volatile uint8_t sendingID = 0;
static volatile bool ipd_info_enable = FALSE;

static volatile uint16_t server_timeover = 180;
static int32_t tcp_server_socket = -1;
static xTaskHandle at_tcp_server_task_handle = NULL;
static xTimerHandle server_timeout_timer;
static SemaphoreHandle_t at_socket_mutex;

#define AT_DATA_BUFFER_MAX_SIZE     2920
static int32_t at_transmit_data_len = 0;
static uint8_t at_transmit_data_buf[AT_DATA_BUFFER_MAX_SIZE + 1];
extern xQueueHandle at_process_queue;


#define AT_SOCKET_LOCK()        xSemaphoreTake(at_socket_mutex,portMAX_DELAY)
#define AT_SOCKET_UNLOCK()      xSemaphoreGive(at_socket_mutex)

/** @defgroup AT_IPCMD_Functions
 * @{
 */
#define AT_SOCKET_QUEUE_DEPTH    10

#define AT_SOCKET_TASK_PRIORITIE  4 //(AT_TASK_PRIORITIE)

static bool at_ip_cmd_need_save_flag = false;
// static bool at_ip_cmd_read_from_flash_flag = FALSE;

void at_netconn_init(uint32_t netconn_max)
{
    uint8_t loop = 0;
    if (netconn_max != 0) {
        at_netconn_max_num = netconn_max;
    }
    at_socket_mutex = xSemaphoreCreateMutex();
    pLink = (at_link_info_t *)at_malloc(at_netconn_max_num * sizeof(at_link_info_t));
    if (pLink == NULL) {
        at_printf("pLink is NULL/r/n");
        esp_restart();
    }
    at_memset(pLink, 0x0, at_netconn_max_num * sizeof(at_link_info_t));
    for (loop = 0; loop < at_netconn_max_num; loop++) {
        pLink[loop].socket = -1;
        pLink[loop].idx = loop;
        vSemaphoreCreateBinary(pLink[loop].sema);
        xSemaphoreTake(pLink[loop].sema, 0);
    }
//    espconn_tcp_set_max_con(at_netconn_max_num);
}

void at_update_link_count(int8_t count)
{
    static int32_t cnt = 0;
    int32_t tmp = 0;
    tmp = cnt + count;

    if ((cnt > 0) && (tmp <= 0)) {
        cnt = 0;
        if (mdState == m_linked) {
            mdState = m_unlink;
        }
    } else if ((cnt == 0) && (tmp > 0)) {
        cnt = tmp;
        mdState = m_linked;
    }
}

/**
 * @brief  Test commad of get module ip.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_testCmdCifsr(uint8_t *cmd_name)
{
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCifsr(uint8_t para_num)
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t *pStr = NULL;
    wifi_mode_t mode;
    tcpip_adapter_dhcp_status_t status = TCPIP_ADAPTER_DHCP_INIT;

    if (para_num != 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_memset(&adapter_ip_info, 0x0, sizeof(adapter_ip_info));
    if (mode == WIFI_MODE_STA) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_str (0, &pStr) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((pStr == NULL) || (*pStr == '\0')) {
        at_port_print((uint8_t *)"IP ERROR\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info);
    adapter_ip_info.ip.addr = inet_addr((char *)pStr);

    if ((adapter_ip_info.ip.addr == ((uint32_t)0xffffffffUL))
            || (adapter_ip_info.ip.addr == ((uint32_t)0x00000000UL))) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    at_printf("%d.%d.%d.%d\r\n", IP2STR(&adapter_ip_info.ip));
    tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_AP, &status);
    if (status == TCPIP_ADAPTER_DHCP_STARTED) {
        tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
    }
    if (tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info) != ESP_OK) {
        if (status == TCPIP_ADAPTER_DHCP_STARTED) {
            tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
        }
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Execution commad of get module ip.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdCifsr(uint8_t *cmd_name)//add get station ip and ap ip
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t temp[64];
    uint8_t bssid[6];
    wifi_mode_t mode = WIFI_MODE_NULL;

    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((mode == WIFI_MODE_AP) || (mode == WIFI_MODE_APSTA)) {
        at_memset(&adapter_ip_info, 0x0, sizeof(adapter_ip_info));
        tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info);
        at_sprintf(temp, "%s:APIP,\""IPSTR"\"\r\n",cmd_name,IP2STR(&adapter_ip_info.ip));
        at_port_print(temp);

        esp_wifi_get_mac(WIFI_IF_AP, bssid);
        at_sprintf(temp, "%s:APMAC,\""MACSTR"\"\r\n",cmd_name,MAC2STR(bssid));
        at_port_print(temp);
    }

    if((mode == WIFI_MODE_STA)||(mode == WIFI_MODE_APSTA))
    {
        at_memset(&adapter_ip_info,0x0,sizeof(adapter_ip_info));
        if(wifi_station_get_connect_status() == STATION_GOT_IP) {
            tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &adapter_ip_info);
        }
        at_sprintf(temp, "%s:STAIP,\""IPSTR"\"\r\n",cmd_name,IP2STR(&adapter_ip_info.ip));
        at_port_print(temp);

        esp_wifi_get_mac(WIFI_IF_STA, bssid);
        at_sprintf(temp, "%s:STAMAC,\""MACSTR"\"\r\n",cmd_name,MAC2STR(bssid));
        at_port_print(temp);
    }

    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Execution commad of get link status.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdCipstatus(uint8_t* cmd_name)
{
    uint8_t* temp = NULL;
    uint8_t i = 0;
    struct sockaddr_in local_addr;
    socklen_t local_name_len;
    uint8_t pStr[4];
    int sc_type = 0;
    socklen_t optlen = sizeof(sc_type);
    ip4_addr_t ip_addr;
    
    temp = (uint8_t*)at_malloc(64);

//    if(wifi_station_get_connect_status() != STATION_GOT_IP)
//    {
//        mdState = m_disconnet;
//    }
//    else
//    {
//        if(mdState == m_disconnet)
//        {
//            mdState = m_gotip;
//        }
//    }

    at_sprintf(temp, "STATUS:%d\r\n",mdState);
    at_port_print(temp);

    for(i = 0; i < at_netconn_max_num; i++)
    {
        if(pLink[i].socket >= 0)
        {
            local_name_len = sizeof(local_addr);
            if(getsockopt(pLink[i].socket,SOL_SOCKET,SO_TYPE,&sc_type,&optlen) < 0)
            {
                at_free(temp);
                return ESP_AT_RESULT_CODE_ERROR;
            }

            if(getsockname(pLink[i].socket,(struct sockaddr*)&local_addr,&local_name_len) < 0)
            {
                at_free(temp);
                return ESP_AT_RESULT_CODE_ERROR;
            }

            at_memset(pStr,0x0,sizeof(pStr));
            if(sc_type == SOCK_STREAM) // tcp
            {
                pStr[0] = 'T';
                pStr[1] = 'C';
                pStr[2] = 'P';
            }
            else if(sc_type == SOCK_DGRAM) // udp
            {
                pStr[0] = 'U';
                pStr[1] = 'D';
                pStr[2] = 'P';
            }
            else // raw
            {
                pStr[0] = 'R';
                pStr[1] = 'A';
                pStr[2] = 'W';
            }

            ip_addr.addr = pLink[i].remote_ip.addr;
            at_sprintf(temp, "%s:%d,\"%s\",\""IPSTR"\",%d,%d,%d\r\n",
                    cmd_name,
                    pLink[i].idx,
                    pStr,
                    IP2STR(&ip_addr),
                    pLink[i].remote_port,
                    ntohs(local_addr.sin_port),
                    pLink[i].terminal_type);
            at_port_print(temp);
        }
    }
    at_free(temp);

    return ESP_AT_RESULT_CODE_OK;
}


/**
 * @brief  Setup commad of start client.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCipstart(uint8_t para_num)
{
    int8_t linkType = AT_SOCKET_TYPE_INVALID;
    uint8_t cnt = 0;
    uint8_t* ipTemp = NULL;
    int32_t remotePort = 0;
    int32_t localPort = 0;
    int32_t linkID = 0;
    int32_t changType = 0;
    int32_t alive_time = 0;
    uint8_t* pStr = NULL;
    struct sockaddr_in sa;
    ip_addr_t ip_address;
    uint32_t optval = 0;
    socklen_t len = sizeof(optval);
    int32_t sock = -1;
    struct hostent* hptr = NULL;
    wifi_mode_t mode = WIFI_MODE_NULL;
    int ret = 0;
#ifdef AT_SSL_SUPPORT
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
#endif
    if(para_num < 3)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1,para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    // link id
    if(at_ipMux)
    {
        if(esp_at_get_para_as_digit (cnt++,&linkID) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }
    else
    {
        linkID = 0;
    }
    if(linkID >= at_netconn_max_num)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // TCP or UDP
    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pStr == NULL)
    {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(at_strcmp(pStr, "TCP") == 0)  // TCP
    {
        linkType = AT_SOCKET_TYPE_TCP;
    }
    else if(at_strcmp(pStr, "UDP") == 0)  // UDP
    {
        linkType = AT_SOCKET_TYPE_UDP;
    }
#ifdef AT_SSL_SUPPORT
    else if(at_strcmp(pStr, "SSL") == 0)  // SSL
    {
        linkType = AT_SOCKET_TYPE_SSL;
    }
#endif
    else
    {
        at_port_print((uint8_t*)"Link typ ERROR\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_str (cnt++,&ipTemp) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        at_port_print((uint8_t*)"IP ERROR\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(ipTemp == NULL) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit (cnt++,&remotePort) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        at_port_print((uint8_t*)"IP ERROR\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if((remotePort > 65535) || (remotePort < 0))
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(linkType == AT_SOCKET_TYPE_UDP)
    {
//        at_printf("remote port:%d\r\n", remotePort);
        if(cnt < para_num)
        {
            if (esp_at_get_para_as_digit (cnt++,&localPort) != ESP_AT_PARA_PARSE_RESULT_OK)
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }

            if((localPort > 65535) || (localPort <= 0))
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            at_printf("local port:%d\r\n", localPort);

            if(cnt < para_num)
            {
                if (esp_at_get_para_as_digit (cnt++,&changType) != ESP_AT_PARA_PARSE_RESULT_OK)
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                if((changType < 0) || (changType > 2))
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                at_printf("change type:%d\r\n", changType);
            }
        }
#ifdef AT_SSL_SUPPORT
    } else if ((linkType == AT_SOCKET_TYPE_TCP)|| (linkType == AT_SOCKET_TYPE_SSL)) {
#else
    } else if (linkType == AT_SOCKET_TYPE_TCP) {
#endif
        if (cnt < para_num) {
            if (esp_at_get_para_as_digit (cnt++,&alive_time) != ESP_AT_PARA_PARSE_RESULT_OK)
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }

            if ((alive_time > 7200) || (alive_time < 0)) { // we set max timeout 7200s
                return ESP_AT_RESULT_CODE_ERROR;
            }
        } else {
            alive_time = 0;
        }
        localPort = 0;//(int32_t)espconn_port();
        changType = 1;
    }

    if(cnt != para_num)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pLink[linkID].socket >= 0)
    {
        at_port_print((uint8_t*)"ALREADY CONNECTED\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_wifi_get_mode(&mode) != ESP_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (mode == WIFI_MODE_NULL) {
        at_port_print((uint8_t*)"mode:null\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    if (mode == WIFI_MODE_STA)
    {
        if(wifi_station_get_connect_status() != STATION_GOT_IP)
        {
            at_port_print((uint8_t*)"no ip\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    ip_address.u_addr.ip4.addr = inet_addr((char*)ipTemp);

    if ((ip_address.u_addr.ip4.addr == IPADDR_NONE) && (at_strcmp(ipTemp,"255.255.255.255") != 0)) {
        if((hptr = gethostbyname((char*)ipTemp)) == NULL)
        {
            at_printf("gethostbyname fail\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }
        ip_address = *(ip_addr_t*)hptr->h_addr_list[0];
    }

    AT_SOCKET_LOCK();
    pLink[linkID].link_state = AT_LINK_DISCONNECTED;
    pLink[linkID].change_mode = changType;
    pLink[linkID].remote_port = remotePort;
    pLink[linkID].remote_ip = ip_address.u_addr.ip4;
    pLink[linkID].repeat_time = 0;
    pLink[linkID].ip_type = linkType;
    pLink[linkID].keep_alive = alive_time;
    pLink[linkID].terminal_type = AT_LOCAL_CLIENT;
    
    switch(linkType)
    {
    case AT_SOCKET_TYPE_TCP:
        pLink[linkID].socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(pLink[linkID].socket < 0)
        {
            AT_SOCKET_UNLOCK();
            at_printf("socket fail:%d\r\n",pLink[linkID].socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }
        AT_SOCKET_UNLOCK();
        optval = 1;
        len = sizeof(optval);
        setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_REUSEADDR,&optval,len);
        setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_REUSEPORT,&optval,len);
        if (pLink[linkID].keep_alive) {
            optval = 1;
            setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_KEEPALIVE,&optval,len);
            optval = pLink[linkID].keep_alive;
            setsockopt(pLink[linkID].socket,IPPROTO_TCP,TCP_KEEPIDLE,&optval,len);
            optval = 1;
            setsockopt(pLink[linkID].socket,IPPROTO_TCP,TCP_KEEPINTVL,&optval,len);
            optval = 3;
            setsockopt(pLink[linkID].socket,IPPROTO_TCP,TCP_KEEPCNT,&optval,len);
        }

        at_memset(&sa,0x0,sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(remotePort);
        sa.sin_addr.s_addr = ip_address.u_addr.ip4.addr;
        pLink[linkID].link_state = AT_LINK_CONNECTING;

        ret = connect(pLink[linkID].socket,(struct sockaddr *)&sa,sizeof(sa));
        if(ret < 0) {
            at_printf("connect fail,ret=%d\r\n",ret);

            socklen_t name_len = sizeof(ret);
            if(getsockopt(pLink[linkID].socket,SOL_SOCKET,SO_ERROR,&ret,&name_len) == 0) {
                at_printf("error=%d\r\n",ret);
            }
            sock = pLink[linkID].socket;
            pLink[linkID].socket = -1;
            pLink[linkID].ip_type = AT_SOCKET_TYPE_INVALID;
            close(sock);
            return ESP_AT_RESULT_CODE_FAIL;
        }

        pLink[linkID].link_state = AT_LINK_CONNECTED;
        break;
#ifdef AT_SSL_SUPPORT
        case AT_SOCKET_TYPE_SSL:
        pLink[linkID].socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(pLink[linkID].socket < 0)
        {
            AT_SOCKET_UNLOCK();
            at_printf("socket fail:%d\r\n",pLink[linkID].socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }
        AT_SOCKET_UNLOCK();
        optval = 1;
        len = sizeof(optval);
        setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_REUSEADDR,&optval,len);
        setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_REUSEPORT,&optval,len);
        if (pLink[linkID].keep_alive) {
            optval = 1;
            setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_KEEPALIVE,&optval,len);
            optval = pLink[linkID].keep_alive;
            setsockopt(pLink[linkID].socket,IPPROTO_TCP,TCP_KEEPIDLE,&optval,len);
            optval = 1;
            setsockopt(pLink[linkID].socket,IPPROTO_TCP,TCP_KEEPINTVL,&optval,len);
            optval = 3;
            setsockopt(pLink[linkID].socket,IPPROTO_TCP,TCP_KEEPCNT,&optval,len);
        }

        at_memset(&sa,0x0,sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(remotePort);
        sa.sin_addr.s_addr = ip_address.u_addr.ip4.addr;
        pLink[linkID].link_state = AT_LINK_CONNECTING;

        ret = connect(pLink[linkID].socket,(struct sockaddr *)&sa,sizeof(sa));
        if(ret < 0) {
            at_printf("connect fail,ret=%d\r\n",ret);

            socklen_t name_len = sizeof(ret);
            if(getsockopt(pLink[linkID].socket,SOL_SOCKET,SO_ERROR,&ret,&name_len) == 0) {
                at_printf("error=%d\r\n",ret);
            }
            sock = pLink[linkID].socket;
            pLink[linkID].socket = -1;
            pLink[linkID].ip_type = AT_SOCKET_TYPE_INVALID;
            close(sock);
            return ESP_AT_RESULT_CODE_FAIL;
        }

        ctx = SSL_CTX_new(TLSv1_1_client_method());
        if (!ctx) {
            at_printf("ctx fail\r\n");
            sock = pLink[linkID].socket;
            pLink[linkID].socket = -1;
            pLink[linkID].ip_type = AT_SOCKET_TYPE_INVALID;
            close(sock);
            return ESP_AT_RESULT_CODE_FAIL;
        }
        ssl = SSL_new(ctx);
        if (!ssl) {
            at_printf("ssl fail\r\n");
            sock = pLink[linkID].socket;
            pLink[linkID].socket = -1;
            close(sock);
            pLink[linkID].ip_type = AT_SOCKET_TYPE_INVALID;
            SSL_CTX_free(ctx);
            ctx = NULL;
            return ESP_AT_RESULT_CODE_FAIL;
        }

        SSL_set_fd(ssl, pLink[linkID].socket);

        ret = SSL_connect(ssl);
        if (!ret) {
            at_printf("ssl connect fail\r\n");
            sock = pLink[linkID].socket;
            pLink[linkID].socket = -1;
            close(sock);
            pLink[linkID].ip_type = AT_SOCKET_TYPE_INVALID;
            SSL_CTX_free(ctx);
            ctx = NULL;
            return ESP_AT_RESULT_CODE_FAIL;
        }
        SSL_CTX_free(ctx);
        ctx = NULL;
        pLink[linkID].link_state = AT_LINK_CONNECTED;
        pLink[linkID].ssl = ssl;
        break;
#endif
    case AT_SOCKET_TYPE_UDP:
        if ((at_ip_mode == TRUE) && (changType != 0)) {
			at_printf("at_ip_mode=%d,changType=%d\r\n",at_ip_mode,changType);
            AT_SOCKET_UNLOCK();
            return ESP_AT_RESULT_CODE_ERROR;
        }
        pLink[linkID].socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(pLink[linkID].socket < 0)
        {
            AT_SOCKET_UNLOCK();
            at_printf("udp sock=%d\r\n",pLink[linkID].socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }
        AT_SOCKET_UNLOCK();
        at_memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(localPort);
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(pLink[linkID].socket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
            sock = pLink[linkID].socket;
            pLink[linkID].socket = -1;
            close(sock);
            pLink[linkID].ip_type = AT_SOCKET_TYPE_INVALID;
            at_port_print((uint8_t*)"bind fail\r\n");
            return ESP_AT_RESULT_CODE_FAIL;
        }
        pLink[linkID].link_state = AT_LINK_CONNECTED;
        if (pLink[linkID].remote_ip.addr == IPADDR_NONE) {
            int broadcast_flag = 1;
            if (setsockopt(pLink[linkID].socket,SOL_SOCKET,SO_BROADCAST,&broadcast_flag,sizeof(broadcast_flag)) < 0) {
                at_printf("%s set opt fail\r\n",__func__);
            }
        }
        break;

    default:
        AT_SOCKET_UNLOCK();
        return ESP_AT_RESULT_CODE_ERROR;
        break;
    }
    if(at_ipMux)
    {
        uint8_t* temp = (uint8_t*)at_malloc(32);
        at_sprintf(temp,"%d,CONNECT\r\n", linkID);
        at_port_print(temp);
        at_free(temp);
    }
    else
    {
        at_port_print((uint8_t*)"CONNECT\r\n");

        if (at_ip_mode) {
            pLink[linkID].send_mode = AT_SEND_MODE_TRANSMIT;
            pLink[linkID].link_state = AT_LINK_TRANSMIT_CONNECTED;
        }
    }
    at_socket_client_create_task(&pLink[linkID]);
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Setup commad of close ip link.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCipclose(uint8_t para_num)
{
    int32_t linkID = 0;

    if(at_ipMux == 0)
    {
        at_port_print((uint8_t*)"MUX=0\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(para_num != 1)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (esp_at_get_para_as_digit (0,&linkID) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(linkID > at_netconn_max_num)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(linkID == at_netconn_max_num)
    {
        for(linkID = 0; linkID < at_netconn_max_num; linkID++)
        {
            if(pLink[linkID].socket >= 0)
            {
                at_socket_client_cleanup_task(&pLink[linkID]);
            }
        }
    }
    else
    {
        if(pLink[linkID].socket < 0)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        at_socket_client_cleanup_task(&pLink[linkID]);
    }
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Execution commad of close ip link.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdCipclose(uint8_t* cmd_name)
{
    if(at_ipMux)
    {
        at_port_print((uint8_t*)"MUX=1\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pLink[0].socket >= 0)
    {
        if(at_socket_client_cleanup_task(&pLink[0]) != true) {
            at_printf("cleanup fail\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }
        return ESP_AT_RESULT_CODE_OK;
    }
    else
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
}
/**
 * @brief  Test commad of send ip data.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_testCmdCipsend(uint8_t* cmd_name)
{
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Setup commad of send ip data.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCipsend(uint8_t para_num)
{
    int32_t result_sendingID = 0;
    int32_t result_sendLen = 0;
    int32_t port = 0;
    //uint8_t ipTemp[128] = {0};
    uint8_t cnt = 0;

    ip_addr_t ip_address;
    at_task_msg msg;
    struct hostent* hptr = NULL;
    //Error case
    if(at_ip_mode == TRUE)
    {
        at_port_print((uint8_t*)"IPMODE=1\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(at_ipMux)
    {

        if(esp_at_get_para_as_digit (cnt++,&result_sendingID) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if(result_sendingID >= at_netconn_max_num)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }
    else
    {
        result_sendingID = 0;
    }

    // Check validity of sending ID
    if(pLink[result_sendingID].socket < 0)
    {
        at_port_print((uint8_t*)"link is not valid\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (cnt++,&result_sendLen) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(result_sendLen > 2048)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(pLink[result_sendingID].ip_type == AT_SOCKET_TYPE_UDP)
    {
        uint8_t* pStr = NULL;
        if(cnt != para_num)
        {
            if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }

            if(cnt != para_num)
            {
                if(esp_at_get_para_as_digit (cnt++, &port) != ESP_AT_PARA_PARSE_RESULT_OK)
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                if((port <=0) || (port >= 65536))
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
            }
        }

        if((pStr != NULL) && at_strlen(pStr))
        {
            ip_address.u_addr.ip4.addr = inet_addr((char*)pStr);
            if ((ip_address.u_addr.ip4.addr == IPADDR_NONE) && (at_strcmp(pStr,"255.255.255.255") != 0)) {
                if((hptr = gethostbyname((char*)pStr)) == NULL)
                {
                    at_printf("gethostbyname fail\r\n");
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                ip_address = *(ip_addr_t*)hptr->h_addr_list[0];
            }


            pLink[result_sendingID].remote_port = port;
            //ip_address.s_addr = htonl(ip_address.s_addr);
            pLink[result_sendingID].remote_ip = ip_address.u_addr.ip4;
        }
    }

    if(para_num != cnt)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    //
    // All Inputs are Verified To Be Correct! Update Global Variables!
    sendingID       = result_sendingID;
    pLink[result_sendingID].link_state = AT_LINK_WAIT_SENDING;
    esp_at_response_result(ESP_AT_RESULT_CODE_OK);
    at_port_print((uint8_t*)"\r\n>");
    at_form_msg_p((&msg),AT_MSG_ID_TRANSFORM_SENDING,sizeof(void*),(void*)result_sendLen);
    xQueueSend(at_recv_cmd_queue,&msg,portMAX_DELAY);
    return ESP_AT_RESULT_CODE_IGNORE;
}


uint8_t at_setupCmdCipsendEx(uint8_t para_num)
{
    uint8_t ret;
//    send_mode = AT_SEND_MODE_SENDEX;
    ret = at_setupCmdCipsend(para_num);

    if (ret == ESP_AT_RESULT_CODE_IGNORE) {
        pLink[sendingID].send_mode = AT_SEND_MODE_SENDEX;
    }
//    send_mode = AT_SEND_MODE_IDLE;
    return ret;
}
/**
 * @brief  Send data through ip.
 * @param  pAtRcvData: point to data
 * @retval None
 */
uint8_t at_sending_data(uint8_t *data,uint32_t len)
{
    if ((data == NULL) || (len == 0)) {
        return ESP_AT_RESULT_CODE_IGNORE;
    }

    uint8_t buf[64];
    at_sprintf((char*)buf, "\r\nRecv %d bytes\r\n", len);
    at_port_print(buf);

    //    at_task_msg send_msg;
    if(pLink[sendingID].socket < 0)
    {
        pLink[sendingID].link_state = AT_LINK_DISCONNECTED;
        at_printf("link_state is AT_LINK_DISCONNECTED\r\n");
        return ESP_AT_RESULT_CODE_SEND_FAIL;
    }

    if (pLink[sendingID].link_state != AT_LINK_WAIT_SENDING) {
        at_printf("link_state is changed\r\n");
        return ESP_AT_RESULT_CODE_SEND_FAIL;
    }

    if (AT_SOCKET_TYPE_UDP == pLink[sendingID].ip_type) {
        struct sockaddr_in addr;

        at_memset(&addr,0x0,sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(pLink[sendingID].remote_port);
        addr.sin_addr.s_addr = pLink[sendingID].remote_ip.addr;
        if(sendto(pLink[sendingID].socket,data,len,0,(struct sockaddr*)&addr,sizeof(addr)) == len) {
            pLink[sendingID].link_state = AT_LINK_CONNECTED;
            at_printf("id:%d,Len:%d,dp:%p\r\n",sendingID,len,data);
        } else {
            int value = 0;
            socklen_t optlen = sizeof(value);
            if (getsockopt(pLink[sendingID].socket,SOL_SOCKET,SO_ERROR,&value,&optlen) < 0) {
                at_printf("getsockopt err\r\n");
            } else {
                at_printf("errno:%d\r\n",value);
            }
            return ESP_AT_RESULT_CODE_SEND_FAIL;
        }
#ifdef AT_SSL_SUPPORT
    } else if (AT_SOCKET_TYPE_SSL== pLink[sendingID].ip_type) {
        if(SSL_write(pLink[sendingID].ssl,data,len) == len)
        {
            pLink[sendingID].link_state = AT_LINK_CONNECTED;
            at_printf("id:%d,Len:%d,dp:%p\r\n",sendingID,len,data);
        }
        else
        {
            pLink[sendingID].link_state = AT_LINK_CONNECTED;
            at_printf("send fail\r\n");
            return ESP_AT_RESULT_CODE_SEND_FAIL;
        }
#endif
    } else {
        if(send(pLink[sendingID].socket,data,len,0) == len)
        {
            pLink[sendingID].link_state = AT_LINK_CONNECTED;
            at_printf("id:%d,Len:%d,dp:%p\r\n",sendingID,len,data);
        }
        else
        {
            pLink[sendingID].link_state = AT_LINK_CONNECTED;
            at_printf("send fail\r\n");
            return ESP_AT_RESULT_CODE_SEND_FAIL;
        }
    }

    return ESP_AT_RESULT_CODE_SEND_OK;
}

void at_transparent_transmit_preprocess(uint8_t *data,int32_t len)
{
    at_task_msg send_msg;
    if((len == 3) && (at_memcmp(data, "+++", 3) == 0)) {
        esp_at_transmit_terminal();
        return;
    } else if(len) {
        at_memcpy(at_transmit_data_buf,data,len);
        at_transmit_data_len = len;
        at_form_msg_p((&send_msg),AT_MSG_ID_TRANSMIT_DATA,at_transmit_data_len,at_transmit_data_buf);
        xQueueSend( at_process_queue, &send_msg, portMAX_DELAY );
    }
}

void IRAM_ATTR esp_at_transmit_terminal_from_isr(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken;
    at_task_msg msg;
    at_transmit_data_len = 0;
    if (at_ip_mode) {
        at_form_msg_p_from_isr(&msg,AT_MSG_ID_TRANSMIT_TERMINAL,0,NULL);
        xQueueSendFromISR (at_recv_cmd_queue,&msg,&xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken == pdTRUE) {
			portYIELD_FROM_ISR();
		}
    }
}

void esp_at_transmit_terminal(void)
{
    at_task_msg msg;
    if (at_ip_mode) {
        at_transmit_data_len = 0;
        pLink[0].link_state = AT_LINK_CONNECTED;
        esp_at_clear_recv_queue();
        at_form_msg_p(&msg,AT_MSG_ID_TRANSMIT_TERMINAL,0,NULL);
        xQueueSend (at_recv_cmd_queue,&msg,portMAX_DELAY);
    }
}

/**
 * @brief  Transparent data through ip.
 * @param  arg: no used
 * @retval None
 */
int32_t at_transparent_transmit(uint8_t *data,int32_t len)
{
    int32_t ret = -1;
    ip_addr_t ip_address;
    struct hostent* hptr = NULL;
    if (at_ip_mode == FALSE) {
        return ret;
    }

    if(len) {
        if (pLink[0].socket >= 0) {
            if(AT_SOCKET_TYPE_UDP == pLink[0].ip_type) {
                struct sockaddr_in addr;

                at_memset(&addr,0x0,sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(pLink[0].remote_port);
                if (pLink[0].remote_ip.addr == IPADDR_ANY) {
                    ip_address.u_addr.ip4.addr = inet_addr((char*)pLink[0].domain);
                    if (ip_address.u_addr.ip4.addr == IPADDR_NONE) {
                        if (at_strcmp(pLink[0].domain,"255.255.255.255") != 0) {
                            if((hptr = gethostbyname((char*)pLink[0].domain)) == NULL){
                                return ret;
                            }
                        } else {
                            int broadcast_flag = 1;
                            if (setsockopt(pLink[0].socket,SOL_SOCKET,SO_BROADCAST,&broadcast_flag,sizeof(broadcast_flag)) < 0) {
                                at_printf("%s set opt fail\r\n",__func__);
                            }
                        }
                        pLink[0].remote_ip = *(ip4_addr_t*)hptr->h_addr_list[0];
                    } else {
                        pLink[0].remote_ip = ip_address.u_addr.ip4;
                    }
                }
                addr.sin_addr.s_addr = pLink[0].remote_ip.addr;
                ret = sendto(pLink[0].socket,data,len,0,(struct sockaddr*)&addr,sizeof(addr));
            } else if (pLink[0].link_state == AT_LINK_TRANSMIT_SEND){
                ret = send(pLink[0].socket,data,len,0);
            }
        }
    }
    return ret;
}

/**
 * @brief  Setup commad of send ip data.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_exeCmdCipsend(uint8_t* cmd_name)
{
    at_task_msg msg;
    if((at_ipMux == TRUE) || (at_ip_mode == FALSE))
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(pLink[0].socket < 0)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    esp_at_response_result(ESP_AT_RESULT_CODE_OK);
    at_port_print((uint8_t*)"\r\n>");
    pLink[0].link_state = AT_LINK_TRANSMIT_SEND;
    at_form_msg_p((&msg),AT_MSG_ID_TRANSFORM_TRANSMIT,0,NULL);
    xQueueSend(at_recv_cmd_queue,&msg,portMAX_DELAY);
    return ESP_AT_RESULT_CODE_IGNORE;
}

/**
 * @brief  Query commad of set multilink mode.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_queryCmdCipmux(uint8_t* cmd_name)
{
    uint8_t temp[32];
    at_sprintf(temp, "%s:%d\r\n",cmd_name, at_ipMux);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Setup commad of set multilink mode.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCipmux(uint8_t para_num)
{
    int32_t muxTemp = 0;
    int loop = 0;
    if(mdState == m_linked)
    {
        at_port_print((uint8_t*)"link is builded\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(para_num != 1)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(esp_at_get_para_as_digit (0, &muxTemp) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(muxTemp == 1)
    {
        if(at_ip_mode == TRUE)  // now serverEn is 0
        {
            at_port_print((uint8_t*)"IPMODE must be 0\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }

        at_ipMux = TRUE;
    }
    else if(muxTemp == 0)
    {
        if(tcp_server_socket >= 0)
        {
            at_port_print((uint8_t*)"CIPSERVER must be 0\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }

        for (loop = 0;loop < at_netconn_max_num;loop++) {
            if (pLink[loop].socket >= 0) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }

        if((at_ipMux == TRUE) && (mdState == m_linked))
        {
            at_port_print((uint8_t*)"Connection exists\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }
        at_ipMux = FALSE;
    }
    else
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Setup commad of module as server.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCipserver(uint8_t para_num)
{
    int32_t port = 0;
    int32_t server_enable = 0;
    int32_t cnt = 0;
    struct sockaddr_in sa;
    uint32_t optval = 0;
    socklen_t len = sizeof(optval);
    esp_at_para_parse_result_type result = ESP_AT_PARA_PARSE_RESULT_FAIL;

    if(para_num < 1)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(at_ipMux == FALSE)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (cnt++, &server_enable) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(server_enable == 1)
    {
        if(para_num != cnt)
        {
            result = esp_at_get_para_as_digit (cnt++, &port);
            if ((result != ESP_AT_PARA_PARSE_RESULT_OK) && (result != ESP_AT_PARA_PARSE_RESULT_OMITTED))
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            
            if (result == ESP_AT_PARA_PARSE_RESULT_OMITTED)
            {
                port = 333;
            }
            else if((port > 65535) || (port <= 0))
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
        else
        {
            port = 333;
        }
    }

    if(para_num != cnt)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(server_enable)
    {
        if(tcp_server_socket >= 0)
        {
            at_port_print((uint8_t*)"no change\r\n");
            return ESP_AT_RESULT_CODE_OK;
        }
        tcp_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(tcp_server_socket < 0)
        {
            at_printf("sock err:%d\r\n",tcp_server_socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }

        optval = 1;
        len = sizeof(optval);
        setsockopt(tcp_server_socket,SOL_SOCKET,SO_REUSEADDR,&optval,len);
        setsockopt(tcp_server_socket,SOL_SOCKET,SO_REUSEPORT,&optval,len);

        at_memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        sa.sin_addr.s_addr = htons(INADDR_ANY);
        if (bind(tcp_server_socket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
            uint32_t optval = 0;
            socklen_t len = sizeof(optval);
            at_port_print((uint8_t*)"bind fail\r\n");
            if(getsockopt(tcp_server_socket,SOL_SOCKET,SO_ERROR,&optval,&len) < 0)
            {
                at_port_print((uint8_t*)"get opt fail\r\n");
            }
            at_socket_server_cleanup_task(tcp_server_socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }

        if(listen(tcp_server_socket,at_netconn_max_num) < 0)
        {
            at_port_print((uint8_t*)"listen fail\r\n");
            at_socket_server_cleanup_task(tcp_server_socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }
        if (at_socket_server_create_task(tcp_server_socket) != TRUE)
        {
            at_port_print((uint8_t*)"try fail\r\n");
            at_socket_server_cleanup_task(tcp_server_socket);
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }
    else
    {
        if(tcp_server_socket < 0)
        {
            at_port_print((uint8_t*)"no change\r\n");
            return ESP_AT_RESULT_CODE_OK;
        }
        at_socket_server_cleanup_task(tcp_server_socket);
    }

    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Query commad of set transparent mode.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_queryCmdCipmode(uint8_t* cmd_name)
{
    uint8_t temp[32];

    at_sprintf(temp, "%s:%d\r\n", cmd_name, at_ip_mode);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  Setup commad of transparent.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdCipmode(uint8_t para_num)
{
    int32_t mode = 0;
    if(at_ipMux)
    {
        at_port_print((uint8_t*)"CIPMUX must be 0\r\n");
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(para_num != 1)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(esp_at_get_para_as_digit (0, &mode) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if((mode > 1) || (mode < 0))
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

#ifdef AT_SSL_SUPPORT
    if (pLink[0].ip_type == AT_SOCKET_TYPE_SSL) {
        at_port_print((uint8_t*)"link is ssl\r\n");
        return ESP_AT_RESULT_CODE_FAIL;
    }
#endif

    if ((pLink[0].socket >= 0) && (mode == 1)) {
        if ((pLink[0].ip_type == AT_SOCKET_TYPE_UDP)
            && (pLink[0].change_mode != 0)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        pLink[0].send_mode = AT_SEND_MODE_TRANSMIT;
        pLink[0].link_state = AT_LINK_TRANSMIT_CONNECTED;
    }

    at_ip_mode = mode;

    return ESP_AT_RESULT_CODE_OK;
}

/**
 * @brief  save commad of transparent.
 * @param  id: commad id number
 * @param  pPara: AT input param
 * @retval None
 */
uint8_t at_setupCmdSaveTransLink(uint8_t para_num)
{
    int32_t enable = 0;
    int32_t remote_port = 0;
    uint8_t* pStr = NULL;
    uint8_t* domain = NULL;
    uint32_t ipaddr = 0;
    uint8_t link_type = 0;
    uint8_t cnt = 0;
    int32_t keep_alive = 0;
    int32_t local_port = 0;
    int32_t change_mode = 0;
    at_nvm_struct nvm;;
    if(esp_at_get_para_as_digit (cnt++,&enable) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(enable == 1)
    {
        if(para_num == cnt)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(pStr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_INVALID(cnt));
            return ESP_AT_RESULT_CODE_ERROR;
        }
        ipaddr = inet_addr((char*)pStr);
        if (ipaddr == IPADDR_ANY)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if (ipaddr == IPADDR_NONE) {
            if (at_strcmp(pStr,"255.255.255.255") == 0) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }
        domain = pStr;

        if(para_num == cnt)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
        if(esp_at_get_para_as_digit (cnt++,&remote_port) != ESP_AT_PARA_PARSE_RESULT_OK)
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if((remote_port <=0) || (remote_port > 65535))
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if(para_num != cnt)
        {
            if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }

            if ((pStr == NULL) || (*pStr == '\0'))
            {
                link_type = AT_SOCKET_TYPE_TCP; // default
            }
            else if(at_strcmp(pStr,"TCP") == 0)
            {
                link_type = AT_SOCKET_TYPE_TCP;
            }
            else if(at_strcmp(pStr,"UDP") == 0)
            {
                link_type = AT_SOCKET_TYPE_UDP;
            }
            else
            {
                return ESP_AT_RESULT_CODE_ERROR;
            }

        }

        if (link_type == AT_SOCKET_TYPE_TCP) {
            // keep alive
            if(para_num != cnt)
            {
                if(esp_at_get_para_as_digit (cnt++,&keep_alive) != ESP_AT_PARA_PARSE_RESULT_OK)
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }

                if((keep_alive < 0) || (keep_alive > 7200)){
                    return ESP_AT_RESULT_CODE_ERROR;
                }
            }

            if(para_num != cnt) {
                if(esp_at_get_para_as_digit (cnt++,&local_port) != ESP_AT_PARA_PARSE_RESULT_OK)
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                if((local_port <= 0) || (local_port > 65535)){
                    return ESP_AT_RESULT_CODE_ERROR;
                }
            } else {
                local_port = 0;
            }

            if(para_num != cnt) {
                if(esp_at_get_para_as_digit (cnt++,&change_mode) != ESP_AT_PARA_PARSE_RESULT_OK)
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                if((change_mode != 0) && (change_mode != 1)){
                    return ESP_AT_RESULT_CODE_ERROR;
                }
            } else {
                change_mode = 1;
            }
        } else if (link_type == AT_SOCKET_TYPE_UDP) {
            // local port
            if(para_num != cnt)
            {
                if(esp_at_get_para_as_digit (cnt++,&local_port) != ESP_AT_PARA_PARSE_RESULT_OK)
                {
                    return ESP_AT_RESULT_CODE_ERROR;
                }
                if((local_port <= 0) || (local_port > 65535)){
                    return ESP_AT_RESULT_CODE_ERROR;
                }
            }
        }
        else
        {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if(para_num != cnt)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    at_memset(&nvm,0x0,sizeof(nvm));
    nvm.trans_config.enable = enable;
    nvm.trans_config.link_id = 0;
    nvm.trans_config.link_type = link_type;
    if (domain) {
        at_strncpy(nvm.trans_config.remote_ip,domain,sizeof(nvm.trans_config.remote_ip));
    }
    nvm.trans_config.remote_port = remote_port;
    nvm.trans_config.local_port = local_port;
    nvm.trans_config.change_mode = change_mode;
    nvm.trans_config.keep_alive = keep_alive;

    if (at_nvm_trans_config_set(&nvm.trans_config) == false) {
        return ESP_AT_RESULT_CODE_FAIL;
    }
    return ESP_AT_RESULT_CODE_OK;
}


static void at_server_timeout_cb( TimerHandle_t xTimer )
{
    uint8_t loop = 0;

    for(loop = 0; loop < at_netconn_max_num;loop++) {
        if((pLink[loop].socket >= 0) && (pLink[loop].terminal_type == AT_REMOTE_CLIENT)) {
            pLink[loop].server_timeout++;

            if (pLink[loop].server_timeout >= server_timeover) {
                at_socket_client_cleanup_task(&pLink[loop]);
            }
        }
    }
}

uint8_t at_queryCmdCipsto(uint8_t* cmd_name)
{
    uint8_t temp[32];
    at_sprintf(temp, "%s:%d\r\n",cmd_name, server_timeover);
    at_port_print(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipsto(uint8_t para_num)
{
    int32_t timeOver = 0;

    if(tcp_server_socket < 0) // not start server
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(para_num != 1)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(esp_at_get_para_as_digit (0, &timeOver) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if((timeOver > 7200) || (timeOver < 0)) // we set max timeout 7200s
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if(timeOver != server_timeover)
    {
        // TODO:set timeout
        server_timeover = timeOver;
        if (server_timeover == 0) {
            uint8_t loop = 0;
            for(loop = 0; loop < at_netconn_max_num;loop++) {
                if((pLink[loop].socket >= 0) && (pLink[loop].terminal_type == AT_REMOTE_CLIENT)) {
                    pLink[loop].server_timeout = 0;
                }
            }
            if (server_timeout_timer) {
                xTimerStop(server_timeout_timer,portMAX_DELAY);
                xTimerDelete(server_timeout_timer,portMAX_DELAY);
                server_timeout_timer = NULL;
            }
        } else {
            if (server_timeout_timer == NULL) {
                server_timeout_timer = xTimerCreate(
                        "sto",
                        1000/(1000/xPortGetTickRateHz()),
                        pdTRUE,
                        NULL,
                        at_server_timeout_cb);
                xTimerStart(server_timeout_timer,portMAX_DELAY);
            }
        }
    } else {
        at_port_print((uint8_t*)"not change\r\n");
    }
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipdomain(uint8_t para_num)
{
    uint8_t *pStr = NULL;
    ip_addr_t ip_address;
    struct hostent* hptr = NULL;
    uint8_t tmp[32];
    int32_t cnt = 0;

    if (para_num != 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    
    if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    ip_address.u_addr.ip4.addr = inet_addr((char*)pStr);

    if ((ip_address.u_addr.ip4.addr == IPADDR_NONE) && (at_strcmp(pStr,"255.255.255.255") != 0)) {
        if((hptr = gethostbyname((char*)pStr)) == NULL)
        {
            at_port_print((uint8_t*)"domain fail\r\n");
            return ESP_AT_RESULT_CODE_ERROR;
        }
        ip_address = *(ip_addr_t*)hptr->h_addr_list[0];
    }

    at_sprintf(tmp,"+CIPDOMAIN:"IPSTR"\r\n",IP2STR(&ip_address.u_addr.ip4));
    at_port_print(tmp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCipDns(uint8_t* cmd_name)
{
    ip_addr_t ip_address;
    uint8_t loop = 0;
    uint8_t tmp[32];


    for (loop = 0;loop < DNS_MAX_SERVERS;loop++) {
        ip_address = dns_getserver(loop);

        if (ip_address.u_addr.ip4.addr != IPADDR_ANY) {
            at_snprintf(tmp,sizeof(tmp),"%s:"IPSTR"\r\n",cmd_name,IP2STR(&ip_address.u_addr.ip4));
            at_port_print(tmp);
        }
    }
    
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdCipDnsDef(uint8_t* cmd_name)
{
    ip_addr_t ip_address;
    uint8_t loop = 0;
    uint8_t tmp[32];
    at_nvm_dns_server_struct nvm_dns_server;

    at_memset(&nvm_dns_server,0x0,sizeof(nvm_dns_server));

    if (at_nvm_dns_server_get(&nvm_dns_server) == false) {
        return ESP_AT_RESULT_CODE_OK;
    }
    
    for (loop = 0;loop < DNS_MAX_SERVERS;loop++) {
        ip_address.u_addr.ip4.addr = nvm_dns_server.dns_server[loop];

        if (ip_address.u_addr.ip4.addr != IPADDR_ANY) {
            at_snprintf(tmp,sizeof(tmp),"%s:"IPSTR"\r\n",cmd_name,IP2STR(&ip_address.u_addr.ip4));
            at_port_print(tmp);
        }
    }
    
    return ESP_AT_RESULT_CODE_OK;
}


uint8_t at_setupCmdCipDns(uint8_t para_num)
{
    uint8_t *pStr = NULL;
    ip_addr_t ip_address[DNS_MAX_SERVERS];
    int32_t cnt = 0;
    int32_t enable = 0;
    uint8_t loop = 0;
    at_nvm_dns_server_struct nvm_dns_server;

    at_memset(ip_address,0x0,sizeof(ip_address));
    at_memset(&nvm_dns_server,0x0,sizeof(nvm_dns_server));
    if(esp_at_get_para_as_digit (cnt++,&enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (enable == 1) {
        for (loop = 0;(loop < DNS_MAX_SERVERS) && (loop < (para_num - 1));loop++) {
            if(esp_at_get_para_as_str (cnt++,&pStr) != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            ip_address[loop].u_addr.ip4.addr = inet_addr((char*)pStr);

            if (ip_address[loop].u_addr.ip4.addr == IPADDR_NONE) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
        }

        if (loop == 0) {
#if (DNS_MAX_SERVERS >= 1)
            ip_address[0].u_addr.ip4.addr = 0xDEDE43D0;
#endif
#if (DNS_MAX_SERVERS >= 2)
            ip_address[1].u_addr.ip4.addr = 0xDCDC43D0;
#endif
        }
    } else if (enable == 0) {
#if 0
#if (DNS_MAX_SERVERS >= 1)
        ip_address[0].u_addr.ip4.addr = 0xDEDE43D0;
#endif
#if (DNS_MAX_SERVERS >= 2)
        ip_address[1].u_addr.ip4.addr = 0xDCDC43D0;
#endif
#endif
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    for (loop = 0;loop < DNS_MAX_SERVERS;loop++) {
        dns_setserver(loop,&ip_address[loop]);
        nvm_dns_server.dns_server[loop] = ip_address[loop].u_addr.ip4.addr;
    }

    if (at_ip_cmd_need_save_flag) {
        if (at_nvm_dns_server_set(&nvm_dns_server) == false) {
            return ESP_AT_RESULT_CODE_FAIL;
        }
    }    
    
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipDnsDef(uint8_t para_num)
{
    uint8_t ret = ESP_AT_RESULT_CODE_ERROR;

    at_ip_cmd_need_save_flag = true;
    ret = at_setupCmdCipDns(para_num);
    at_ip_cmd_need_save_flag = false;

    return ret;
}

void trans_config_reinit(void)
{
    at_task_msg msg;

    at_memset(&msg,0x0,sizeof(msg));
    at_form_msg_p((&msg),AT_MSG_ID_TRANSFORM_TRANSMIT,0,NULL);
    xQueueSend(at_recv_cmd_queue,&msg,portMAX_DELAY);
    at_port_print((uint8_t*)"\r\n>");
}

void trans_client_create(at_link_info_t *plink)
{
    uint32_t optval = 0;
    socklen_t len = sizeof(optval);

    if(AT_SOCKET_TYPE_TCP == plink->ip_type)
    {
        plink->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else if(AT_SOCKET_TYPE_UDP == plink->ip_type)
    {
        plink->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    at_printf("trans_client_connect:%d\r\n",plink->socket);
    if(plink->socket < 0)
    {
        return ;
    }

    if(AT_SOCKET_TYPE_TCP == plink->ip_type) {
        optval = 1;
        len = sizeof(optval);
        setsockopt(plink->socket,SOL_SOCKET,SO_REUSEADDR,&optval,len);
        setsockopt(plink->socket,SOL_SOCKET,SO_REUSEPORT,&optval,len);
    }

    if (plink->keep_alive) {
        optval = 1;
        setsockopt(plink->socket,SOL_SOCKET,SO_KEEPALIVE,&optval,len);
        optval = plink->keep_alive;
        setsockopt(plink->socket,IPPROTO_TCP,TCP_KEEPIDLE,&optval,len);
        optval = 1;
        setsockopt(plink->socket,IPPROTO_TCP,TCP_KEEPINTVL,&optval,len);
        optval = 3;
        setsockopt(plink->socket,IPPROTO_TCP,TCP_KEEPCNT,&optval,len);
    }
}

void trans_client_init(const at_nvm_trans_config_struct* trans_config)
{
    at_link_info_t *plink = NULL;  // must be single link

    if (trans_config == NULL) {
        return;
    }
    at_ip_mode = (trans_config->enable == 1) ? TRUE : FALSE;

    if(at_ip_mode != TRUE) {
        return;
    }

    if ((trans_config->link_id != 0)
            || ((trans_config->link_type != (uint8_t)AT_SOCKET_TYPE_TCP) && (trans_config->link_type != (uint8_t)AT_SOCKET_TYPE_UDP))
            || (at_strlen(trans_config->remote_ip) == 0)) {
        return;
    }

    plink = &pLink[trans_config->link_id];
    plink->change_mode = trans_config->change_mode;
    plink->keep_alive = trans_config->keep_alive;
    plink->local_port = (uint16_t)trans_config->local_port;
    at_memcpy(plink->domain,trans_config->remote_ip,sizeof(plink->domain));
    plink->remote_port = trans_config->remote_port;
    plink->send_mode = AT_SEND_MODE_TRANSMIT;
    plink->terminal_type = AT_LOCAL_CLIENT;
    plink->repeat_time = 0;
    plink->ip_type = trans_config->link_type;
    plink->link_state = AT_LINK_DISCONNECTED;

    trans_config_reinit();

    trans_client_create(plink);
    at_socket_client_create_task(plink);
}

int32_t at_get_trans_socket(void)
{
    return pLink[0].socket;
}

uint8_t at_queryCmdCipIpdInfo(uint8_t* cmd_name)
{
    uint8_t* temp = (uint8_t*)at_malloc(64);

    at_sprintf(temp, "%s:%s", cmd_name,ipd_info_enable?"TRUE":"FALSE");
    at_port_print(temp);
    at_free(temp);
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmdCipIpdInfo(uint8_t para_num)
{
    int32_t mode = 0;

    if (para_num != 1) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(esp_at_get_para_as_digit (0, &mode) != ESP_AT_PARA_PARSE_RESULT_OK)
    {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if(mode == 0) {
        ipd_info_enable = FALSE;
    } else if(mode == 1) {
        ipd_info_enable = TRUE;
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    return ESP_AT_RESULT_CODE_OK;
}

static void at_process_recv_socket(at_link_info_t* plink)
{
#define AT_SOCKET_RECV_BUF_MAX 2920
    int32_t len = 0;
    struct sockaddr_in sa;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int32_t sock = -1;
    uint8_t* buffer = NULL;
    uint8_t temp[40];

    if(plink == NULL)
    {
        return;
    }

    buffer = (uint8_t*)at_malloc(AT_SOCKET_RECV_BUF_MAX);
    if (buffer == NULL) {
        at_printf("alloc fail\r\n");
        return;
    }
    socklen = sizeof(struct sockaddr_in);
    at_memset(&sa,0x0,socklen);
    if (plink->ip_type == AT_SOCKET_TYPE_UDP) {
        len = recvfrom(plink->socket,buffer,AT_SOCKET_RECV_BUF_MAX,0,(struct sockaddr*)&sa,&socklen);
#ifdef AT_SSL_SUPPORT
    } else if (plink->ip_type == AT_SOCKET_TYPE_SSL) {
        len = SSL_read(plink->ssl,buffer,AT_SOCKET_RECV_BUF_MAX);
#endif
    } else {
        len = recv(plink->socket,buffer,AT_SOCKET_RECV_BUF_MAX,0);
    }
    if(len > 0)
    {
        if (at_ip_mode == TRUE) {
            esp_at_port_write_data(buffer, len);
        } else {
            uint8_t* data = (uint8_t*)at_malloc(len + 40);
            uint32_t header_len = 0;

            if (data) {
                if (plink->ip_type == AT_SOCKET_TYPE_UDP) {
                    if (plink->change_mode == 2) {
                        plink->remote_ip.addr = sa.sin_addr.s_addr;
                        plink->remote_port = htons(sa.sin_port);
                    } else if (plink->change_mode == 1) {
                        plink->remote_ip.addr = sa.sin_addr.s_addr;
                        plink->remote_port = htons(sa.sin_port);
                        plink->change_mode = 0;
                    }
                }

                if (ipd_info_enable == TRUE) {
                    if(at_ipMux) {
                        header_len = at_sprintf(data, "\r\n+IPD,%d,%d,"IPSTR",%d:",plink->idx, len,
                                IP2STR(&(plink->remote_ip)),ntohs(plink->remote_port));
                    }
                    else {
                        header_len = at_sprintf(data, "\r\n+IPD,%d,"IPSTR",%d:", len,
                                IP2STR(&(plink->remote_ip)),ntohs(plink->remote_port));

                    }
                } else {
                    if(at_ipMux) {
                        header_len = at_sprintf(data,"\r\n+IPD,%d,%d:",plink->idx, len);
                    } else {
                        header_len = at_sprintf(data,"\r\n+IPD,%d:",len);
                    }
                }

                at_memcpy(data + header_len,buffer,len);
                esp_at_port_write_data(data,header_len + len);
                at_free(data);
                data = NULL;

                if (plink->link_state != AT_LINK_WAIT_SENDING) {
                    plink->link_state = AT_LINK_CONNECTED;
                }
                if((plink->socket >= 0) && (plink->terminal_type == AT_REMOTE_CLIENT)) {
                    plink->server_timeout = 0;
                }
            } else {
                at_printf("alloc fail\r\n");
            }
        }
    } else {
        if(plink->socket >= 0) {
            for(;;) {
                uint32_t optval = 0;
                socklen_t optlen = sizeof(optval);

                if ((at_ip_mode != TRUE) || (plink->link_state != AT_LINK_TRANSMIT_SEND)) {
                    if (at_ipMux == TRUE) {
                        at_sprintf(temp,"%d,CLOSED\r\n", plink->idx);
                    } else {
                        at_sprintf(temp,"CLOSED\r\n");
                    }
                    at_port_print(temp);

                    if (plink->link_state != AT_LINK_DISCONNECTING) {
#ifdef AT_SSL_SUPPORT
                        if (AT_SOCKET_TYPE_SSL == plink->ip_type) {
                            SSL_shutdown(plink->ssl);
                            SSL_free(plink->ssl);
                            plink->ssl = NULL;
                        }
#endif
	                    sock = plink->socket;
	                    plink->socket = -1;
	                    plink->ip_type = AT_SOCKET_TYPE_INVALID;
	                    if (sock >= 0) {
	                        close(sock);
	                    }
                        plink->link_state = AT_LINK_DISCONNECTED;
                	}
                    break;
                } else {
                    sock = plink->socket;
                    plink->socket = -1;
                    plink->ip_type = AT_SOCKET_TYPE_INVALID;
                    if (sock >= 0) {
                        close(sock);
                    }
                    plink->link_state = AT_LINK_DISCONNECTED;
                    plink->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    if (plink->socket < 0) {
                        vTaskDelay(50/(1000 /xPortGetTickRateHz()));
                        plink->link_state = AT_LINK_TRANSMIT_SEND;
                        continue;
                    }

                    optval = 1;
                    optlen = sizeof(optval);
                    setsockopt(plink->socket,SOL_SOCKET,SO_REUSEADDR,&optval,optlen);
                    setsockopt(plink->socket,SOL_SOCKET,SO_REUSEPORT,&optval,optlen);
                    if (plink->keep_alive) {
                        optval = 1;
                        setsockopt(plink->socket,SOL_SOCKET,SO_KEEPALIVE,&optval,len);
                        optval = plink->keep_alive;
                        setsockopt(plink->socket,IPPROTO_TCP,TCP_KEEPIDLE,&optval,len);
                        optval = 1;
                        setsockopt(plink->socket,IPPROTO_TCP,TCP_KEEPINTVL,&optval,len);
                        optval = 3;
                        setsockopt(plink->socket,IPPROTO_TCP,TCP_KEEPCNT,&optval,len);
                    }

                    at_memset(&sa, 0, sizeof(sa));
                    sa.sin_family = AF_INET;
                    sa.sin_port = htons(plink->remote_port);
                    sa.sin_addr.s_addr = plink->remote_ip.addr;
                    plink->link_state = AT_LINK_CONNECTING;
                    if (connect(plink->socket,(struct sockaddr *)&sa,sizeof(sa)) < 0) {
                        vTaskDelay(50/(1000 /xPortGetTickRateHz()));
                        plink->link_state = AT_LINK_TRANSMIT_SEND;
                        continue;
                    }

                    if (plink->socket >= 0) {
                        plink->link_state = AT_LINK_TRANSMIT_SEND;
                        break;
                    }
                }
            }
        }
    }
    at_free(buffer);
}
static void at_socket_process_task( void *pvParameters )
{

    struct sockaddr_in sa;
    uint32_t addr = 0;
    at_link_info_t* plink = (at_link_info_t*)pvParameters;
    struct hostent* hptr = NULL;
    ip_addr_t ip_address;
    bool link_count_flag = false;

    if (plink == NULL) {
        vTaskDelete(NULL);
    }

    xSemaphoreTake(plink->sema,0); // just try take it
    if ((plink->ip_type == AT_SOCKET_TYPE_TCP) || (plink->ip_type == AT_SOCKET_TYPE_SSL)) {
        link_count_flag = true;
        at_update_link_count(1);
    }
    if ((plink->send_mode == AT_SEND_MODE_TRANSMIT) && (plink->link_state == AT_LINK_DISCONNECTED)) {
        for (;;) {
            at_memset(&sa, 0, sizeof(sa));
            if (plink->ip_type == AT_SOCKET_TYPE_UDP) {
                sa.sin_family = AF_INET;
                sa.sin_port = htons(plink->local_port);
                sa.sin_addr.s_addr = htonl(INADDR_ANY);
                if (bind(plink->socket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
                    vTaskDelay(100/(1000 /xPortGetTickRateHz()));
                    if (at_ip_mode != TRUE) {
                        close(plink->socket);
                        plink->socket = -1;
                        plink->ip_type = AT_SOCKET_TYPE_INVALID;
                        break;
                    }
                } else {
                    plink->link_state = AT_LINK_TRANSMIT_SEND;
                    break;
                }
            } else {
                sa.sin_family = AF_INET;
                sa.sin_port = htons(plink->remote_port);
                addr = inet_addr((char*)plink->domain);

                if (addr == IPADDR_ANY) {
                    close(plink->socket);
                    plink->socket = -1;
                    plink->ip_type = AT_SOCKET_TYPE_INVALID;
                    break;
                }
                if ((addr == IPADDR_NONE) && (at_strcmp(plink->domain,"255.255.255.255") != 0)) {
                    if((hptr = gethostbyname((char*)plink->domain)) == NULL) {
                        vTaskDelay(1000/(1000 /xPortGetTickRateHz()));
                        if (at_ip_mode != TRUE) {
                            close(plink->socket);
                            plink->socket = -1;
                            plink->ip_type = AT_SOCKET_TYPE_INVALID;
                            break;
                        } else {
                            continue;
                        }
                    }

                    ip_address = *(ip_addr_t*)hptr->h_addr_list[0];
                    addr = ip_address.u_addr.ip4.addr;
                }

                plink->remote_ip.addr = addr;
                sa.sin_addr.s_addr = addr;
                if (connect(plink->socket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
                    at_printf("connect fail\r\n");
                    close(plink->socket);
                    vTaskDelay(1000/(1000 /xPortGetTickRateHz()));
                    if (at_ip_mode != TRUE) {
                        plink->socket = -1;
                        plink->link_state = AT_LINK_DISCONNECTED;
                        break;
                    }
                    trans_client_create(plink);
                } else {
                    at_printf("connect ok\r\n");
                    plink->link_state = AT_LINK_TRANSMIT_SEND;
                    break;
                }
                plink->link_state = AT_LINK_CONNECTED;
            }
        }
    }

    for(;;)
    {
        if ((plink->link_state == AT_LINK_DISCONNECTED)
             || (plink->link_state == AT_LINK_DISCONNECTING)) {
            break;
        }

        at_process_recv_socket(plink);
    }

    if (link_count_flag) {
        at_update_link_count(-1);
    }
    at_printf("delete\r\n");

    if(plink->link_state == AT_LINK_DISCONNECTING) {
    	xSemaphoreGive(plink->sema);
        plink->link_state = AT_LINK_DISCONNECTED;
    }
    vTaskDelete(NULL);
}

static void at_socket_server_listen_task( void *pvParameters )
{
    int32_t loop = 0;
    int32_t sock = -1;
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    uint32_t addr = 0;
    int32_t client_sock = -1;
    uint8_t tmp[16];

    if (pvParameters == NULL) {
        vTaskDelete(NULL);
    }

    sock = *(uint32_t*)pvParameters;

    if (sock < 0) {
        vTaskDelete(NULL);
    }
    for(;;)
    {
        client_sock = accept(tcp_server_socket,(struct sockaddr *)&sa,&len);
        if(client_sock >= 0)
        {
            AT_SOCKET_LOCK();
            for(loop = 0;loop < at_netconn_max_num;loop++)
            {
                if(pLink[loop].socket < 0) {
                    break;
                }
            }
            
            if(loop < at_netconn_max_num)
            {
                pLink[loop].link_state = AT_LINK_CONNECTED;
                addr = ntohl(sa.sin_addr.s_addr);
                pLink[loop].remote_ip.addr = ntohl(addr);
                pLink[loop].remote_port = ntohs(sa.sin_port);
                pLink[loop].repeat_time = 0;
                pLink[loop].socket = client_sock;
                pLink[loop].terminal_type = AT_REMOTE_CLIENT;
                pLink[loop].server_timeout = 0;
                at_socket_client_create_task(&pLink[loop]);
                AT_SOCKET_UNLOCK();
                at_sprintf(tmp,"%d,CONNECT\r\n",loop);
                at_port_print(tmp);
            }
            else
            {
                AT_SOCKET_UNLOCK();
                at_sprintf(tmp,"connect reach max\r\n");
                at_port_print(tmp);
                close(client_sock);
            }
        } else {
            break;
        }
    }

	if (tcp_server_socket >= 0) {
        close(tcp_server_socket);
        tcp_server_socket = -1;
    }
	
	at_tcp_server_task_handle = NULL;
    vTaskDelete(NULL);
}

bool at_socket_client_create_task(at_link_info_t* plink)
{
    uint8_t task_name[10];

    if (plink == NULL) {
        return FALSE;
    }

    at_sprintf(task_name,"socket%d",plink->idx);
    if(pdPASS == xTaskCreate(at_socket_process_task,(const char * const)task_name,2048,plink,AT_SOCKET_TASK_PRIORITIE,&plink->task_handle)) {
        return TRUE;
    } else {
        at_printf("create socket fail\r\n");
        return FALSE;
    }
}


bool at_socket_client_cleanup_task(at_link_info_t* plink)
{
    if (plink == NULL) {
        return FALSE;
    }
    AT_SOCKET_LOCK();

    if (plink->socket >= 0) {
        plink->link_state = AT_LINK_DISCONNECTING;
#ifdef AT_SSL_SUPPORT
        if (plink->ip_type == AT_SOCKET_TYPE_SSL) {
            SSL_shutdown(plink->ssl);
            SSL_free(plink->ssl);
            plink->ssl = NULL;
        }
#endif
        close(plink->socket);
        xSemaphoreTake(plink->sema,portMAX_DELAY);
        plink->socket = -1;
        plink->ip_type = AT_SOCKET_TYPE_INVALID;
    }
    AT_SOCKET_UNLOCK();
    return TRUE;
}

bool at_socket_server_create_task(int32_t sock)
{
    if ((sock < 0) || (sock != tcp_server_socket) || (at_tcp_server_task_handle != NULL)) {
        return FALSE;
    }

    if(pdPASS == xTaskCreate(at_socket_server_listen_task,"server",2048,&tcp_server_socket,AT_SOCKET_TASK_PRIORITIE,&at_tcp_server_task_handle)) {
        return TRUE;
    } else {
        return FALSE;
    }
}


bool at_socket_server_cleanup_task(int32_t sock)
{
	at_printf("at_socket_server_cleanup_task\r\n");
    if ((sock < 0) || (sock != tcp_server_socket)) {
        return FALSE;
    }

	at_printf("at_tcp_server_task_handle:%p\r\n",at_tcp_server_task_handle);
#if 0
    if (at_tcp_server_task_handle) {
        vTaskDelete(at_tcp_server_task_handle);
        at_tcp_server_task_handle = NULL;
    }
#endif
	at_printf("tcp_server_socket:%d\r\n",tcp_server_socket);
    if (tcp_server_socket >= 0) {
        close(tcp_server_socket);
        tcp_server_socket = -1;
    }
	at_printf("tcp_server_socket:return\r\n");
    return TRUE;
}


/**
 * @}
 */
