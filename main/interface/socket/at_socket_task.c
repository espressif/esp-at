/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"

#include "tcpip_adapter.h"
#include "sys/socket.h"
#include "netdb.h"

#include "nvs_flash.h"

#include "esp_at.h"

#define ESP_AT_BUFFER_SIZE 200
#define ESP_AT_SOCKET_PORT CONFIG_AT_SOCKET_PORT
#define ESP_AT_RING_ESP_AT_BUFFER_SIZE 8*1024

static int at_client_fd;
static bool at_transparent_transmition = false;
static RingbufHandle_t at_read_ring_buf;
static const char* TAG = "esp_at";

/*Called when socket recieve a normal AT command, make sure you have added \r\n in your socket data*/
static int32_t at_socket_read_data(uint8_t* data, int32_t len)
{
    if (data == NULL || len < 0) {
        ESP_LOGE(TAG, "Cannot get read data address.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "Empty read data.");
        return 0;
    }

    size_t ring_len = len;
    uint8_t* recv_data = (uint8_t*) xRingbufferReceive(at_read_ring_buf, &ring_len, (portTickType) 0);

    if (recv_data == NULL) {
        ESP_LOGE(TAG, "Cannot recieve socket data from ringbuf.");
        return -1;
    } else {
        memcpy(data, recv_data, len);
    }

    return len;
}

/*Result of AT command, auto call when read_data get data*/
static int32_t at_socket_write_data(uint8_t* buf, int32_t len)
{
    if (len < 0 || buf == NULL) {
        ESP_LOGE(TAG, "Cannot get write data.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGE(TAG, "Empty write data.");
        return 0;
    }

    if (at_client_fd >= 0 && len > 0) {
        if (send(at_client_fd, buf, len, 0) < 0) {
            ESP_LOGE(TAG, "Cannot send message.");
            return -1;
        }
    }

    return len;
}

/*We can ignore it, -1 is a right value*/
static int32_t at_port_get_data_length(void)
{
    return -1;
}

/*In socket mode, we can ignore it*/
static bool at_port_wait_write_complete(int32_t timeout_msec)
{
    return true;
}

static void socket_task(void* pvParameters)
{
    struct sockaddr_in server_addr;
    fd_set server_fd_set;
    struct sockaddr_in client_address;
    socklen_t address_len;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ESP_AT_SOCKET_PORT);
    server_addr.sin_addr.s_addr = 0;

    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock_fd == -1) {
        ESP_LOGE(TAG, "Cannot create socket.");
        goto Exit0;
    }

    if (bind(server_sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        ESP_LOGE(TAG, "Cannot bind socket");
        goto Exit0;
    }

    if (listen(server_sock_fd, 1) == -1) {
        ESP_LOGE(TAG, "Cannot listen socket.");
        goto Exit0;
    }

    for (;;) {
        at_client_fd = accept(server_sock_fd, (struct sockaddr*) &client_address, &address_len);

        if (at_client_fd == -1) {
            ESP_LOGE(TAG, "Cannot accept socket.");
            break;
        }

        uint8_t* recv_msg = (uint8_t*) malloc(ESP_AT_BUFFER_SIZE * sizeof(uint8_t));

        for (;;) {

            //Firstly, we do some preparatory work for AT command
            FD_ZERO(&server_fd_set);
            FD_SET(at_client_fd, &server_fd_set);

            if (select(at_client_fd + 1, &server_fd_set, NULL, NULL, NULL) >= 0) {

                //There are something happened in socket
                if (FD_ISSET(at_client_fd, &server_fd_set)) {
                    int byte_num = recv(at_client_fd, recv_msg, ESP_AT_BUFFER_SIZE, 0);

                    /* If using custom commands and want to exit transparent transmition, make sure
                    the program know first*/
                    if (at_transparent_transmition && (byte_num == 3) && (memcmp(recv_msg, "+++", 3) == 0)) {
                        ESP_LOGI(TAG, "Exit transparent transmition mode");
                        esp_at_transmit_terminal();
                        continue;
                    }

                    if (byte_num > 0) {
                        if (xRingbufferSend(at_read_ring_buf, recv_msg, byte_num, portMAX_DELAY) == pdFALSE) {        // a buffer is better
                            ESP_LOGE(TAG, "Cannot send data to ringbuf");
                            continue;
                        }

                        //Call AT lib to handle commands
                        esp_at_port_recv_data_notify(byte_num, portMAX_DELAY);
                    } else {  //have not recieve data or error data
                        at_client_fd = -1;
                        ESP_LOGE(TAG, "Time out or client exit,try to reconnect");
                        break;
                    }
                }
            } else {
                ESP_LOGE(TAG, "Cannot select socket.");
                continue;
            }
        }

        free(recv_msg);
        recv_msg = NULL;
    }

    //There is something wrong happened when we try to use socket.
Exit0:

    if (server_sock_fd >= 0) {
        close(server_sock_fd);
    }

    vTaskDelete(NULL);
}

/*This function let you handle some operation when using custom cmd and AT status change*/
static void at_status_callback(esp_at_status_type status)
{
    switch (status) {
        case ESP_AT_STATUS_NORMAL:
            at_transparent_transmition = false;
            break;

        case ESP_AT_STATUS_TRANSMIT:
            at_transparent_transmition = true;
            break;
    }
}

void at_interface_init (void)
{
    uint8_t* version = (uint8_t*)malloc(192);

    if (version == NULL) {
        ESP_LOGE(TAG, "malloc version space fail");
        return;
    }

    //If you want to use AT lib, make sure obey this function.
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_socket_read_data,
        .write_data = at_socket_write_data,
        .get_data_length = at_port_get_data_length,
        .wait_write_complete = at_port_wait_write_complete,
    };

    esp_at_custom_ops_struct esp_at_custom_ops = {
        .status_callback = at_status_callback,
    };

    esp_at_device_ops_regist(&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
}

void at_custom_init(void)
{
    wifi_mode_t mode;
    tcpip_adapter_ip_info_t ip;
    at_read_ring_buf = xRingbufferCreate(ESP_AT_RING_ESP_AT_BUFFER_SIZE, RINGBUF_TYPE_ALLOWSPLIT);

    if (at_read_ring_buf == NULL) {
        ESP_LOGE(TAG, "Cannot create ringbuf");
        return;
    }

    /*We hope have a init ip to control esp32, so sta mode is unfriendly.
     you can modify if you need it*/
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));

    if (mode == WIFI_MODE_STA) {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    }

    ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip));
    /*This IP is a socket server,you can connect it and use AT command to control esp32*/
    ESP_LOGI(TAG, "IP address of Soft AP is:"IPSTR, IP2STR(&ip.ip));

    xTaskCreate(&socket_task, "socket_task", 4096, NULL, 5, NULL);

    esp_at_port_write_data((uint8_t*) "\r\nready\r\n", strlen("\r\nready\r\n"));
}
