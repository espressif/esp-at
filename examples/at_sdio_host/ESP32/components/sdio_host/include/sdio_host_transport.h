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

#define HOST_SLC0_RX_NEW_PACKET_INT_ST  (BIT(23))
#define HOST_SLC0_TOHOST_BIT0_INT_ST  (BIT(0))

/**
 * Init SDIO host and slave
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_init(void);

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
sdio_err_t sdio_host_wait_int(uint32_t timeout);

/** Get interrupt bits of SDIO slave.
 *
 * @param intr_raw Output of the raw interrupt bits. Set to NULL if only masked bits are read.
 * @param intr_st Output of the masked interrupt bits. set to NULL if only raw bits are read.
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_host_get_intr(uint32_t *intr_raw, uint32_t *intr_st);

/** Clear interrupt bits of SDIO slave. All the bits set in the mask will be cleared, while other bits will stay the same.
 *
 * @param intr_mask Mask of interrupt bits to clear.
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_host_clear_intr(uint32_t intr_mask);

/** Get a packet from SDIO slave.
 *
 * @param[out] out_data Data output address
 * @param size The size of the output buffer, if the buffer is smaller than the size of data to receive from slave, the driver returns ``ESP_ERR_NOT_FINISHED``
 * @param[out] out_length Output of length the data actually received from slave.
 * @param wait_ms Time to wait before timeout, in ms.
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_host_get_packet(void* out_data, size_t size, size_t *out_length, uint32_t wait_ms);

/** Send a packet to the SDIO slave.
 *
 * @param start Start address of the packet to send
 * @param length Length of data to send, if the packet is over-size, the it will be divided into blocks and hold into different buffers automatically.
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_host_send_packet(const void* start, size_t length);

/** Send a interrupt signal to the SDIO slave.
 *
 * @param intr_no interrupt number, now only support 0.
 *
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
sdio_err_t sdio_host_send_intr(uint8_t intr_no);

#ifdef __cplusplus
}
#endif
