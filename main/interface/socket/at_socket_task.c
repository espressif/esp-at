/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#define AT_SOCKET_RECV_BUFFER_SIZE              256
#define AT_RING_BUFFER_SIZE                     (8 * 1024)

#ifdef CONFIG_AT_BASE_ON_SOCKET
#include "sys/socket.h"
#include "netdb.h"
#include "esp_at.h"
#include "esp_at_interface.h"

// static variables
static int s_client_fd = -1;
static bool s_trans_mode = false;
static RingbufHandle_t s_ring_buffer;
static const char *TAG = "at-socket";

static int32_t at_socket_read_data(uint8_t *data, int32_t len)
{
    if (data == NULL || len < 0) {
        ESP_LOGE(TAG, "invalid data:%p or len:%d", data, len);
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "read empty data");
        return 0;
    }

    size_t data_len = len;
    uint8_t *recv_data = (uint8_t *)xRingbufferReceive(s_ring_buffer, &data_len, 0);

    if (recv_data == NULL || data_len > AT_RING_BUFFER_SIZE) {
        ESP_LOGE(TAG, "cannot recv data from ringbuf");
        return -1;
    } else {
        memcpy(data, recv_data, data_len);
    }

    return data_len;
}

static int32_t at_socket_write_data(uint8_t *data, int32_t len)
{
    if (len < 0 || data == NULL) {
        ESP_LOGE(TAG, "invalid data:%p or len:%d", data, len);
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "write empty data");
        return 0;
    }

    if (s_client_fd >= 0 && len > 0) {
        if (send(s_client_fd, data, len, 0) < 0) {
            ESP_LOGE(TAG, "cannot send message");
            return -1;
        }
    }

    return len;
}

static void socket_task(void *params)
{
    // new server fd
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        ESP_LOGE(TAG, "cannot create socket");
        goto exit_task;
    }

    // bind server fd
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONFIG_AT_SOCKET_PORT);
    server_addr.sin_addr.s_addr = 0;
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        ESP_LOGE(TAG, "cannot bind socket");
        goto exit_task;
    }

    // listen
    if (listen(server_fd, 1) == -1) {
        ESP_LOGE(TAG, "cannot listen socket");
        goto exit_task;
    }
    ESP_LOGD(TAG, "socket listening...");

    for (;;) {
        // accept a new client
        socklen_t len;
        struct sockaddr_in remote_addr;
        ESP_LOGD(TAG, "wait for a new client...");
        s_client_fd = accept(server_fd, (struct sockaddr*) &remote_addr, &len);
        ESP_LOGD(TAG, "accept a new client: %d", s_client_fd);

        if (s_client_fd == -1) {
            ESP_LOGE(TAG, "cannot accept socket");
            break;
        }

        // create a buffer to store data from client
        uint8_t *buffer = (uint8_t *)malloc(AT_SOCKET_RECV_BUFFER_SIZE * sizeof(uint8_t));

        for (;;) {
            // set fd_set
            fd_set server_fd_set;
            FD_ZERO(&server_fd_set);
            FD_SET(s_client_fd, &server_fd_set);

            if (select(s_client_fd + 1, &server_fd_set, NULL, NULL, NULL) >= 0) {
                // receive data from client
                if (FD_ISSET(s_client_fd, &server_fd_set)) {
                    int byte_num = recv(s_client_fd, buffer, AT_SOCKET_RECV_BUFFER_SIZE, 0);

                    // exit transparent transmition mode
                    if (s_trans_mode && (byte_num == 3) && (memcmp(buffer, "+++", 3) == 0)) {
                        ESP_LOGI(TAG, "exit passthrough mode");
                        esp_at_transmit_terminal();
                        continue;
                    }

                    if (byte_num > 0) {
                        if (xRingbufferSend(s_ring_buffer, buffer, byte_num, portMAX_DELAY) == pdFALSE) {
                            ESP_LOGE(TAG, "cannot send data to ringbuf");
                            continue;
                        }
                        esp_at_port_recv_data_notify(byte_num, portMAX_DELAY);
                    } else {
                        s_client_fd = -1;
                        ESP_LOGE(TAG, "time out or connection error, wait for a new client..");
                        break;
                    }
                }
            } else {
                ESP_LOGE(TAG, "cannot select socket");
                continue;
            }
        }

        free(buffer);
        buffer = NULL;
    }

exit_task:
    if (server_fd >= 0) {
        close(server_fd);
    }

    vTaskDelete(NULL);
}

static void at_socket_transmit_mode_switch_cb(esp_at_status_type status)
{
    switch (status) {
    case ESP_AT_STATUS_NORMAL:
        s_trans_mode = false;
        break;

    case ESP_AT_STATUS_TRANSMIT:
        s_trans_mode = true;
        break;
    }
}

static void at_socket_init(void)
{
    // create a ring buffer for rx data
    s_ring_buffer = xRingbufferCreate(AT_RING_BUFFER_SIZE, RINGBUF_TYPE_ALLOWSPLIT);
    if (!s_ring_buffer) {
        ESP_LOGE(TAG, "create ringbuf failed");
        return;
    }

    // set wifi mode for socket interface
    wifi_mode_t mode;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));
    if (mode == WIFI_MODE_STA) {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    }

    // get softap config
    wifi_config_t config;
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_AP, &config));

    // get ip address of softap
    esp_netif_ip_info_t ip;
    esp_netif_t * ap_if = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    ESP_ERROR_CHECK(esp_netif_get_ip_info(ap_if, &ip));
    printf("softap: (%s) started, listen on (" IPSTR ":%d)\r\n", config.ap.ssid, IP2STR(&ip.ip), CONFIG_AT_SOCKET_PORT);

    xTaskCreate(&socket_task, "socket_task", 4096, NULL, 5, NULL);
}

void at_interface_init(void)
{
    // init interface driver
    at_socket_init();

    // init interface operations
    esp_at_device_ops_struct socket_ops = {
        .read_data = at_socket_read_data,
        .write_data = at_socket_write_data,
        .get_data_length = NULL,
        .wait_write_complete = NULL,
    };
    at_interface_ops_init(&socket_ops);

    // init interface hooks
    esp_at_custom_ops_struct socket_hooks = {
        .status_callback = at_socket_transmit_mode_switch_cb,
        .pre_sleep_callback = NULL,
        .pre_wakeup_callback = NULL,
        .pre_deepsleep_callback = NULL,
        .pre_restart_callback = NULL,
        .pre_active_write_data_callback = NULL,
    };
    at_interface_hooks(&socket_hooks);
}

#endif
