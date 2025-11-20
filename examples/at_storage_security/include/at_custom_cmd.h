/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include "esp_at_core.h"
#include "esp_at.h"

/**
 * @brief Wrap functions for nvs set/get blob and string
*/
esp_err_t __real_nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length);
esp_err_t __real_nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length);
esp_err_t __real_nvs_set_str(nvs_handle_t handle, const char *key, const char *value);
esp_err_t __real_nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length);
