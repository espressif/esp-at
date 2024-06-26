/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdkconfig.h"

#ifdef CONFIG_AT_BASE_ON_UART
#include "soc/io_mux_reg.h"
#include "soc/gpio_periph.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_at.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "at_uart.h"
#include "esp_at_interface.h"

// static variables
static QueueHandle_t s_at_uart_queue = NULL;

// global variables
uint8_t g_at_cmd_port = UART_NUM_1;
at_uart_port_pins_t g_uart_port_pin;

static int32_t at_uart_write_data(uint8_t *data, int32_t len)
{
    uint32_t length = 0;

    length = uart_write_bytes(g_at_cmd_port, (char *)data, len);
    return length;
}

static int32_t at_uart_read_data(uint8_t *buffer, int32_t len)
{
    if (len == 0) {
        return 0;
    }

    if (buffer == NULL) {
        if (len == -1) {
            size_t size = 0;
            if (uart_get_buffered_data_len(g_at_cmd_port, &size) != ESP_OK) {
                return -1;
            }
            len = size;
        }

        if (len == 0) {
            return 0;
        }

        uint8_t *data = (uint8_t *)malloc(len);
        if (data) {
            len = uart_read_bytes(g_at_cmd_port, data, len, portTICK_PERIOD_MS);
            free(data);
            return len;
        } else {
            return -1;
        }
    } else {
        return uart_read_bytes(g_at_cmd_port, buffer, len, portTICK_PERIOD_MS);
    }
}

static int32_t at_uart_get_data_len(void)
{
    size_t size = 0;
    if (uart_get_buffered_data_len(g_at_cmd_port, &size) == ESP_OK) {
        int pattern_pos = uart_pattern_get_pos(g_at_cmd_port);
        if (pattern_pos >= 0) {
            size = pattern_pos;
        }
        return size;
    } else {
        return 0;
    }
}

static bool at_uart_wait_tx_done(int32_t ms)
{
    if (uart_wait_tx_done(g_at_cmd_port, ms / portTICK_PERIOD_MS) == ESP_OK) {
        return true;
    }

    return false;
}

static void at_uart_task(void *params)
{
    uart_event_t event;
    int pattern_pos = -1;
    BaseType_t retry_flag = pdFALSE;
    uint8_t *data = NULL;
    uint32_t data_len = 0;

    for (;;) {
        if (xQueueReceive(s_at_uart_queue, (void *)&event, portMAX_DELAY)) {
retry:
            switch (event.type) {
            case UART_DATA:
            case UART_BUFFER_FULL:
                data_len += event.size;
                // we can put all data together to process
                retry_flag = pdFALSE;
                while (xQueueReceive(s_at_uart_queue, (void *)&event, 0) == pdTRUE) {
                    if (event.type == UART_DATA) {
                        data_len += event.size;
                    } else if (event.type == UART_BUFFER_FULL) {
                        esp_at_port_recv_data_notify(data_len, portMAX_DELAY);
                        data_len = event.size;
                        break;
                    } else {
                        retry_flag = pdTRUE;
                        break;
                    }
                }
                esp_at_port_recv_data_notify(data_len, portMAX_DELAY);
                data_len = 0;

                if (retry_flag == pdTRUE) {
                    goto retry;
                }
                break;

            case UART_PATTERN_DET:
                pattern_pos = uart_pattern_pop_pos(g_at_cmd_port);
                if (pattern_pos >= 0) {
                    data = (uint8_t *)malloc(pattern_pos + 3);
                    uart_read_bytes(g_at_cmd_port, data, pattern_pos + 3, 0);
                    free(data);
                    data = NULL;
                } else {
                    uart_flush_input(g_at_cmd_port);
                    xQueueReset(s_at_uart_queue);
                }
                esp_at_transmit_terminal();
                break;

            case UART_FIFO_OVF:
                retry_flag = pdFALSE;
                while (xQueueReceive(s_at_uart_queue, (void *)&event, 0) == pdTRUE) {
                    if ((event.type == UART_DATA) || (event.type == UART_BUFFER_FULL) || (event.type == UART_FIFO_OVF)) {
                        // put all data together to process
                    } else {
                        retry_flag = pdTRUE;
                        break;
                    }
                }
                esp_at_port_recv_data_notify(at_uart_get_data_len(), portMAX_DELAY);
                data_len = 0;
                if (retry_flag == pdTRUE) {
                    goto retry;
                }
                break;

            default:
                break;
            }
        }
    }
    vTaskDelete(NULL);
}

static void at_uart_init(void)
{
    // get uart port and uart pins
    at_mfg_uart_port_pins_get(&g_uart_port_pin);
    g_at_cmd_port = g_uart_port_pin.number;

    // install uart driver
    uart_driver_install(g_at_cmd_port, AT_UART_RX_BUFFER_SIZE, AT_UART_TX_BUFFER_SIZE, AT_UART_QUEUE_SIZE, &s_at_uart_queue, 0);

    // set uart configuration
    uart_config_t config;
    at_uart_config_init(&config);
    if (!at_nvs_uart_config_get(&config)) {
        at_nvs_uart_config_set(&config);
    }
    uart_param_config(g_at_cmd_port, &config);

    // set uart pins
    uart_set_pin(g_at_cmd_port, g_uart_port_pin.tx_pin, g_uart_port_pin.rx_pin, g_uart_port_pin.rts_pin, g_uart_port_pin.cts_pin);

    // set uart interrupt
    at_uart_intr_config();

    // do some possible uart workarounds
    at_uart_workaround();

    printf("AT cmd port:uart%d tx:%d rx:%d cts:%d rts:%d baudrate:%d\r\n",
           g_at_cmd_port, g_uart_port_pin.tx_pin, g_uart_port_pin.rx_pin,
           g_uart_port_pin.cts_pin, g_uart_port_pin.rts_pin, config.baud_rate);

    xTaskCreate(at_uart_task, "uTask", 1024, NULL, 1, NULL);
}

void at_uart_transmit_mode_switch_cb(esp_at_status_type status)
{
    switch (status) {
    case ESP_AT_STATUS_NORMAL:
        uart_disable_pattern_det_intr(g_at_cmd_port);
        break;

    case ESP_AT_STATUS_TRANSMIT: {
        /**
         * As the implement of API uart_enable_pattern_det_baud_intr() in esp-idf,
         * the last three timeout parameters is different on ESP32 and non ESP32 platform.
         *
         * That is, on ESP32 platform, it uses the APB clocks as the unit;
         * on non ESP32 platform (ESP32-C3, ..), it uses the UART baud rate clocks as the unit.
         *
         * Notes:
         * on non ESP32 platform, due to the value of input parameters have a limit of 0xFFFF (see as macro: UART_RX_GAP_TOUT_V..),
         * so the maximum uart baud rate is recommended to be less than (0xFFFF * 1000 / AT_UART_PATTERN_TIMEOUT_MS) = 3276750 ~= 3.2Mbps
         * otherwise, this uart_enable_pattern_det_baud_intr() will not work.
        */
#ifdef CONFIG_IDF_TARGET_ESP32
        int apb_clocks = (uint32_t)APB_CLK_FREQ * AT_UART_PATTERN_TIMEOUT_MS / 1000;
        uart_enable_pattern_det_baud_intr(g_at_cmd_port, '+', 3, apb_clocks, apb_clocks, apb_clocks);
#else
        uint32_t uart_baud = 0;
        uart_get_baudrate(g_at_cmd_port, &uart_baud);
        int uart_clocks = (uint32_t)uart_baud * AT_UART_PATTERN_TIMEOUT_MS / 1000;
        uart_enable_pattern_det_baud_intr(g_at_cmd_port, '+', 3, uart_clocks, uart_clocks, uart_clocks);
#endif
    }
    break;
    }
}

void at_uart_deepsleep_before_cb(void)
{
    // disable uart pins for power saving, in case of leakage current
    if (g_uart_port_pin.tx_pin >= 0) {
        gpio_set_direction(g_uart_port_pin.tx_pin, GPIO_MODE_DISABLE);
    }
    if (g_uart_port_pin.rx_pin >= 0) {
        gpio_set_direction(g_uart_port_pin.rx_pin, GPIO_MODE_DISABLE);
    }
    if (g_uart_port_pin.cts_pin >= 0) {
        gpio_set_direction(g_uart_port_pin.cts_pin, GPIO_MODE_DISABLE);
    }
    if (g_uart_port_pin.rts_pin >= 0) {
        gpio_set_direction(g_uart_port_pin.rts_pin, GPIO_MODE_DISABLE);
    }
}

void at_uart_restart_before_cb(void)
{
    uart_disable_rx_intr(g_at_cmd_port);
}

void at_interface_init(void)
{
    // init interface driver
    at_uart_init();

    // init interface operations
    esp_at_device_ops_struct uart_ops = {
        .read_data = at_uart_read_data,
        .write_data = at_uart_write_data,
        .get_data_length = at_uart_get_data_len,
        .wait_write_complete = at_uart_wait_tx_done,
    };
    at_interface_ops_init(&uart_ops);

    // init interface hooks
    esp_at_custom_ops_struct uart_hooks = {
        .status_callback = at_uart_transmit_mode_switch_cb,
        .pre_sleep_callback = NULL,
        .pre_wakeup_callback = NULL,
        .pre_deepsleep_callback = at_uart_deepsleep_before_cb,
        .pre_restart_callback = at_uart_restart_before_cb,
        .pre_active_write_data_callback = NULL,
    };
    at_interface_hooks(&uart_hooks);
}

#endif
