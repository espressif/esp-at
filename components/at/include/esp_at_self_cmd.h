/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include "sdkconfig.h"

/**
 *  This header file defines an AT self-interface feature.
 *
 *  AT typically communicates with the host MCU via physical interface (UART/SPI/SDIO) or virtual interface (Socket),
 *  here, we define a new self-interface that allows users to send AT commands via esp-at self instead of a physical/virtual interface,
 *  to check the response of the AT commands. This enables users to execute certain preset AT commands before AT ready,
 *  thereby modifying the default initial configuration or status of the AT firmware.
*/
#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
/**
 * @brief Get the current self-interface mode
 *
 * @return
 *    - true: the self-interface is enabled
 *    - false: otherwise
*/
bool at_self_cmd_get_mode(void);

/**
 * @brief Read the data from the self-interface
 *
 * @param[inout] buffer: the buffer to store the returned data
 * @param[in] buffer_len: the length of the buffer
 *
 * @return
 *   - the actual length of the data read
*/
int32_t at_self_cmd_read_data(uint8_t *buffer, int32_t buffer_len);

/**
 * @brief Write the data to the self-interface
 *
 * @param[in] data: the data to write
 * @param[in] len: the length of the data
 *
 * @return
 *   - the actual length of the data written
*/
int32_t at_self_cmd_write_data(uint8_t *data, int32_t len);

/**
 * @brief Get the length of the buffered data in the self-interface
 *
 * @return
 *   - the buffered length of the data in the self-interface
*/
int32_t at_self_cmd_get_data_len(void);

#endif
