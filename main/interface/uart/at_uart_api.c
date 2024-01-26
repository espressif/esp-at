/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdkconfig.h"

#ifdef CONFIG_AT_BASE_ON_UART
#include "hal/gpio_ll.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_at.h"
#include "at_uart.h"
#include "driver/uart.h"

// static variables
static const uint8_t g_at_uart_parity_table[] = {UART_PARITY_DISABLE, UART_PARITY_ODD, UART_PARITY_EVEN};

// global variables
extern uint8_t g_at_cmd_port;
extern const char *g_at_mfg_nvs_name;
extern at_uart_port_pins_t g_uart_port_pin;

bool at_nvs_uart_config_get_internal(at_uart_config_t *config)
{
    nvs_handle handle;
    if (nvs_open("UART", NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_get_i32(handle, "rate", &config->baudrate) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_get_i8(handle, "databits", &config->data_bits) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_get_i8(handle, "stopbits", &config->stop_bits) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_get_i8(handle, "parity", &config->parity) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_get_i8(handle, "flow_ctrl", &config->flow_control) != ESP_OK) {
        nvs_close(handle);
        return false;
    }

    nvs_close(handle);
    return true;
}

bool at_nvs_uart_config_set_internal(at_uart_config_t *config)
{
    nvs_handle handle;

    if (config == NULL) {
        return false;
    }

    if (nvs_open("UART", NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_i32(handle, "rate", config->baudrate) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_set_i8(handle, "databits", config->data_bits) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_set_i8(handle, "stopbits", config->stop_bits) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_set_i8(handle, "parity", config->parity) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    if (nvs_set_i8(handle, "flow_ctrl", config->flow_control) != ESP_OK) {
        nvs_close(handle);
        return false;
    }

    nvs_close(handle);
    return true;
}

static int32_t at_mfg_uart_baudrate_get(void)
{
    // default value
    int32_t baud_rate = AT_UART_BAUD_RATE_DEF;

    at_mfg_params_storage_mode_t mode = at_get_mfg_params_storage_mode();
    if (mode == AT_PARAMS_IN_MFG_NVS) {
        nvs_handle handle;
        if (nvs_open_from_partition(g_at_mfg_nvs_name, "factory_param", NVS_READONLY, &handle) != ESP_OK) {
            return baud_rate;
        }
        if (nvs_get_i32(handle, "uart_baudrate", &baud_rate) != ESP_OK) {
            nvs_close(handle);
            return baud_rate;
        }

    } else if (mode == AT_PARAMS_IN_PARTITION) {
        char data[AT_BUFFER_ON_STACK_SIZE] = {0};
        const esp_partition_t *partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
        if (partition) {
            if (esp_partition_read(partition, 0, data, AT_BUFFER_ON_STACK_SIZE) != ESP_OK) {
                return baud_rate;
            }
            // magic number
            if (data[0] != 0xFC || data[1] != 0xFC) {
                return baud_rate;
            }
            // uart baudrate is stored in the 12nd to 15th bytes of the partition
            if (data[12] != 0xFF || data[13] != 0xFF || data[14] != 0xFF || data[15] != 0xFF) {
                baud_rate = *(int32_t *)&data[12];
            }
        }
    } else {
        return baud_rate;
    }

    return baud_rate;
}

uint8_t at_uart_port_get(void)
{
    return g_at_cmd_port;
}

int8_t at_uart_parity_get(int8_t index)
{
    return g_at_uart_parity_table[index];
}

esp_err_t at_mfg_uart_port_pins_get(at_uart_port_pins_t *config)
{
    // default values
    config->number = g_at_cmd_port;
    config->tx_pin = CONFIG_AT_UART_PORT_TX_PIN_DEFAULT;
    config->rx_pin = CONFIG_AT_UART_PORT_RX_PIN_DEFAULT;
    config->cts_pin = CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT;
    config->rts_pin = CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT;

    // get uart port and uart pins from flash
    int8_t uart_num;
    int32_t tx_pin, rx_pin, cts_pin, rts_pin;
    at_mfg_params_storage_mode_t mode = at_get_mfg_params_storage_mode();
    if (mode == AT_PARAMS_IN_MFG_NVS) {
        nvs_handle handle;
        if (nvs_open_from_partition(g_at_mfg_nvs_name, "factory_param", NVS_READONLY, &handle) != ESP_OK) {
            printf("open factory_param namespace failed\r\n");
            return ESP_FAIL;
        }
        if (nvs_get_i8(handle, "uart_port", &uart_num) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        if (nvs_get_i32(handle, "uart_tx_pin", &tx_pin) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        if (nvs_get_i32(handle, "uart_rx_pin", &rx_pin) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        if (nvs_get_i32(handle, "uart_cts_pin", &cts_pin) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }
        if (nvs_get_i32(handle, "uart_rts_pin", &rts_pin) != ESP_OK) {
            nvs_close(handle);
            return ESP_FAIL;
        }

    } else if (mode == AT_PARAMS_IN_PARTITION) {
        char data[AT_BUFFER_ON_STACK_SIZE] = {0};
        const esp_partition_t *partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
        if (partition) {
            if (esp_partition_read(partition, 0, data, AT_BUFFER_ON_STACK_SIZE) != ESP_OK) {
                return ESP_FAIL;
            }
            // magic number
            if (data[0] != 0xFC || data[1] != 0xFC) {
                return ESP_FAIL;
            }
            // uart configurations are stored in the 12nd to 19th bytes of the partition
            uart_num = data[5];
            if (data[16] != 0xFF && data[17] != 0xFF) {
                tx_pin = data[16];
                rx_pin = data[17];
            }
            if (data[18] != 0xFF) {
                cts_pin = data[18];
            } else {
                cts_pin = -1;
            }
            if (data[19] != 0xFF) {
                rts_pin = data[19];
            } else {
                rts_pin = -1;
            }
        }
    } else {
        return ESP_FAIL;
    }

    config->number = uart_num;
    config->tx_pin = tx_pin;
    config->rx_pin = rx_pin;
    config->cts_pin = cts_pin;
    config->rts_pin = rts_pin;

    return ESP_OK;
}

void at_uart_intr_config(void)
{
    uart_intr_config_t intr_config = {
        .intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M
        | UART_RXFIFO_TOUT_INT_ENA_M
        | UART_RXFIFO_OVF_INT_ENA_M,
        .rxfifo_full_thresh = 100,
        .rx_timeout_thresh = 10,
        .txfifo_empty_intr_thresh = 10
    };

    uart_intr_config(g_at_cmd_port, &intr_config);
}

void at_uart_workaround(void)
{
    // a workaround for uart1 outputs uninterrupted data during light-sleep
    PIN_SLP_INPUT_ENABLE(GPIO_PIN_MUX_REG[g_uart_port_pin.rx_pin]);
    gpio_sleep_set_pull_mode(g_uart_port_pin.rx_pin, GPIO_PULLUP_ONLY);
}

void at_uart_config_init(uart_config_t *config)
{
    config->baud_rate = at_mfg_uart_baudrate_get();
    config->data_bits = CONFIG_AT_UART_DEFAULT_DATABITS - 5;
    config->parity = at_uart_parity_get(CONFIG_AT_UART_DEFAULT_PARITY_BITS);
    config->stop_bits = CONFIG_AT_UART_DEFAULT_STOPBITS;
    config->flow_ctrl = CONFIG_AT_UART_DEFAULT_FLOW_CONTROL;
    config->rx_flow_ctrl_thresh = 122;
    config->source_clk = UART_SCLK_DEFAULT;
}

void at_nvs_uart_config_set(uart_config_t *config)
{
    at_uart_config_t to_set_config;
    memset(&to_set_config, 0x0, sizeof(to_set_config));
    to_set_config.baudrate = config->baud_rate;
    to_set_config.data_bits = config->data_bits;
    to_set_config.flow_control = config->flow_ctrl;
    to_set_config.parity = config->parity;
    to_set_config.stop_bits = config->stop_bits;

    at_nvs_uart_config_set_internal(&to_set_config);
}

bool at_nvs_uart_config_get(uart_config_t *config)
{
    // get uart configurations from flash
    at_uart_config_t to_get_config;
    memset(&to_get_config, 0x0, sizeof(to_get_config));
    if (at_nvs_uart_config_get_internal(&to_get_config)) {
        if (to_get_config.baudrate >= AT_UART_BAUD_RATE_MIN && to_get_config.baudrate <= AT_UART_BAUD_RATE_MAX) {
            config->baud_rate = to_get_config.baudrate;
        }
        if (to_get_config.data_bits >= UART_DATA_5_BITS && to_get_config.data_bits <= UART_DATA_8_BITS) {
            config->data_bits = to_get_config.data_bits;
        }
        if (to_get_config.stop_bits >= UART_STOP_BITS_1 && to_get_config.stop_bits <= UART_STOP_BITS_2) {
            config->stop_bits = to_get_config.stop_bits;
        }
        if (to_get_config.parity == UART_PARITY_DISABLE || to_get_config.parity == UART_PARITY_ODD || to_get_config.parity == UART_PARITY_EVEN) {
            config->parity = to_get_config.parity;
        }
        if (to_get_config.flow_control >= UART_HW_FLOWCTRL_DISABLE && to_get_config.flow_control <= UART_HW_FLOWCTRL_CTS_RTS) {
            config->flow_ctrl = to_get_config.flow_control;
        }
        return true;
    }

    return false;
}

#endif
