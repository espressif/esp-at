/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "sdio_config.h"

#define SD_IO_CCCR_FN_ENABLE        0x02
#define SD_IO_CCCR_FN_READY         0x03
#define SD_IO_CCCR_INT_ENABLE       0x04
#define SD_IO_CCCR_BUS_WIDTH        0x07

#define CCCR_BUS_WIDTH_ECSI        (1<<5)

#define SD_IO_CCCR_BLKSIZEL         0x10
#define SD_IO_CCCR_BLKSIZEH         0x11

#define TX_BUFFER_MAX   0x1000
#define TX_BUFFER_MASK  0xFFF

#define ESP_SLAVE_CMD53_END_ADDR    0x1f800

#define ESP32_SLCHOST_BASE          0x3ff55000

#define ESP_SDIO_PKT_LEN            (ESP32_SLCHOST_BASE + 0x60)&0x3FF
#define ESP_SDIO_INT_CLR            (ESP32_SLCHOST_BASE + 0xD4)&0x3FF
#define ESP_SDIO_INT_RAW            (ESP32_SLCHOST_BASE + 0x50)&0x3FF
#define ESP_SDIO_INT_ST             (ESP32_SLCHOST_BASE + 0x58)&0x3FF
#define ESP_SDIO_TOKEN_RDATA        (ESP32_SLCHOST_BASE + 0x44)&0x3FF
#define ESP_SDIO_SEND_OFFSET        16
#define ESP_SDIO_CONF               (ESP32_SLCHOST_BASE + 0x8c)&0x3FF
#define ESP_SDIO_CONF_OFFSET        0

#define RX_BYTE_MAX                 0x100000
#define RX_BYTE_MASK                0xFFFFF
