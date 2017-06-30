/*
 * File : at_port.c
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
#include "esp_event.h"
#include "esp_system.h"
#include "at.h"
#include "at_cmd.h"
#include "at_ipCmd.h"
#include "lwip/sockets.h"
#include "esp_event_loop.h"

#include "esp_wps.h"

/** @defgroup AT_PORT_Defines
 * @{
 */
/**
 * @}
 */
extern bool at_ip_mode;
extern uint8_t at_wifiMode;
extern at_link_info_t *pLink;
extern uint8_t at_netconn_max_num;

#define AT_CMD_MAX_SIZE 128
enum {
    AT_STATE_IDLE,      // idle
    AT_STATE_PROCESSING,// at mode
    AT_STATE_SENDING_RECV,
    AT_STATE_WAIT_SEND_DONE,    // sending data mode
    AT_STATE_TRANSMIT   // transmit transparently
};

#define AT_RECV_TASK_NAME "at_recv_task"
#define AT_RECV_TASK_PRIORITIE 6
#define AT_RECV_TASK_STACK_DEPTH  (1024*4)
#define AT_RECV_DATA_QUEUE_DEPTH    20
#define AT_RECV_CMD_QUEUE_DEPTH     10
#define AT_PROCESS_QUEUE_DEPTH      10
xQueueHandle at_recv_data_queue;
xQueueHandle at_recv_cmd_queue;
QueueSetHandle_t at_recv_set;

#define AT_TASK_NAME "at_process_task"
#define AT_TASK_PRIORITIE 4//5
#define AT_TASK_STACK_DEPTH (1024*6)
xQueueHandle at_process_queue;
static EventGroupHandle_t wifi_event_group;
static bool esp_at_transmit_flag_exit = false;

//at global variables
static uint8_t *at_result_string[ESP_AT_RESULT_CODE_MAX] = {
    (uint8_t *)"\r\nOK\r\n",        //ESP_AT_RESULT_CODE_OK         = 0x00,
    (uint8_t *)"\r\nERROR\r\n",     //ESP_AT_RESULT_CODE_ERROR      = 0x01,
    (uint8_t *)"\r\nERROR\r\n",         //ESP_AT_RESULT_CODE_FAIL       = 0x02,
    (uint8_t *)"\r\nSEND OK\r\n",       //ESP_AT_RESULT_CODE_SEND_OK    = 0x03,
    (uint8_t *)"\r\nSEND FAIL\r\n", // ESP_AT_RESULT_CODE_SEND_FAIL  = 0x04,
    NULL,
    NULL,
};

void at_send_busy(void)
{
    at_port_print((uint8_t *)"\r\nbusy p...\r\n");
}

void at_handle_result_code(uint8_t result_code, void *pbuff)
{
    at_task_msg send_msg;
    if (result_code < ESP_AT_RESULT_CODE_MAX) {
        at_port_print(at_result_string[result_code]);

        switch (result_code) {
        case ESP_AT_RESULT_CODE_SEND_OK:
        case ESP_AT_RESULT_CODE_SEND_FAIL:
        case ESP_AT_RESULT_CODE_OK:
        case ESP_AT_RESULT_CODE_ERROR:
        case ESP_AT_RESULT_CODE_FAIL:
        case ESP_AT_RESULT_CODE_PROCESS_DONE:
            at_form_msg_p((&send_msg), AT_MSG_ID_CMD_PROCESSING_DONE, 0, NULL);

            xQueueSend( at_recv_cmd_queue, &send_msg, portMAX_DELAY );
            break;
        default:
            break;
        }
    }
}

void at_handle_restart(void)
{
    at_port_print(at_result_string[ESP_AT_RESULT_CODE_OK]);//"OK"
    esp_at_port_wait_write_complete(portMAX_DELAY);
    esp_restart();
}

void esp_at_response_result(uint8_t result_code)
{
    if (result_code < ESP_AT_RESULT_CODE_MAX) {
        at_port_print(at_result_string[result_code]);//"OK"
    }
}

#define AT_DATA_BUFFER_MAX_SIZE     (8*1024)
#define AT_TRANSMIT_MAX_SIZE     (2920)
uint8_t p_buffer[AT_DATA_BUFFER_MAX_SIZE + AT_CMD_MAX_SIZE + 1];
uint8_t *p_data = p_buffer;

void esp_at_clear_recv_queue(void)
{
    at_task_msg msg;
    while (xQueueReceive(at_recv_data_queue, &(msg), (TickType_t)0 )) {
        esp_at_port_read_data(NULL, esp_at_port_get_data_length());
    }
    esp_at_transmit_flag_exit = true;
}

static void at_recv_task( void *pvParameters )
{
    at_task_msg msg;
    at_task_msg send_msg;
    uint8_t at_state = AT_STATE_IDLE;
    uint8_t *p_data_head = NULL;          // search AT
    uint8_t *p_data_tail = NULL;          // search \r\n

    int32_t len = 0;
    uint32_t timeout_tick = portMAX_DELAY;
    uint32_t send_total_len = 0;
    bool process_continue = FALSE;
    QueueSetMemberHandle_t setmemberhandle = NULL;
    bool transmit_done = TRUE;

    for (;;) {
        setmemberhandle = xQueueSelectFromSet(at_recv_set, timeout_tick);
        if (at_recv_cmd_queue == setmemberhandle) {
            xQueueReceive(at_recv_cmd_queue, (void *)&msg, 0);
        } else if (at_recv_data_queue == setmemberhandle) {
            xQueueReceive(at_recv_data_queue, (void *)&msg, 0);
        } else {
            if (esp_at_transmit_flag_exit) {
                at_ip_mode = false;
                at_state = AT_STATE_IDLE;
                p_data = p_buffer;
                p_data_tail = NULL;
                p_data_head = NULL;
                timeout_tick = portMAX_DELAY;  // forever
                transmit_done = true;
                esp_at_custom_status_callback(ESP_AT_STATUS_NORMAL);
                esp_at_transmit_flag_exit = false;
            } else if (at_state == AT_STATE_TRANSMIT) {  // timeout
                if ((p_data - p_buffer > 0) && (transmit_done == TRUE)) {
                    at_transparent_transmit_preprocess(p_buffer, p_data - p_buffer);
                    transmit_done = FALSE;
                    p_data = p_buffer;
                }
                timeout_tick = portMAX_DELAY;
            }
            continue;
        }

        if (esp_at_transmit_flag_exit) {
            at_ip_mode = false;
            at_state = AT_STATE_IDLE;
            p_data = p_buffer;
            p_data_tail = NULL;
            p_data_head = NULL;
            timeout_tick = portMAX_DELAY;  // forever
            transmit_done = true;
            esp_at_custom_status_callback(ESP_AT_STATUS_NORMAL);
            esp_at_transmit_flag_exit = false;
        }

        switch (msg.msg_id) {
        case AT_MSG_ID_AT_PORT_RECV: {
            len = esp_at_port_get_data_length();
            if (len < 0) {
                len = msg.len;
            }

            if (len <= 0) {
                break;
            }

            if ((p_buffer + AT_DATA_BUFFER_MAX_SIZE - p_data) < len) {
                at_printf("discard %d data\r\n", len);
                break;
            }
            len = esp_at_port_read_data(p_data, len);

            if (len <= 0) {
                break;
            }
            p_data[len] = '\0';

            do {
                process_continue = FALSE;
                switch (at_state) {
                case AT_STATE_IDLE:
                    if (p_data_tail == NULL) {
                        p_data_tail = p_buffer;
                        for (;;) {
                            if ((*p_data_tail == '\r') && (*(p_data_tail + 1) == '\n')) {
                                break;
                            } else if ((*p_data_tail) == '\0') {
                                p_data_tail = NULL;
                                break;
                            }
                            p_data_tail++;
                        }
                    }

                    if (p_data_tail != NULL) {
                        p_data_tail += 2;
                        if (at_get_echo_flag() == TRUE) {
                            esp_at_port_write_data(p_data, p_data_tail - p_data);
                        }

                        p_data_head = p_buffer;
                        at_form_msg_p((&send_msg), AT_MSG_ID_CMD_DATA, (p_data_tail - p_data_head), p_data_head);
                        xQueueSend( at_process_queue, &send_msg, portMAX_DELAY );
                        at_state = AT_STATE_PROCESSING;

                        if ((p_data + len) != p_data_tail) {
                            p_data += len;
                            p_data_tail = NULL;
                            process_continue = TRUE;
                        }
                    } else {
                        if (at_get_echo_flag() == TRUE) {
                            esp_at_port_write_data(p_data, len);
                        }
                        p_data += len;
                        if (p_data - p_buffer >= AT_CMD_MAX_SIZE) {
                            if (p_data[-1] == '\r') {
                                p_buffer[0] = p_data[-1];
                                p_data = p_buffer + 1;
                            } else {
                                p_data = p_buffer;
                            }
                        }
                    }
                    break;
                case AT_STATE_PROCESSING:
                    if (at_strstr(p_data, "AT+RST\r\n") != NULL) {
                        at_port_print((uint8_t *)"\r\nWill force to restart!!!\r\n");
                        at_handle_restart();
                    } else {
                        esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_PROCESSING);
                        at_send_busy();
                    }
                    break;
                case AT_STATE_SENDING_RECV: {
                    extern uint8_t sendingID;
                    bool sendex_flag = FALSE;
                    if (len == 3) {
                        if (at_memcmp(p_data, "+++", 3) == 0) {
                            p_data = p_buffer;
                            at_state = AT_STATE_IDLE;
                            p_data_tail = NULL;
                            p_data_head = NULL;
                            at_port_print((uint8_t *)"\r\nSEND Canceled\r\n");
                            break;
                        }
                    }

                    if (pLink[sendingID].send_mode == AT_SEND_MODE_SENDEX) {
                        uint32_t loop = 0;
                        for (loop = 0; loop < p_data - p_buffer + len; loop++) {
                            if ((p_buffer[loop] == '\\') && (loop + 1 < p_data - p_buffer + len) && (p_buffer[loop + 1] == '0')) {
                                p_buffer[loop] = '\0';
                                sendex_flag = TRUE;
                                break;
                            }
                        }
                        len = p_buffer + loop - p_data;
                    }

                    p_data += len;
                    if ((p_data - p_buffer >= send_total_len) || (sendex_flag == TRUE)) {
                        if ((p_data - p_buffer) < send_total_len) {
                            send_total_len = (p_data - p_buffer);
                        }
                        at_form_msg_p((&send_msg),AT_MSG_ID_SEND_DATA,send_total_len,p_buffer);
                        xQueueSend( at_process_queue, &send_msg, portMAX_DELAY );
                        at_state = AT_STATE_WAIT_SEND_DONE;
                        if (p_data - p_buffer > send_total_len) {
                            at_send_busy();
                        }
                        sendex_flag = FALSE;
                    }
                }
                break;

                case AT_STATE_TRANSMIT:
                    if ((len == 3) && (at_memcmp(p_data, "+++", 3) == 0)) {
                        esp_at_transmit_terminal ();
                        p_data = p_buffer;
                    } else {
                        p_data += len;
                        if (p_data - p_buffer == AT_TRANSMIT_MAX_SIZE) {
                            at_transparent_transmit_preprocess(p_buffer, AT_TRANSMIT_MAX_SIZE);
                            timeout_tick = portMAX_DELAY;
                            transmit_done = FALSE;
                            p_data = p_buffer;
                        } else if (p_data - p_buffer > AT_TRANSMIT_MAX_SIZE) {
                            at_transparent_transmit_preprocess(p_buffer, AT_TRANSMIT_MAX_SIZE);
                            timeout_tick = portMAX_DELAY;
                            transmit_done = FALSE;
                            at_memcpy(p_buffer, &p_buffer[AT_TRANSMIT_MAX_SIZE], (p_data - p_buffer) - AT_TRANSMIT_MAX_SIZE);
                            p_data = p_data - AT_TRANSMIT_MAX_SIZE;
                        } else {
                            timeout_tick = 20 / (1000 /xPortGetTickRateHz());
                        }
                    }
                    break;
                default:
                    break;
                }
            } while (process_continue);
        }
        break;
        case AT_MSG_ID_TRANSFORM_SENDING: {
            p_data = p_buffer;
            at_state = AT_STATE_SENDING_RECV;
            send_total_len = (uint32_t)msg.data;
        }
        break;
        case AT_MSG_ID_TRANSFORM_TRANSMIT: {
            p_data = p_buffer;
            at_state = AT_STATE_TRANSMIT;
            timeout_tick = portMAX_DELAY;  // 20ms
            p_data_tail = NULL;
            p_data_head = NULL;
            transmit_done = TRUE;
            esp_at_custom_status_callback(ESP_AT_STATUS_TRANSMIT);
        }
        break;
        case AT_MSG_ID_TRANSMIT_TERMINAL:
            if (at_state == AT_STATE_TRANSMIT) {
                at_ip_mode = FALSE;
                at_state = AT_STATE_IDLE;
                p_data = p_buffer;
                p_data_tail = NULL;
                p_data_head = NULL;
                timeout_tick = portMAX_DELAY;  // forever
                transmit_done = TRUE;
                esp_at_custom_status_callback(ESP_AT_STATUS_NORMAL);

            }
            break;
        case AT_MSG_ID_TRANSMIT_DONE:
            if (at_state == AT_STATE_TRANSMIT) {
                if ((p_data - p_buffer) > AT_TRANSMIT_MAX_SIZE) {
                    at_transparent_transmit_preprocess(p_buffer, AT_TRANSMIT_MAX_SIZE);
                    at_memcpy(p_buffer, &p_buffer[AT_TRANSMIT_MAX_SIZE], (p_data - p_buffer) - AT_TRANSMIT_MAX_SIZE);
                    p_data = p_data - AT_TRANSMIT_MAX_SIZE;
                    timeout_tick = portMAX_DELAY;
                    transmit_done = FALSE;
                } else if ((p_data - p_buffer) == 0) {
                    timeout_tick = portMAX_DELAY;
                    transmit_done = TRUE;
                } else {
                    timeout_tick = 20 / (1000 /xPortGetTickRateHz());
                    transmit_done = TRUE;
                }
            }

            break;
        case AT_MSG_ID_TRANSFORM_IDLE:
        case AT_MSG_ID_CMD_PROCESSING_DONE: {
            p_data = p_buffer;
            p_data_tail = NULL;
            p_data_head = NULL;

            at_state = AT_STATE_IDLE;
        }
        break;
        case AT_MSG_ID_SEND_DONE: {
            p_data = p_buffer;
            p_data_tail = NULL;
            p_data_head = NULL;
            at_state = AT_STATE_IDLE;
        }
        break;
        default: {

        }
        break;
        }
    }
}

static void at_process_task( void *pvParameters )
{
    at_task_msg msg;
    at_task_msg send_msg;
    uint32_t result_code;

    for (;;) {
        if (xQueueReceive(at_process_queue, (void *)&msg, portMAX_DELAY) == pdTRUE) {
            result_code = ESP_AT_RESULT_CODE_ERROR;
            switch (msg.msg_id) {
            case AT_MSG_ID_CMD_DATA:
                result_code = at_cmd_process(msg.data, msg.len);
                break;
            case AT_MSG_ID_SEND_DATA:
                result_code = at_sending_data(msg.data, msg.len);
                break;
            case AT_MSG_ID_TRANSMIT_DATA: {
                at_transparent_transmit(msg.data, msg.len);
#if 0
                if (ret > 0) {
                    at_memset(&send_msg, 0x0, sizeof(send_msg));
                    at_form_msg_p(&send_msg, AT_MSG_ID_TRANSMIT_DONE, 0, NULL);
                    xQueueSend(at_recv_cmd_queue, &send_msg, portMAX_DELAY);
                } else if (ret == 0) {
                    esp_at_transmit_terminal ();
                } else {

                }
#else
                at_memset(&send_msg, 0x0, sizeof(send_msg));
                at_form_msg_p(&send_msg, AT_MSG_ID_TRANSMIT_DONE, 0, NULL);
                xQueueSend(at_recv_cmd_queue, &send_msg, portMAX_DELAY);
#endif
                result_code = ESP_AT_RESULT_CODE_IGNORE;
            }
            break;
            default:
                break;
            }
            at_handle_result_code(result_code, NULL);
        }
    }
}

int8_t at_wifi_status = SYSTEM_EVENT_WIFI_READY;
uint8_t at_wifi_reason = WIFI_REASON_BEACON_TIMEOUT;
bool at_wifi_auto_reconnect_flag = true;

void wifi_station_init_connect_status(void)
{
    at_wifi_status = SYSTEM_EVENT_STA_START;
    at_wifi_reason = WIFI_REASON_BEACON_TIMEOUT;
}


int8_t wifi_station_get_connect_status(void)
{
    // at_printf("wifi_station_get_connect_status:%d,%d\r\n",at_wifi_status,at_wifi_reason);
    if (at_wifi_status == SYSTEM_EVENT_STA_DISCONNECTED) {
        if (WIFI_REASON_NO_AP_FOUND == at_wifi_reason) {
            return STATION_NO_AP_FOUND; // no ap found
        } else if (WIFI_REASON_BEACON_TIMEOUT == at_wifi_reason) {
            return STATION_IDLE; // no ap found
        } else if (WIFI_REASON_AUTH_FAIL == at_wifi_reason) {
            return STATION_WRONG_PASSWORD; // wrong password
        } else if (WIFI_REASON_ASSOC_FAIL == at_wifi_reason) {
            return STATION_CONNECT_FAIL; // connect fail
        } else {
            return STATION_CONNECT_FAIL;
        }
    } else if (at_wifi_status == SYSTEM_EVENT_STA_START) {
        return STATION_CONNECTING; // connecting
    } else if (at_wifi_status == SYSTEM_EVENT_STA_GOT_IP) {
        return STATION_GOT_IP;
    }

    return STATION_IDLE;
}

static esp_err_t at_wifi_event_handler_cb(void *ctx, system_event_t *event)
{
    wifi_mode_t mode = WIFI_MODE_NULL;
    tcpip_adapter_dhcp_status_t status = TCPIP_ADAPTER_DHCP_INIT;
    static system_event_id_t last_state = SYSTEM_EVENT_STA_DISCONNECTED;
    if (event == NULL) {
        return ESP_FAIL;
    }

    switch (event->event_id) {
    case SYSTEM_EVENT_STA_CONNECTED:
        at_wifi_status = SYSTEM_EVENT_STA_CONNECTED;
        last_state = SYSTEM_EVENT_STA_CONNECTED;
        if (at_ip_mode != TRUE) {
            at_port_print((uint8_t *)"WIFI CONNECTED\r\n");
        }
        tcpip_adapter_dhcpc_get_status(TCPIP_ADAPTER_IF_STA, &status);
        if (status == TCPIP_ADAPTER_DHCP_STOPPED) {
            at_wifi_status = SYSTEM_EVENT_STA_GOT_IP;
        }
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED: {
        uint8_t loop = 0;

        at_wifi_status = SYSTEM_EVENT_STA_DISCONNECTED;
        at_wifi_reason = event->event_info.disconnected.reason;

        if (at_ip_mode != TRUE) {
            mdState = m_disconnet;
            if (esp_wifi_get_mode(&mode) == ESP_OK) {
                if ((mode == WIFI_MODE_NULL)
                        || ((WIFI_MODE_NULL == WIFI_MODE_STA) && (wifi_station_get_connect_status() != STATION_GOT_IP))) {
                    for (loop = 0; loop < at_netconn_max_num; loop++) {
                        if ((pLink[loop].socket >= 0) && (pLink[loop].ip_type == AT_SOCKET_TYPE_TCP)) {
                            at_socket_client_cleanup_task(&pLink[loop]);
                        }
                    }
                }
            }
        }

        if (at_wifi_auto_reconnect_flag || (at_ip_mode  == true)) {
            esp_wifi_connect();
        } else {
            if ((last_state == SYSTEM_EVENT_STA_CONNECTED) || (last_state == SYSTEM_EVENT_STA_GOT_IP)) {
                if (at_ip_mode != TRUE) {
                    at_port_print((uint8_t *)"WIFI DISCONNECT\r\n");
                }
            }
        }
        last_state = SYSTEM_EVENT_STA_DISCONNECTED;
    }
    break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        mdState = m_gotip;
        at_wifi_status = SYSTEM_EVENT_STA_GOT_IP;
        last_state = SYSTEM_EVENT_STA_GOT_IP;
        if (at_ip_mode != TRUE) {
            at_port_print((uint8_t *)"WIFI GOT IP\r\n");
        }
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:       /**< ESP32 station wps succeeds in enrollee mode */
        esp_wifi_wps_disable();
        esp_wifi_connect();
        at_printf("wps success,connecting ap ...\r\n");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:        /**< ESP32 station wps fails in enrollee mode */
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:       /**< ESP32 station wps timeout in enrollee mode */
        esp_wifi_wps_disable();
        esp_wifi_disconnect();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        break;
    default:
        break;
    }

    return ESP_OK;
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(at_wifi_event_handler_cb, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}
/**
 * @brief  Initializes build two tasks.
 * @param  None
 * @retval None
 */
void esp_at_module_init (uint32_t netconn_max, const uint8_t *custom_version)
{
    initialise_wifi();
    at_set_custom_version(custom_version);
    at_netconn_init(netconn_max);

    at_process_queue = xQueueCreate(AT_PROCESS_QUEUE_DEPTH, sizeof(at_task_msg));
    if (at_process_queue != NULL) {
        xTaskCreate(at_process_task, AT_TASK_NAME, AT_TASK_STACK_DEPTH, NULL, AT_TASK_PRIORITIE, NULL);
    } else {
        at_printf("process queue fail\r\n");
        esp_restart();
    }

    esp_at_port_read_data(NULL,esp_at_port_get_data_length());
    at_recv_data_queue = xQueueCreate(AT_RECV_DATA_QUEUE_DEPTH, sizeof(at_task_msg));
    if (at_recv_data_queue != NULL) {
        at_recv_cmd_queue = xQueueCreate(AT_RECV_CMD_QUEUE_DEPTH, sizeof(at_task_msg));
        if (at_recv_cmd_queue != NULL) {
            at_recv_set = xQueueCreateSet(AT_RECV_CMD_QUEUE_DEPTH + AT_RECV_DATA_QUEUE_DEPTH);
            if (at_recv_set != NULL) {
                xQueueAddToSet((QueueSetMemberHandle_t)at_recv_cmd_queue, at_recv_set);
                xQueueAddToSet((QueueSetMemberHandle_t)at_recv_data_queue, at_recv_set);
                at_nvm_init();
                xTaskCreate(at_recv_task, AT_RECV_TASK_NAME, AT_RECV_TASK_STACK_DEPTH, NULL, AT_RECV_TASK_PRIORITIE, NULL);
            } else {
                at_printf("recv set fail\r\n");
                esp_restart();
            }
        } else {
            at_printf("recv ext queue fail\r\n");
            esp_restart();
        }
    } else {
        at_printf("recv queue fail\r\n");
        esp_restart();
    }


    {
        wifi_mode_t mode;
        bool auto_connect;
        if (esp_wifi_get_mode(&mode) == ESP_OK) {
            if (esp_wifi_get_auto_connect(&auto_connect) == ESP_OK) {
                if ((mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA) && auto_connect) {
                    esp_wifi_connect();
                }
            }
        }
    }
}


/**
 * @}
 */
