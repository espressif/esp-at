/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"

#include "at_compress_ota.h"
#include "esp_at_ota.h"

#if defined(CONFIG_BOOTLOADER_COMPRESSED_ENABLED) && defined(CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT)
#include "bootloader_custom_ota.h"
#define AT_OTA_LEGACY_COMPRESS      1
#else
#define AT_OTA_LEGACY_COMPRESS      0
#endif

static const char *TAG = "at-ota";

typedef enum {
    AT_OTA_STATE_IDLE = 0,
    AT_OTA_STATE_ACTIVE,
} at_ota_state_t;

/* Private handle layout. Kept in the .c so that changing its members never
 * alters the ABI seen by callers/libs (they only hold an opaque pointer). */
typedef struct {
    at_ota_state_t state;
    const esp_partition_t *partition;
    union {
        /* esp_ota OTA method. Serves both the non-compressed OTA and the new compressed OTA,
         * which shares the exact same esp_ota_* write path. */
        esp_ota_handle_t normal;
        /* Legacy compressed OTA method (esp-bootloader-plus v2). Only active when
         * CONFIG_BOOTLOADER_COMPRESSED_ENABLED && CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT. */
        at_compress_ota_handle_t compress;
    } handle;
} esp_at_ota_t;

const esp_partition_t *esp_at_ota_get_update_partition(void)
{
    const esp_partition_t *update_partition = NULL;

#if AT_OTA_LEGACY_COMPRESS
    update_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, BOOTLOADER_CUSTOM_OTA_PARTITION_SUBTYPE, NULL);
    if (update_partition == NULL) {
        ESP_LOGE(TAG, "No compressed OTA partition, type:%d subtype:%d", ESP_PARTITION_TYPE_DATA, BOOTLOADER_CUSTOM_OTA_PARTITION_SUBTYPE);
    } else {
        ESP_LOGI(TAG, "Compressed OTA partition found, subtype %d at offset 0x%x", update_partition->subtype, update_partition->address);
    }
#else
    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    if (configured != running) {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                 configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);

    update_partition = esp_ota_get_next_update_partition(running);
    if (update_partition) {
        ESP_LOGI(TAG, "Next partition found, subtype %d at offset 0x%x", update_partition->subtype, update_partition->address);
        if (update_partition->address == running->address) {
            ESP_LOGE(TAG, "The partition to be updated is the current running partition");
            update_partition = NULL;
        }
    } else {
        ESP_LOGE(TAG, "No OTA update partition found");
    }
#endif

    return update_partition;
}

esp_err_t esp_at_ota_begin(esp_at_ota_handle_t *handle)
{
    if (!handle) {
        return ESP_ERR_INVALID_ARG;
    }
    *handle = NULL;

    esp_at_ota_t *ota = calloc(1, sizeof(esp_at_ota_t));
    if (!ota) {
        ESP_LOGE(TAG, "failed to allocate ota handle");
        return ESP_ERR_NO_MEM;
    }

#if AT_OTA_LEGACY_COMPRESS
    esp_err_t err = at_compress_ota_begin(&ota->handle.compress);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "at_compress_ota_begin failed");
        free(ota);
        return err;
    }
    ota->partition = ota->handle.compress.partition;
#else
    const esp_partition_t *partition = esp_at_ota_get_update_partition();
    if (partition == NULL) {
        free(ota);
        return ESP_FAIL;
    }

    esp_err_t err = esp_ota_begin(partition, OTA_SIZE_UNKNOWN, &ota->handle.normal);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
        free(ota);
        return err;
    }
    ota->partition = partition;
#endif

    ota->state = AT_OTA_STATE_ACTIVE;
    *handle = (esp_at_ota_handle_t)ota;
    return ESP_OK;
}

esp_err_t esp_at_ota_write(esp_at_ota_handle_t handle, const void *data, size_t size)
{
    esp_at_ota_t *ota = (esp_at_ota_t *)handle;
    if (ota == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (ota->state != AT_OTA_STATE_ACTIVE) {
        return ESP_ERR_INVALID_STATE;
    }

#if AT_OTA_LEGACY_COMPRESS
    return at_compress_ota_write(&ota->handle.compress, data, (int)size);
#else
    esp_err_t err = esp_ota_write(ota->handle.normal, data, size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_write failed (%s)", esp_err_to_name(err));
    }
    return err;
#endif
}

esp_err_t esp_at_ota_end(esp_at_ota_handle_t handle)
{
    esp_at_ota_t *ota = (esp_at_ota_t *)handle;
    if (ota == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (ota->state != AT_OTA_STATE_ACTIVE) {
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err;
#if AT_OTA_LEGACY_COMPRESS
    err = at_compress_ota_end(&ota->handle.compress);
#else
    err = esp_ota_end(ota->handle.normal);
    if (err != ESP_OK) {
        if (err == ESP_ERR_OTA_VALIDATE_FAILED) {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        ESP_LOGE(TAG, "esp_ota_end failed (%s)", esp_err_to_name(err));
    } else {
        err = esp_ota_set_boot_partition(ota->partition);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)", esp_err_to_name(err));
        }
    }
#endif

    free(ota);
    return err;
}

esp_err_t esp_at_ota_abort(esp_at_ota_handle_t handle)
{
    esp_at_ota_t *ota = (esp_at_ota_t *)handle;
    if (ota == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (ota->state != AT_OTA_STATE_ACTIVE) {
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err;
#if AT_OTA_LEGACY_COMPRESS
    err = ESP_OK;
#else
    err = esp_ota_abort(ota->handle.normal);
#endif

    free(ota);
    return err;
}
