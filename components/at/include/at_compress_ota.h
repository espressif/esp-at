/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_partition.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Compress OTA
 *
 *  An OTA method for saving flash space. Typical workflow:
 *  Compress the firmware to be upgraded by compression algorithm, and pack into compressed image,
 *  which inludes compressed image header (indicates the compressed image info and verify info) and compressed image body (compressed firmware).
 *  Compress OTA will download the compressed image into special partition slot (type:0x01 subtype:0x22).
 *  After reboot the module, esp-bootloader-plus (https://github.com/espressif/esp-bootloader-plus) will check if firmware needs to be upgraded.
 *  If yes, esp-bootloader-plus will verify the integrity of compressed image, and then decompress it into the bootable partition slot.
 */

typedef struct {
    uint32_t wrote_size;                    /*!< The compressed image size that has been written to flash */
    uint32_t compressed_img_size;           /*!< The compressed image size to be written */
    bool image_header_checked;              /*!< The compressed image header checked done */
    const esp_partition_t *partition;       /*!< The partition to hold on compressed image */
} at_compress_ota_handle_t;

/**
 * @brief  Commence an Compress OTA upgrade writing to the specified partition.
 *
 * @param[out]   handle On success, returns a handle which should be used for subsequent at_compress_ota_write() and at_compress_ota_end() calls.
 *
 * @return
 *    - ESP_OK: Compress OTA operation commenced successfully.
 *    - others: Compress OTA operation commenced failed.
 */
esp_err_t at_compress_ota_begin(at_compress_ota_handle_t *handle);

/**
 * @brief  Write Compress OTA upgrade data to partition.
 *
 * @param[in] handle    Handle obtained from at_compress_ota_begin()
 * @param[in] data      Data buffer to write
 * @param[in] size      Size of data buffer in bytes
 *
 * @return
 *    - ESP_OK: Data was written to flash successfully.
 *    - others: Data was written to flash failed.
 */
esp_err_t at_compress_ota_write(at_compress_ota_handle_t *handle, const void *data, int size);

/**
 * @brief  Finish Compress OTA upgrade and validate newly written compressed image.
 *
 * @param[in] handle    Handle obtained from at_compress_ota_begin()
 *
 * @return
 *    - ESP_OK: Newly written Compressed image is valid.
 *    - others: Newly written Compressed image is invalid.
 */
esp_err_t at_compress_ota_end(at_compress_ota_handle_t *handle);

/**
 * @brief  HTTP/HTTPS Compress OTA upgrade.
 *
 * @param[in] config    Pointer to esp_http_client_config_t structure
 *
 * @return
 *    - ESP_OK: Compress OTA upgrade done, next reboot will use specified partition.
 *    - others: Compress OTA upgrade failed.
 */
esp_err_t at_compress_https_ota(const esp_http_client_config_t *config);

#ifdef __cplusplus
}
#endif
