
#include <stdlib.h>

#include "esp_log.h"
#include "esp_err.h"
#include "wear_levelling.h"
#include "diskio.h"
#include "diskio_spiflash.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"

#include "fatfs.h"

#define MY_ALLOCATION_UNIT	512	// define sector size, valid values: 512, 1024, 2048, 4096
#define MY_CLUSTER_SIZE		(MY_ALLOCATION_UNIT * 1)	// define cluster size as sector size multiplied by 1,2,4,8

static const char *TAG = "fatfs";

static esp_partition_t s_partition = {
    /*esp_partition_type_t*/	.type = ESP_PARTITION_TYPE_DATA,		/*!< partition type (app/data) */
    /*esp_partition_subtype_t*/	.subtype = ESP_PARTITION_SUBTYPE_DATA_FAT,	/*!< partition subtype */
    /*uint32_t*/		.address = 0,					/*!< starting address of the partition in flash */
    /*uint32_t*/		.size = 0,					/*!< size of the partition, in bytes */
    /*char*/			.label = "storage",				/*!< partition label, zero-terminated ASCII string */
    /*bool*/			.encrypted = false,				/*!< flag is set to true if partition is encrypted */
};



esp_err_t emulate_esp_vfs_fat_spiflash_mount(const char* base_path,
    //const char* partition_label,
    const esp_vfs_fat_mount_config_t* mount_config,
    wl_handle_t* wl_handle,
    FATFS** out_fs,
    int imageSize)
{
    esp_err_t result = ESP_OK;
    const size_t allocation_unit = MY_ALLOCATION_UNIT;
    const size_t cluster_size = MY_CLUSTER_SIZE; //4096; reducing cluster size 4096->512
    void *workbuf = NULL;

    *out_fs = NULL;	//MVA

    s_partition.size = imageSize;
    esp_partition_t *data_partition = &s_partition;
//    esp_partition_t *data_partition = (esp_partition_t *)esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, partition_label);
//    if (data_partition == NULL) {
//        ESP_LOGE(TAG, "Failed to find FATFS partition (type='data', subtype='fat', partition_label='%s'). Check the partition table.", partition_label);
//        return ESP_ERR_NOT_FOUND;
//    }

    result = wl_mount(data_partition, wl_handle);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "failed to mount wear levelling layer. result = %i", result);
        return result;
    }
    // connect driver to FATFS
    BYTE pdrv = 0xFF;
    if (ff_diskio_get_drive(&pdrv) != ESP_OK) {
        ESP_LOGD(TAG, "the maximum count of volumes is already mounted");
        return ESP_ERR_NO_MEM;
    }
    ESP_LOGD(TAG, "using pdrv=%i", pdrv);
    char drv[3] = {(char)('0' + pdrv), ':', 0};

    result = ff_diskio_register_wl_partition(pdrv, *wl_handle);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "ff_diskio_register_wl_partition failed pdrv=%i, error - 0x(%x)", pdrv, result);
        goto fail;
    }
    FATFS *fs = NULL;
    result = esp_vfs_fat_register(base_path, drv, mount_config->max_files, &fs);
    if (result == ESP_ERR_INVALID_STATE) {
        // it's okay, already registered with VFS
    } else if (result != ESP_OK) {
        ESP_LOGD(TAG, "esp_vfs_fat_register failed 0x(%x)", result);
        goto fail;
    }
    *out_fs = fs;

    // Try to mount partition
    FRESULT fresult = f_mount(fs, drv, 1);
    if (fresult != FR_OK) {
        ESP_LOGW(TAG, "f_mount failed (%d)", fresult);
        if (!(fresult == FR_NO_FILESYSTEM && mount_config->format_if_mount_failed)) {
            result = ESP_FAIL;
            goto fail;
        }
        workbuf = malloc(cluster_size);
        ESP_LOGI(TAG, "Formatting FATFS partition: allocation_unit=%d, cluster_size=%d", allocation_unit, cluster_size);
        fresult = f_mkfs(drv, FM_ANY | FM_SFD, allocation_unit, workbuf, cluster_size);
        if (fresult != FR_OK) {
            result = ESP_FAIL;
            ESP_LOGE(TAG, "f_mkfs failed (%d)", fresult);
            goto fail;
        }
        free(workbuf);
        workbuf = NULL;
        ESP_LOGI(TAG, "Mounting again");
        fresult = f_mount(fs, drv, 0);
        if (fresult != FR_OK) {
            result = ESP_FAIL;
            ESP_LOGE(TAG, "f_mount failed after formatting (%d)", fresult);
            goto fail;
        }
    }
    return ESP_OK;

fail:
    free(workbuf);
    esp_vfs_fat_unregister_path(base_path);
    ff_diskio_unregister(pdrv);
    return result;
}


esp_err_t emulate_esp_vfs_fat_spiflash_unmount(const char *base_path, wl_handle_t wl_handle)
{
    BYTE pdrv = ff_diskio_get_pdrv_wl(wl_handle);
    if (pdrv == 0xff) {
        return ESP_ERR_INVALID_STATE;
    }
    char drv[3] = {(char)('0' + pdrv), ':', 0};

    f_mount(0, drv, 0);
    ff_diskio_unregister(pdrv);
    // release partition driver
    esp_err_t err_drv = wl_unmount(wl_handle);
    esp_err_t err = esp_vfs_fat_unregister_path(base_path);
    if (err == ESP_OK) err = err_drv;
    return err;
}

//-------------------------------
// vfs.c
//-------------------------------

ssize_t emulate_esp_vfs_write(int fd, const void * data, size_t size) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_write(r, fd, data, size);
}

off_t emulate_esp_vfs_lseek(int fd, off_t size, int mode) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_lseek(r, fd, size, mode);
}

ssize_t emulate_esp_vfs_read(int fd, void * dst, size_t size) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_read(r, fd, dst, size);
}

int emulate_esp_vfs_open(const char * path, int flags, int mode) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_open(r, path, flags, mode);
}

int emulate_esp_vfs_close(int fd) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_close(r, fd);
}

int emulate_esp_vfs_fstat(int fd, struct stat * st) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_fstat(r, fd, st);
}

int emulate_esp_vfs_stat(const char * path, struct stat * st) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_stat(r, path, st);
}

int emulate_esp_vfs_link(const char* n1, const char* n2) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_link(r, n1, n2);
}

int emulate_esp_vfs_unlink(const char *path) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_unlink(r, path);
}

int emulate_esp_vfs_rename(const char *src, const char *dst) {
  struct _idf_reent *r = __idf_getreent();
  return esp_vfs_rename(r, src, dst);
}

//-------

DIR* emulate_vfs_opendir(const char* name) {
  return vfs_opendir(name);
}

struct dirent* emulate_vfs_readdir(DIR* pdir) {
  return vfs_readdir(pdir);
}

int emulate_vfs_readdir_r(DIR* pdir, struct dirent* entry, struct dirent** out_dirent) {
  return vfs_readdir_r(pdir, entry, out_dirent);
}

long emulate_vfs_telldir(DIR* pdir) {
  return vfs_telldir(pdir);
}

void emulate_vfs_seekdir(DIR* pdir, long loc) {
  return vfs_seekdir(pdir, loc);
}

void emulate_vfs_rewinddir(DIR* pdir) {
  return vfs_rewinddir(pdir);
}

int emulate_vfs_closedir(DIR* pdir) {
  return vfs_closedir(pdir);
}

int emulate_vfs_mkdir(const char* name, mode_t mode) {
  return vfs_mkdir(name, mode);
}

int emulate_vfs_rmdir(const char* name) {
  return vfs_rmdir(name);
}

int emulate_vfs_fcntl(int fd, int cmd, ...) {
  int result;
  va_list args;
  va_start(args, cmd);
  result = vfs_fcntl(fd, cmd, args);
  va_end(args);
  return result;
}


