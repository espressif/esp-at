/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "sdkconfig.h"
#include "esp_idf_version.h"

// label of the fs partition
#if CONFIG_AT_FS_FATFS
#define AT_FS_PARTITION_LABEL           "fs_storage"
#elif CONFIG_AT_FS_LITTLEFS
#define AT_FS_PARTITION_LABEL           "fs_storage"
#elif CONFIG_AT_FS_FATFS_LEGACY
#define AT_FS_PARTITION_LABEL           "fatfs"
#endif

// mount point of the fs
#if CONFIG_AT_FS_FATFS
#define AT_FS_MOUNT_POINT               "/fatfs"
#elif CONFIG_AT_FS_FATFS_LEGACY
#define AT_FS_MOUNT_POINT               "/fatfs"
#elif CONFIG_AT_FS_LITTLEFS
#define AT_FS_MOUNT_POINT               "/littlefs"
#endif

typedef enum {
    ESP_AT_FS_FATFS = 0,
    ESP_AT_FS_LITTLEFS,
    ESP_AT_FS_TYPE_MAX,
} esp_at_fs_type_t;
