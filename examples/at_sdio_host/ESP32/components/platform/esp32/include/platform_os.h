/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Delay some times
 *
 * @param uint32_t - set the timer to delay in this number of milliseconds
 * @return
 *      - SUCCESS on success
 *      - FAILURE on fail
 */
void platform_os_delay(uint32_t milliseconds);

#ifdef __cplusplus
}
#endif
