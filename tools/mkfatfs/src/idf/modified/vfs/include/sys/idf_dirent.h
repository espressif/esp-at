// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * This header file provides POSIX-compatible definitions of directory
 * access functions and related data types.
 * See http://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html
 * for reference.
 */

/**
 * @brief Opaque directory structure
 */
typedef struct {
    uint16_t dd_vfs_idx; /*!< VFS index, not to be used by applications */
    uint16_t dd_rsv;     /*!< field reserved for future extension */
    /* remaining fields are defined by VFS implementation */
} DIR;

/**
 * @brief Directory entry structure
 */
struct dirent {
    int d_ino;          /*!< file number */
    uint8_t d_type;     /*!< not defined in POSIX, but present in BSD and Linux */
#define DT_UNKNOWN  0
#define DT_REG      1
#define DT_DIR      2
    char d_name[256];   /*!< zero-terminated file name */
};

//MVA add prefix vfs_ to avoid conflict with host file system
//VVV
DIR* vfs_opendir(const char* name);
struct dirent* vfs_readdir(DIR* pdir);
int vfs_readdir_r(DIR* pdir, struct dirent* entry, struct dirent** out_dirent);
long vfs_telldir(DIR* pdir);
void vfs_seekdir(DIR* pdir, long loc);
void vfs_rewinddir(DIR* pdir);
int vfs_closedir(DIR* pdir);
int vfs_mkdir(const char* name, mode_t mode);
int vfs_rmdir(const char* name);
int vfs_fcntl(int fd, int cmd, ...);
//MVA ^^^


