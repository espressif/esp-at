/*
 * File : at_ipCmd.h
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
#ifndef __AT_IPCMD_H
#define __AT_IPCMD_H

#include "at.h"

#ifdef AT_SSL_SUPPORT
#include "openssl/ssl.h" 
#endif

typedef enum {
    AT_LOCAL_CLIENT = 0,
    AT_REMOTE_CLIENT
} at_terminal_type;

typedef enum {
    AT_LINK_DISCONNECTED = 0,
    AT_LINK_CONNECTING,
    AT_LINK_CONNECTED,
    AT_LINK_WAIT_SENDING,
    AT_LINK_SENDING,
    AT_LINK_WAIT_SENDING_CB,
    AT_LINK_TRANSMIT_CONNECTED,
    AT_LINK_TRANSMIT_SEND,
    AT_LINK_TRANSMIT_NON_CONN, // now is transmiting mode ,but link is disconnected
    AT_LINK_DISCONNECTING,
    AT_LINK_CHANGED
} at_link_state;

enum {
    AT_SEND_MODE_IDLE = 0,
    AT_SEND_MODE_SEND,
    AT_SEND_MODE_SENDEX,
    AT_SEND_MODE_SENDBUF,
    AT_SEND_MODE_TRANSMIT,
};

enum {
    AT_DEST_PORT_CHANGE_NEVER = 0,
    AT_DEST_PORT_CHANGE_ONCE,
    AT_DEST_PORT_CHANGE_ALWAYS
};


typedef struct {
    at_link_state link_state;
    at_terminal_type terminal_type;
    uint8_t idx;
    uint8_t send_mode;
    uint8_t ip_type;
    uint8_t repeat_time;
    uint8_t change_mode;
    ip4_addr_t remote_ip;
    uint8_t domain[64];
    uint16_t remote_port;
    uint16_t local_port;

    int32_t socket;
    int32_t keep_alive;
    int32_t server_timeout;
#ifdef AT_SSL_SUPPORT
    SSL* ssl;
#endif
    xTaskHandle task_handle;
    SemaphoreHandle_t sema;
} at_link_info_t;

//#define at_netconn_max_num (MEMP_NUM_NETCONN - 1)
extern uint8_t at_netconn_max_num;
extern at_link_info_t *pLink;
extern volatile bool at_ipMux;


uint8_t at_testCmdCifsr(uint8_t *cmd_name);
uint8_t at_setupCmdCifsr(uint8_t para_num);
uint8_t at_exeCmdCifsr(uint8_t *cmd_name);

uint8_t at_testCmdCipstatus(uint8_t *cmd_name);
uint8_t at_exeCmdCipstatus(uint8_t *cmd_name);

uint8_t at_setupCmdCipstart(uint8_t para_num);

uint8_t at_setupCmdCipclose(uint8_t para_num);
uint8_t at_exeCmdCipclose(uint8_t *cmd_name);

uint8_t at_testCmdCipsend(uint8_t *cmd_name);
uint8_t at_setupCmdCipsend(uint8_t para_num);
uint8_t at_exeCmdCipsend(uint8_t *cmd_name);

uint8_t at_queryCmdCipmux(uint8_t *cmd_name);
uint8_t at_setupCmdCipmux(uint8_t para_num);

uint8_t at_setupCmdCipserver(uint8_t para_num);

uint8_t at_queryCmdCipmode(uint8_t *cmd_name);
uint8_t at_setupCmdCipmode(uint8_t para_num);

uint8_t at_queryCmdCipsto(uint8_t *cmd_name);
uint8_t at_setupCmdCipsto(uint8_t para_num);

uint8_t at_setupCmdCipdomain(uint8_t para_num);

uint8_t at_exeCmdCiping(uint8_t *cmd_name);

uint8_t at_exeCmdCipappup(uint8_t *cmd_name);

uint8_t at_setupCmdSaveTransLink(uint8_t para_num);

int32_t at_transparent_transmit(uint8_t *data, int32_t len);

uint8_t at_queryCmdCipIpdInfo(uint8_t *cmd_name);
uint8_t at_setupCmdCipIpdInfo(uint8_t para_num);

uint8_t at_setupCmdCipsendEx(uint8_t para_num);

uint8_t at_queryCmdCipDns(uint8_t* cmd_name);
uint8_t at_queryCmdCipDnsDef(uint8_t* cmd_name);
uint8_t at_setupCmdCipDns(uint8_t para_num);
uint8_t at_setupCmdCipDnsDef(uint8_t para_num);

bool at_socket_client_create_task(at_link_info_t *plink);
bool at_socket_client_cleanup_task(at_link_info_t *plink);
bool at_socket_server_create_task(int32_t sock);
bool at_socket_server_cleanup_task(int32_t sock);

void at_transparent_transmit_preprocess(uint8_t *data, int32_t len);
void esp_at_transmit_terminal(void);

uint8_t at_sending_data(uint8_t *data, uint32_t len);
void at_update_link_count(int8_t count);
void at_netconn_init(uint32_t netconn_max);



#endif
