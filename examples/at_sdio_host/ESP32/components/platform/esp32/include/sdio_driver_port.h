/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "sdio_host_error.h"

/**
 * Probe and initialize SDIO slave using given host
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_init(void);

/**
 * Write blocks of data to an SDIO card using CMD53
 *
 * This function performs write operation using CMD53 in block mode.
 * For byte mode, see sdio_driver_write_bytes.
 *
 * @param function  IO function number
 * @param addr  byte address within IO function where writing starts
 * @param buffer  data to be written
 * @param len  number of bytes to read, must be divisible by the card block
 *              size.
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_write_blocks(uint32_t function, uint32_t addr, void* buffer, uint32_t len);

/**
 * Read blocks of data from an SDIO card using CMD53
 *
 * This function performs read operation using CMD53 in block mode.
 * For byte mode, see sdio_driver_read_bytes.
 *
 * @param function  IO function number
 * @param addr  byte address within IO function where writing starts
 * @param buffer  buffer which receives the data read from card
 * @param len  number of bytes to read, must be divisible by the card block
 *              size.
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_read_blocks(uint32_t function, uint32_t addr, void* buffer, uint32_t len);

/**
 * Read multiple bytes from an SDIO card using CMD53
 *
 * This function performs read operation using CMD53 in byte mode.
 * For block mode, see sdio_driver_read_blocks.
 *
 * @param function  IO function number
 * @param addr  byte address within IO function where reading starts
 * @param dst  buffer which receives the data read from card
 * @param size  number of bytes to read
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_read_bytes(uint32_t function, uint32_t addr, void* buffer, uint32_t len);

/**
 * Write multiple bytes to an SDIO card using CMD53
 *
 * This function performs write operation using CMD53 in byte mode.
 * For block mode, see sdio_driver_write_blocks.
 *
 * @param function  IO function number
 * @param addr  byte address within IO function where writing starts
 * @param buffer  data to be written
 * @param len  number of bytes to write
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_write_bytes(uint32_t function, uint32_t addr, void* buffer, uint32_t len);

/**
 * Read one byte from SDIO slave using CMD52
 *
 * @param function  IO function number
 * @param reg  byte address within IO function
 * @param[out] out_byte  output, receives the value read from the card
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_read_byte(uint32_t function, uint32_t reg, uint8_t *out_byte);

/**
 * Write one byte to SDIO slave using CMD52
 *
 * @param function  IO function number
 * @param reg  byte address within IO function
 * @param in_byte  value to be written
 * @param[out] out_byte  if not NULL, receives new byte value read
 *                       from the card (read-after-write).
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_driver_write_byte(uint32_t function, uint32_t reg, uint8_t in_byte, uint8_t* out_byte);

/**
 * Block until an SDIO interrupt is received
 *
 * Slave uses D1 line to signal interrupt condition to the host.
 * This function can be used to wait for the interrupt.
 *
 * @param timeout  time to wait for the interrupt, in ms
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 *      - ERR_TIMEOUT if the interrupt does not happen in timeout
 */
sdio_err_t sdio_driver_wait_int(uint32_t timeout);

#ifdef __cplusplus
}
#endif
