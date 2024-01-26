/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include "sdkconfig.h"

#ifdef CONFIG_AT_BASE_ON_UART
#include "soc/soc_caps.h"
#include "hal/uart_types.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#include "esp32/rom/uart.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/uart.h"
#elif CONFIG_IDF_TARGET_ESP32C2
#include "esp32c2/rom/uart.h"
#elif CONFIG_IDF_TARGET_ESP32C6
#include "esp32c6/rom/uart.h"
#endif

/*******************************************************
 *                Macros
 *******************************************************/
#define AT_UART_BAUD_RATE_MAX                       SOC_UART_BITRATE_MAX    /**< maximum uart baud rate */
#define AT_UART_BAUD_RATE_MIN                       80                      /**< minimum uart baud rate */
#define AT_UART_BAUD_RATE_DEF                       115200                  /**< default uart baud rate */
#define AT_UART_PATTERN_TIMEOUT_MS                  20                      /**< uart pattern timeout */

#define AT_UART_PARITY_NONE                         UART_PARITY_DISABLE     /**< uart parity disable */
#define AT_UART_PARITY_EVEN                         UART_PARITY_EVEN        /**< uart parity even */
#define AT_UART_PARITY_ODD                          UART_PARITY_ODD         /**< uart parity odd */

#define AT_UART_RX_BUFFER_SIZE                      2048                    /**< uart rx buffer size */
#define AT_UART_TX_BUFFER_SIZE                      8192                    /**< uart tx buffer size */
#define AT_UART_QUEUE_SIZE                          30                      /**< uart queue size */

/**
 * @brief The default uart pins configurations of at command port
*/
#if defined(CONFIG_IDF_TARGET_ESP32)
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          17
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          16
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         15
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         14
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C3)
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          7
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          6
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         5
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         4
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C6)
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          7
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          6
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         5
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         4
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C2)
#ifdef CONFIG_ESPTOOLPY_FLASHSIZE_2MB
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          7
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          6
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         19
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         20
#else
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          7
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          6
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         5
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         4
#endif
#endif

/**
 * @brief The uart configuration
*/
typedef struct {
    int32_t baudrate;       /**< uart baud rate */
    int8_t data_bits;       /**< uart data bits */
    int8_t stop_bits;       /**< uart stop bits */
    int8_t parity;          /**< uart parity */
    int8_t flow_control;    /**< uart flow control */
} at_uart_config_t;

/**
 * @brief The uart port and pins configuration
*/
typedef struct {
    int8_t number;          /**< uart port number */
    int32_t tx_pin;         /**< uart tx pin */
    int32_t rx_pin;         /**< uart rx pin */
    int32_t cts_pin;        /**< uart cts pin */
    int32_t rts_pin;        /**< uart rts pin */
} at_uart_port_pins_t;

/**
 * @brief This function is used to get raw uart configuration from default system nvs partition.
 *
 * @param[out] config: The pointer of at_uart_config_t
 *
 * @return    true/false
*/
bool at_nvs_uart_config_get_internal(at_uart_config_t *config);

/**
 * @brief This function is used to set raw uart configuration to default system nvs partition.
 *
 * @param[in] config: The pointer of at_uart_config_t
 *
 * @return    true/false
*/
bool at_nvs_uart_config_set_internal(at_uart_config_t *config);

/**
 * @brief This function is used to get processed uart configuration from default system nvs partition.
 *
 * @param[out] config: The pointer of uart_config_t
 *
 * @return    true/false
*/
bool at_nvs_uart_config_get(uart_config_t *config);

/**
 * @brief This function is used to set processed uart configuration to default system nvs partition.
 *
 * @param[in] config: The pointer of uart_config_t
 *
 * @return    true/false
*/
void at_nvs_uart_config_set(uart_config_t *config);

/**
 * @brief This function is used to get real uart communication port.
 *
 * @note Users can change the uart communication port by modifying the default manufacturing nvs partition.
 *
 * @return uart port number
*/
uint8_t at_uart_port_get(void);

/**
 * @brief This function is used to get uart parity of communication port.
 *
 * The enumeration: uart_parity_t has a discontinuous value, which is not user-friendly.
 * Here we use a user-friendly value as an input, and convert it to a uart_parity_t value.
 *
 * @param[in] index: user-friendly uart parity
 *              - 0: Disable
 *              - 1: Odd
 *              - 2: Even
 *
 * @return uart parity, see uart_parity_t
*/
int8_t at_uart_parity_get(int8_t index);

/**
 * @brief This function is used to get uart port and uart pins from manufacturing nvs partition.
 *
 * @note If the manufacturing nvs partition is not initialized, the default uart port and pins will be returned.
 *
 * @param[out] config: The pointer of at_uart_port_pins_t
 *
 * @return
 *    - ESP_OK: succeed
 *    - others: fail
*/
esp_err_t at_mfg_uart_port_pins_get(at_uart_port_pins_t *config);

/**
 * @brief Do some possible workarounds for uart driver.
*/
void at_uart_workaround(void);

/**
 * @brief Initialize default uart configuration.
 *
 * @param[in] config: The pointer of uart_config_t
*/
void at_uart_config_init(uart_config_t *config);

/**
 * @brief Configure uart interrupt.
*/
void at_uart_intr_config(void);

#endif
