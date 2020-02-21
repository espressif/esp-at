#pragma once
/*
 * spiffs.h
 *
 *  Created on: September 20, 2017
 *      Author: kearins
 */

#include "esp_err.h"

#if defined(__cplusplus)
extern "C" {
#endif


esp_err_t emulate_esp_vfs_fat_spiflash_mount(const char* base_path,
    //const char* partition_label,
    const esp_vfs_fat_mount_config_t* mount_config,
    wl_handle_t* wl_handle,
    FATFS** out_fs,
    int imageSize
);

esp_err_t emulate_esp_vfs_fat_spiflash_unmount(const char *base_path, wl_handle_t wl_handle);

//vfs.c
ssize_t emulate_esp_vfs_write(int fd, const void * data, size_t size);
off_t emulate_esp_vfs_lseek(int fd, off_t size, int mode);
ssize_t emulate_esp_vfs_read(int fd, void * dst, size_t size);
int emulate_esp_vfs_open(const char * path, int flags, int mode);
int emulate_esp_vfs_close(int fd);
int emulate_esp_vfs_fstat(int fd, struct stat * st);
int emulate_esp_vfs_stat(const char * path, struct stat * st);
int emulate_esp_vfs_link(const char* n1, const char* n2);
int emulate_esp_vfs_unlink(const char *path);
int emulate_esp_vfs_rename(const char *src, const char *dst);

//-------

DIR* emulate_vfs_opendir(const char* name);
struct dirent* emulate_vfs_readdir(DIR* pdir);
int emulate_vfs_readdir_r(DIR* pdir, struct dirent* entry, struct dirent** out_dirent);
long emulate_vfs_telldir(DIR* pdir);
void emulate_vfs_seekdir(DIR* pdir, long loc);
void emulate_vfs_rewinddir(DIR* pdir);
int emulate_vfs_closedir(DIR* pdir);
int emulate_vfs_mkdir(const char* name, mode_t mode);
int emulate_vfs_rmdir(const char* name);
int emulate_vfs_fcntl(int fd, int cmd, ...);



#if defined(__cplusplus)
}
#endif
