/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

//typedef SemaphoreHandle_t AT_MUTEX_T;
typedef void* AT_MUTEX_T;

/* Definitions for error constants. */

#define ESP_OK          0
#define ESP_FAIL        -1
#define ESP_ERR_NO_MEM          0x101
#define ESP_ERR_INVALID_ARG     0x102
#define ESP_ERR_INVALID_STATE   0x103
#define ESP_ERR_INVALID_SIZE    0x104
#define ESP_ERR_NOT_FOUND       0x105
#define ESP_ERR_NOT_SUPPORTED   0x106
#define ESP_ERR_TIMEOUT         0x107
#define ESP_ERR_INVALID_RESPONSE    0x108

#define at_debugLevel 2

#define ESP_AT_LOGE(x, ...) {if(at_debugLevel >= 0) {printf("E %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_AT_LOGW(x, ...) {if(at_debugLevel >= 1) {printf("W %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_AT_LOGI(x, ...) {if(at_debugLevel >= 2) {printf("I %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_AT_LOGD(x, ...) {if(at_debugLevel >= 3) {printf("D %s: ",x); printf(__VA_ARGS__); printf("\n");}}
#define ESP_AT_LOGV(x, ...) {if(at_debugLevel >= 4) {printf("V %s: ",x); printf(__VA_ARGS__); printf("\n");}}

// Forces data to be 4 bytes aligned
#define WORD_ALIGNED_ATTR __attribute__((aligned(4)))

/**
 * @brief Set cs line to high.
 *
 * @return None
 */
void at_cs_high(void);

/**
 * @brief Set cs line to low.
 *
 * @return None
 */
void at_cs_low(void);

/**
  * @brief  Delay some time.
  *
  * @param  uint32_t  Delay time in ms.
  *
  * @return None
  */
void at_do_delay(uint32_t wait_ms);

/**
 * @brief Send a SPI transaction, wait for it to complete, and return the result.
 *
 * @param tx_buff Pointer to transmit buffer
 * @param rx_buff Pointer to receive buffer
 * @param len     Total data length, in bytes
 *
 * @return
 *   - ESP_OK if success
 *   - ESP_ERR_INVALID_ARG if channal not valid
 */
esp_err_t at_spi_transmit(void* tx_buff, void* rx_buff, uint32_t len);

/**
 * @brief Initialize peripherals, include SPI and GPIO.
 *
 * @return
 *   - ESP_OK if success
 *   - ESP_FAIL if fail
 */
esp_err_t at_spi_slot_init(void);

/**
 * @brief Wait interrupt line.
 *
 * @param wait_ms Wait time in ms.
 *
 * @return
 *   - ESP_OK if success
 *   - ESP_ERR_TIMEOUT if timeout
 */
esp_err_t at_spi_wait_int(uint32_t wait_ms);

/**
 * @brief create a new mutex
 *
 * @return
 *   - The handle to the created mutex if the mutex type semaphore was created successfully
 *   - NULL if fail
 */
AT_MUTEX_T at_mutex_init(void);

/**
 * @brief lock a mutex
 *
 * @param pxMutex -- the mutex to lock
 *
 * @return None
 */
void at_mutex_lock(AT_MUTEX_T pxMutex);

/**
 * @brief unlock a mutex
 *
 * @param pxMutex -- the mutex to unlock
 *
 * @return None
 */
void at_mutex_unlock(AT_MUTEX_T pxMutex);

/**
 * @brief Delete a mutex
 *
 * @param pxMutex -- the mutex to delete
 *
 * @return None
 */
void at_mutex_free(AT_MUTEX_T pxMutex);
