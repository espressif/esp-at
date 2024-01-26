/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "at_ota_token.h"

typedef enum {
    ESP_AT_OTA_MODE_NORMAL = 0,             /**< upgrade mode is TCP */
    ESP_AT_OTA_MODE_SSL,                    /**< upgrade mode is TLS */
    ESP_AT_OTA_MODE_MAX,
} at_ota_mode_t;

const char *esp_at_get_ota_token_by_id(uint32_t id, at_ota_mode_t mode);
