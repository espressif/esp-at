/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*! \public
 * @brief SDIO host Error enum
 *
 * Values less than -1 are specific error codes
 * Value of -1 is a generic failure response
 * Value of 0 is a generic success response
 */
typedef enum {
    SUCCESS          =  0,
    FAILURE          = -1,
    ERR_TIMEOUT      = -2,
    ERR_INVALID_ARG  = -3,
    ERR_NO_MEMORY    = -4,
    ERR_NOT_FOUND    = -5,
    ERR_NOT_FINISHED = -6
} sdio_err_t;

#ifdef __cplusplus
}
#endif
