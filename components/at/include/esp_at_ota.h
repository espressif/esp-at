/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "esp_err.h"
#include "esp_partition.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Unified AT OTA abstraction
 *
 * Wraps two firmware OTA methods behind a single set of APIs:
 *   - Legacy compressed OTA (at_compress_ota_*), used with esp-bootloader-plus v2.
 *   - esp_ota OTA (esp_ota_*), used by both the non-compressed OTA and the new compressed OTA.
 * Which method is active is decided at compile time by
 * CONFIG_BOOTLOADER_COMPRESSED_ENABLED && CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT,
 * so the callers stay configuration-agnostic.
 *
 * Handle usage follows the same pattern as esp_https_ota:
 *   begin() takes esp_at_ota_handle_t * (out parameter);
 *   write()/end()/abort() take esp_at_ota_handle_t by value.
 * Call either end() or abort() exactly once after a successful begin().
 */

/**
 * @brief Opaque handle for a unified firmware OTA update.
 *
 * Created by esp_at_ota_begin() and released by esp_at_ota_end() or
 * esp_at_ota_abort().
 */
typedef void *esp_at_ota_handle_t;

/**
 * @brief  Find the firmware partition that will receive the OTA update.
 *
 * For compress OTA it is the dedicated compressed-image data partition.
 * For normal OTA it is the next update partition after the running one.
 *
 * @note It is useful for an oversize check / availability check before esp_at_ota_begin().
 *
 * @return
 *    - Pointer to the target partition on success.
 *    - NULL if no suitable partition is found.
 */
const esp_partition_t *esp_at_ota_get_update_partition(void);

/**
 * @brief  Commence a firmware OTA upgrade.
 *
 * Allocates a handle, finds the target partition, erases it, and initializes
 * the handle. On success @p handle receives a valid handle that must be
 * released with esp_at_ota_end() or esp_at_ota_abort(). On error, @p handle
 * is explicitly set to NULL.
 *
 * @param[out] handle  Pointer to an esp_at_ota_handle_t which will be initialised.
 *
 * @return
 *    - ESP_OK: OTA operation commenced successfully.
 *    - ESP_ERR_INVALID_ARG: handle is NULL.
 *    - ESP_ERR_NO_MEM: could not allocate the handle.
 *    - others: failed.
 */
esp_err_t esp_at_ota_begin(esp_at_ota_handle_t *handle);

/**
 * @brief  Write firmware OTA data.
 *
 * @param[in] handle  Handle obtained from esp_at_ota_begin().
 * @param[in] data    Data buffer to write.
 * @param[in] size    Size of data buffer in bytes.
 *
 * @return
 *    - ESP_OK: data written successfully.
 *    - ESP_ERR_INVALID_ARG: handle is NULL.
 *    - ESP_ERR_INVALID_STATE: handle is not in an active OTA state.
 *    - others: failed.
 */
esp_err_t esp_at_ota_write(esp_at_ota_handle_t handle, const void *data, size_t size);

/**
 * @brief  Finish a firmware OTA upgrade, validate the newly written image and
 *         release the handle.
 *
 * For normal OTA this also configures the OTA data so the next reboot boots the
 * updated partition (compress OTA leaves this to esp-bootloader-plus). The
 * handle is freed regardless of the result and must not be used afterwards.
 *
 * @param[in] handle  Handle obtained from esp_at_ota_begin().
 *
 * @return
 *    - ESP_OK: image is valid (and boot partition set for normal OTA).
 *    - ESP_ERR_INVALID_ARG: handle is NULL.
 *    - ESP_ERR_INVALID_STATE: handle is not in an active OTA state.
 *    - others: failed.
 */
esp_err_t esp_at_ota_end(esp_at_ota_handle_t handle);

/**
 * @brief  Abort a firmware OTA upgrade and release the handle.
 *
 * Safe to call on the error path. The handle is freed and must not be used
 * afterwards. For compress OTA the underlying abort is a no-op.
 *
 * @param[in] handle  Handle obtained from esp_at_ota_begin().
 *
 * @return
 *    - ESP_OK: aborted successfully.
 *    - ESP_ERR_INVALID_ARG: handle is NULL.
 *    - ESP_ERR_INVALID_STATE: handle is not in an active OTA state.
 *    - others: failed.
 */
esp_err_t esp_at_ota_abort(esp_at_ota_handle_t handle);

#ifdef __cplusplus
}
#endif
