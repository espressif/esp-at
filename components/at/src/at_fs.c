/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_at_types.h"
#include "esp_at.h"

#if CONFIG_AT_FS_LITTLEFS
#include "esp_littlefs.h"
#else
#include "esp_vfs_fat.h"
#endif

static bool s_fs_mounted = false;

#if defined(CONFIG_AT_FS_FATFS) || defined(CONFIG_AT_FS_FATFS_LEGACY)
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
#endif

static const char *TAG = "at-fs";

const char *esp_at_fs_get_mount_point(void)
{
    return AT_FS_MOUNT_POINT;
}

at_fs_type_t esp_at_fs_get_type(void)
{
#if CONFIG_AT_FS_LITTLEFS
    return AT_FS_TYPE_LITTLEFS;
#else
    return AT_FS_TYPE_FATFS;
#endif
}

bool esp_at_fs_mount(void)
{
    if (s_fs_mounted) {
        ESP_LOGD(TAG, "File system already mounted");
        return true;
    }

#if CONFIG_AT_FS_LITTLEFS
    const esp_vfs_littlefs_conf_t mount_config = {
        .base_path = AT_FS_MOUNT_POINT,
        .partition_label = AT_FS_PARTITION_LABEL,
        .format_if_mount_failed = true,
        .dont_mount = false,
    };
    esp_err_t ret = esp_vfs_littlefs_register(&mount_config);
#else
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
    };
    esp_err_t ret = esp_vfs_fat_spiflash_mount_rw_wl(AT_FS_MOUNT_POINT, AT_FS_PARTITION_LABEL, &mount_config, &s_wl_handle);
#endif
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to mount fs, error: %s", esp_err_to_name(ret));
        return false;
    }

    s_fs_mounted = true;
    ESP_LOGD(TAG, "File system mounted successfully");
    return true;
}

bool esp_at_fs_unmount(void)
{
    if (!s_fs_mounted) {
        return true;
    }

#if CONFIG_AT_FS_LITTLEFS
    esp_err_t ret = esp_vfs_littlefs_unregister(AT_FS_PARTITION_LABEL);
#else
    esp_err_t ret = esp_vfs_fat_spiflash_unmount_rw_wl(AT_FS_MOUNT_POINT, s_wl_handle);
#endif
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to unmount fs, error: %s", esp_err_to_name(ret));
        return false;
    }

    s_fs_mounted = false;
    ESP_LOGD(TAG, "File system unmounted successfully");
    return true;
}

esp_err_t esp_at_get_fs_info(uint32_t *out_total_bytes, uint32_t *out_free_bytes)
{
    if (!out_total_bytes || !out_free_bytes) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!s_fs_mounted) {
        ESP_LOGE(TAG, "File system not mounted");
        return ESP_ERR_INVALID_STATE;
    }

#if CONFIG_AT_FS_LITTLEFS
    size_t total_size = 0, used_size = 0;
    esp_err_t ret = esp_littlefs_info(AT_FS_PARTITION_LABEL, &total_size, &used_size);
    if (ret == ESP_OK) {
        *out_total_bytes = (uint32_t)total_size;
        *out_free_bytes = (uint32_t)(total_size - used_size);
    }
#else
    uint64_t total_size = 0, free_size = 0;
    esp_err_t ret = esp_vfs_fat_info(AT_FS_MOUNT_POINT, &total_size, &free_size);
    if (ret == ESP_OK) {
        *out_total_bytes = (uint32_t)total_size;
        *out_free_bytes = (uint32_t)free_size;
    }
#endif
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get file system info (%s)", esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}
