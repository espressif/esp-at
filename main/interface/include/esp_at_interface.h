/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include "esp_at_core.h"

#ifdef CONFIG_AT_BASE_ON_UART
#include "at_uart.h"
#endif

/**
 * @brief This function is used to initialize at interface.
 *
 * @note You can use one interface among UART/SPI/SDIO/Socket to enable communication between your MCU and the Espressif module.
 * @note The available interfaces might be multiple, but only one interface can be used at a time.
*/
void at_interface_init(void);

/**
 * @brief This function is used to intialize the interface operations for communication port.
 *
 * @note Each interface must have its own operations, you can use the default operations or customize your own operations.
 * @note The interface operations include: read data, write data, get data length, wait data tx done.
 *
 * @param[in] ops: The pointer of the interface operations.
*/
void at_interface_ops_init(esp_at_device_ops_struct *ops);

/**
 * @brief This function is used to initialize the interface-hooks for communication port.
 *
 * There are two types of hooks: system-hooks and interface-hooks. This function is used to initialize the interface-hooks.
 * When the system state comes to the a special state, the corresponding hooks will be called.
 * The system-hooks will be called first, then the interface-hooks will be called.
 *
 * There are some special states:
 *      - AT is ready to switch transmit mode
 *      - AT is ready to enter light-sleep/modem sleep/non-sleep
 *      - AT is ready to enter deep-sleep
 *      - AT is ready to restart
 *      - AT is ready to active write data

 * @note Each interface may have its own hooks, you can use the default hooks or customize your own hooks.
 * @param[in] if_hooks: The pointer of the interface-hooks.
 *              - NULL: use the system-hooks.
 *              - not NULL: use the system-hooks and the interface-hooks.
*/
void at_interface_hooks(esp_at_custom_ops_struct *if_hooks);

/**
 * @brief This function is used to get the raw write pointer from interface.
 *
 * @return
 *    at_write_data_fn_t: The pointer of the raw write function.
*/
at_write_data_fn_t at_interface_get_write_fn(void);

/**
 * @brief This function is used to get the raw read pointer from interface.
 *
 * @return
 *    at_read_data_fn_t: The pointer of the raw read function.
*/
at_read_data_fn_t at_interface_get_read_fn(void);

typedef struct {
    int (*open)(void);                                              /*!< initialize the security channel over interface */
    int32_t (*read)(uint8_t *data, int32_t size);                   /*!< read out the plain data from security channel */
    int32_t (*write)(uint8_t *data, int32_t size);                  /*!< write the plain data to security channel */
    void (*close)(void);                                            /*!< free the security channel */
} at_intf_security_ops_t;

/**
 * @brief Establish a security channel over the communicate interface (UART/SPI/SDIO/Socket)
 *
 * @param[in] ops: The pointer of the security operations.
 *
 * @return
 *      - 0: success
 *      - others: fail
*/
int at_interface_security_set(at_intf_security_ops_t *ops);
