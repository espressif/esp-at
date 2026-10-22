/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "at_ota_token.h"
#include "esp_err.h"

typedef enum {
    AT_OTA_OVER_HTTP = 0,                   /**< HTTP */
    AT_OTA_OVER_HTTPS,                      /**< HTTPS, server certificate is not verified */
    AT_OTA_OVER_HTTPS_VERIFY,               /**< HTTPS, verify the server certificate */
    AT_OTA_OVER_MAX,
} at_ota_mode_t;

/**
 * @brief  Get the OTA server token for one module.
 *
 * AT_OTA_OVER_HTTP selects the HTTP token. AT_OTA_OVER_HTTPS selects the SSL
 * token. Any other mode, including AT_OTA_OVER_HTTPS_VERIFY, is treated as
 * unknown. The caller does not own the returned string.
 *
 * @param[in] id    Module id, normally from esp_at_get_module_id().
 * @param[in] mode  AT_OTA_OVER_HTTP or AT_OTA_OVER_HTTPS.
 *
 * @return
 *    - Token string when the module and mode are known.
 *    - "Unknown" when the module id or mode has no token.
 */
const char *esp_at_get_ota_token_by_id(uint32_t id, at_ota_mode_t mode);

/**
 * @brief  Load one CA certificate from the manufacturing NVS.
 *
 * Reads the blob ota_ca.<ca_id> from the ota_ca namespace. On success
 * @p ca_pem receives a NUL-terminated PEM that the caller must free. On
 * error, @p ca_pem is set to NULL. A missing or zero-length blob is an
 * error; there is no fallback to an unverified connection.
 *
 * @param[in]  ca_id   Certificate set index. Negative values are rejected.
 * @param[out] ca_pem  Receives the allocated PEM.
 *
 * @return
 *    - ESP_OK: certificate loaded.
 *    - ESP_ERR_INVALID_ARG: ca_id is negative, or the NVS key does not fit.
 *    - ESP_ERR_NO_MEM: could not allocate the PEM buffer.
 *    - ESP_FAIL: the blob is missing or has length 0.
 *    - others: failed.
 */
esp_err_t esp_at_ota_load_ca(int ca_id, char **ca_pem);

/**
 * @brief  Check that the system clock can verify a certificate.
 *
 * mbedTLS rejects a certificate while the clock is still at the epoch. The
 * clock must be at or after 2020-01-01 00:00:00 UTC. This function does not
 * start SNTP; the host sets the clock with AT+CIPSNTPCFG.
 *
 * @return
 *    - ESP_OK: the clock is usable.
 *    - ESP_FAIL: the clock is earlier than 2020-01-01 00:00:00 UTC.
 */
esp_err_t esp_at_ota_check_time(void);
