/*
 * File : at.h
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
#ifndef __AT_H
#define __AT_H

#include <stdlib.h>
#include <stdio.h>
#include "esp_types.h"
#include "string.h"
#include "malloc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_attr.h"


#include "lwip/ip4_addr.h"
#include "lwip/dns.h"

#include "tcpip_adapter.h"

#include "at_version.h"

#define AT_SMARTCONFIG_SUPPORT
#define AT_WPS_SUPPORT
#define AT_SLEEP_SUPPORT
#define AT_SCAN_SUPPORT
#define AT_SCAN_SORT_SUPPORT
#define AT_LIST_IF_SUPPORT
#define AT_DHCP_SUPPORT
// #define AT_CMD_OPTIMIZE
#define AT_CMD_NO_OPTIMIZE
#define AT_SSL_SUPPORT


typedef enum {
    at_statIdle,
    at_statRecving,
    at_statProcess,
    at_statIpSending,
    at_statIpSended,
    at_statIpTraning
} at_stateType;

typedef enum {
    m_inactive,
    m_idle,
    m_gotip,
    m_linked,
    m_unlink,
    m_disconnet
} at_mdStateType;


typedef struct {
    char *at_cmdName;
    // int8  at_cmdLen;
    uint8_t (*at_testCmd)(uint8_t *cmd_name);
    uint8_t (*at_queryCmd)(uint8_t *cmd_name);
    uint8_t (*at_setupCmd)(uint8_t para_num);
    uint8_t (*at_exeCmd)(uint8_t *cmd_name);
} esp_at_cmd_struct;


typedef struct {
    uint16_t msg_id;
    uint16_t len;
    void *data;
    uint32_t time_stamp;
//  void* private_data;
} at_task_msg;

typedef struct {
    int32_t (*read_data) (uint8_t *data, int32_t len);              // read data from device
    int32_t (*write_data)(uint8_t *data, int32_t len);              // write data into device

    int32_t (*get_data_length)(void);                                    // get the length of data received
    bool (*wait_write_complete)(int32_t timeout_msec);
} esp_at_device_ops_struct;

typedef enum {
    ESP_AT_STATUS_NORMAL = 0x0,                /*!< Normal mode.Now mcu can send AT command */
    ESP_AT_STATUS_TRANSMIT,                    /*!< Transparent Transmition mode */
} esp_at_status_type;

/**
 * @brief esp_at_ops_struct
 *  some custom function interacting with AT
 *
 */
typedef struct {
    void (*status_callback) (esp_at_status_type status);              /*!< callback when AT status changes */

} esp_at_custom_ops_struct;

typedef enum {
    // at task handled msg id
    AT_MSG_ID_CMD_DATA,
    AT_MSG_ID_SEND_DATA,
    AT_MSG_ID_TRANSMIT_DATA,
    AT_MSG_ID_RESULT_CODE,
    AT_MSG_ID_RECEIVE_SOCKET,

    //
    AT_MSG_ID_AT_PORT_RECV,

    // at uart handled task msg id
    AT_MSG_ID_UART_RECV_TIMEOUT,
    AT_MSG_ID_UART_RX,
    AT_MSG_ID_TRANSFORM_IDLE,
    AT_MSG_ID_TRANSFORM_SENDING,
    AT_MSG_ID_TRANSFORM_TRANSMIT,
    AT_MSG_ID_TRANSMIT_TERMINAL,
    AT_MSG_ID_TRANSMIT_DONE,
    AT_MSG_ID_CMD_PROCESSING_DONE,
    AT_MSG_ID_SEND_DONE,
} at_msg_id;

// error number
// module number
typedef enum {
    ESP_AT_MODULE_NUM = 0x01
} esp_at_module;
// subcategory number
typedef enum {
    ESP_AT_SUB_OK                       = 0x00,
    ESP_AT_SUB_COMMON_ERROR             = 0x01,
    ESP_AT_SUB_NO_TERMINATOR            = 0x02,
    ESP_AT_SUB_NO_AT                    = 0x03,
    ESP_AT_SUB_PARA_LENGTH_MISMATCH    = 0x04,
    ESP_AT_SUB_PARA_TYPE_MISMATCH     = 0x05,
    ESP_AT_SUB_PARA_NUM_MISMATCH       = 0x06,
    ESP_AT_SUB_PARA_INVALID             = 0x07,
    ESP_AT_SUB_PARA_PARSE_FAIL          = 0x08,
    ESP_AT_SUB_UNSUPPORT_CMD            = 0x09,
    ESP_AT_SUB_CMD_EXEC_FAIL            = 0x0A,
    ESP_AT_SUB_CMD_PROCESSING           = 0x0B,
   
} esp_at_error_code;

typedef enum {
    ESP_AT_RESULT_CODE_OK           = 0x00,
    ESP_AT_RESULT_CODE_ERROR        = 0x01,
    ESP_AT_RESULT_CODE_FAIL         = 0x02,
    ESP_AT_RESULT_CODE_SEND_OK      = 0x03,
    ESP_AT_RESULT_CODE_SEND_FAIL    = 0x04,
    ESP_AT_RESULT_CODE_IGNORE       = 0x05,
    ESP_AT_RESULT_CODE_PROCESS_DONE = 0x06, // not response string

    ESP_AT_RESULT_CODE_MAX
} esp_at_result_code_string_index;


#define ESP_AT_ERROR_NO(subcategory,extension)  \
    ((ESP_AT_MODULE_NUM << 24) | ((subcategory) << 16) | (extension))

#define ESP_AT_CMD_ERROR_OK                                                         ESP_AT_ERROR_NO(ESP_AT_SUB_OK,0x00)
#define ESP_AT_CMD_ERROR_NON_FINISH                                          ESP_AT_ERROR_NO(ESP_AT_SUB_NO_TERMINATOR,0x00)
#define ESP_AT_CMD_ERROR_NOT_FOUND_AT                                  ESP_AT_ERROR_NO(ESP_AT_SUB_NO_AT,0x00)
#define ESP_AT_CMD_ERROR_PARA_LENGTH(which_para)             ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_LENGTH_MISMATCH,which_para)
#define ESP_AT_CMD_ERROR_PARA_TYPE(which_para)                  ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_TYPE_MISMATCH,which_para)
#define ESP_AT_CMD_ERROR_PARA_NUM(need,given)                   ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_NUM_MISMATCH,(((need) << 8) | (given)))
#define ESP_AT_CMD_ERROR_PARA_INVALID(which_para)            ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_INVALID,which_para)
#define ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(which_para)     ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_PARSE_FAIL,which_para)
#define ESP_AT_CMD_ERROR_CMD_UNSUPPORT                              ESP_AT_ERROR_NO(ESP_AT_SUB_UNSUPPORT_CMD,0x00)
#define ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(result)                  ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_EXEC_FAIL,result)
#define ESP_AT_CMD_ERROR_CMD_PROCESSING                            ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_PROCESSING,0x00)

void esp_at_module_init(uint32_t netconn_max, const uint8_t *custom_version);

#define at_form_msg_p(msg,id,data_len,data_p) do {\
    (msg)->msg_id = id;\
    (msg)->len = data_len;\
    (msg)->data = data_p;\
    (msg)->time_stamp = xTaskGetTickCount(); \
    }while(0)

#define at_form_msg_p_from_isr(msg,id,data_len,data_p) do {\
    (msg)->msg_id = id;\
    (msg)->len = data_len;\
    (msg)->data = data_p;\
    (msg)->time_stamp = xTaskGetTickCountFromISR(); \
    }while(0)

typedef enum {
    ESP_AT_PARA_PARSE_RESULT_FAIL = -1,
    ESP_AT_PARA_PARSE_RESULT_OK = 0,
    ESP_AT_PARA_PARSE_RESULT_OMITTED,
} esp_at_para_parse_result_type;


int32_t esp_at_port_read_data (uint8_t*buf,int32_t len);
int32_t  esp_at_port_write_data(uint8_t *data, int32_t len);
bool esp_at_port_wait_write_complete (int32_t timeout_msec);
int32_t esp_at_port_get_data_length (void);
void at_port_print(uint8_t *str);


void at_handle_restart(void);
void esp_at_printf_error_code(uint32_t err_code);

esp_at_para_parse_result_type esp_at_get_para_as_digit(int32_t para_index, int32_t *result);
esp_at_para_parse_result_type esp_at_get_para_as_str(int32_t para_index, uint8_t **result);
void esp_at_response_result (uint8_t result_code);

uint32_t at_cmd_process(uint8_t *pStr, uint16_t cmd_len);
void at_nvm_init(void);

void esp_at_port_recv_data_notify_from_isr(int32_t len);

bool esp_at_port_recv_data_notify (int32_t len, uint32_t msec);

void esp_at_transmit_terminal_from_isr(void);

void esp_at_transmit_terminal(void);

void esp_at_custom_cmd_array_regist(esp_at_cmd_struct *custom_at_cmd_array, uint32_t cmd_num);

void esp_at_custom_status_callback (esp_at_status_type status);

void esp_at_clear_recv_queue(void);

#ifndef TRUE
#define TRUE            true
#endif

#ifndef FALSE
#define FALSE           false
#endif

typedef enum {
    AT_SOCKET_TYPE_INVALID = -1,
    AT_SOCKET_TYPE_TCP = 0,
    AT_SOCKET_TYPE_UDP,
#ifdef AT_SSL_SUPPORT 
    AT_SOCKET_TYPE_SSL,
#endif
} at_socket_type;

extern xQueueHandle at_recv_cmd_queue;
extern xQueueHandle at_recv_data_queue;
extern volatile at_mdStateType mdState;

#include "at_baseCmd.h"
#include "at_ipCmd.h"
#include "at_wifiCmd.h"

#define at_printf(fmt,...)          printf(fmt,##__VA_ARGS__)
#define at_printf_isr(fmt,...)      //os_printf_isr(fmt,##__VA_ARGS__)
#define at_strlen(s)                strlen((char*)(s))
#define at_strcpy(dest,src)         strcpy((char*)(dest),(char*)(src))
#define at_strncpy(dest,src,n)      strncpy((char*)(dest),(char*)(src),n)
#define at_strcmp(s1,s2)            strcmp((char*)(s1),(char*)(s2))
#define at_strncmp(s1,s2,n)         strncmp((char*)(s1),(char*)(s2),n)
#define at_strstr(s1,s2)            strstr((char*)(s1),(char*)(s2))
#define at_sprintf(s,...)           sprintf((char*)(s), ##__VA_ARGS__)
#define at_snprintf(s,size,...)     snprintf((char*)(s),size, ##__VA_ARGS__)
#define at_memset(dest,x,n)         memset(dest,x,n)
#define at_memcpy(dest,src,n)       memcpy(dest,src,n)
#define at_memcmp(s1,s2,n)          memcmp(s1,s2,n)
#define at_malloc(size)             malloc(size)
#define at_free(ptr)                free(ptr)


#define at_enter_critical()
#define at_exit_critical()

int8_t wifi_station_get_connect_status(void);
void wifi_station_init_connect_status(void);

#define STATION_IDLE                    0
#define STATION_CONNECTING              1
#define STATION_WRONG_PASSWORD          2
#define STATION_NO_AP_FOUND             3
#define STATION_CONNECT_FAIL            4
#define STATION_GOT_IP                  5

#endif
