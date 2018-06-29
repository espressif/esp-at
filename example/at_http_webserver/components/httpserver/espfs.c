#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "espfs.h"
#include "esp_parse_at.h"

// Donnot care flag
int espFsFlags(EspFsFile* fh)
{
    return 0;
}

#ifndef ESP_AT_FATFS_COMMAND

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

// Mount path for the partition
const char* base_path = "/fatfs";

/*
*  Init file system, assert we can mount the fs.
*/
int espFsInit(void)
{
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 10,
        .format_if_mount_failed = false    // If cannot mount fs, no need to go down
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);

    if (err != ESP_OK) {
        printf("Failed to mount FATFS (0x%x)", err);
        return 0;
    }

    printf("Mount FATFS success\n");
    return 1;
}

/*
*  Just open the file and assign to the espFsFile pointer
*/
EspFsFile* espFsOpen(char* fileName)
{
    struct stat st;
    FILE* f;
    memset(&st, 0x0, sizeof(st));

    char* total_filename = (char*)malloc(strlen(base_path) + strlen((char*)fileName) + 2);

    if (total_filename == NULL) {
        return NULL;
    }

    sprintf(total_filename, "%s%s", base_path, fileName);


    if (stat(total_filename, &st) != 0) {
        printf("Can not open fat file:%s\n", total_filename);
        free(total_filename);
        return NULL;
    }

    printf("File:%s size : %d\n", total_filename, (uint32_t)st.st_size);

    f = fopen((char*)total_filename, "rb");

    if (f == NULL) {
        printf("open %s fail\n", fileName);
        free(total_filename);
        return NULL;
    }

    EspFsFile* r = (EspFsFile*)malloc(sizeof(EspFsFile));

    if (r == NULL) {
        free(total_filename);
        fclose(f);
        return NULL;
    }

    r->filename = total_filename;
    r->filefd = f;
    r->totalLen = (uint32_t)st.st_size;
    r->decompressor = 0;
    r->posDecomp = 0;
    return r;
}

/*
*  Read the file content according to the len and offset
*/
int espFsRead(EspFsFile* fh, char* buff, int len)
{
    uint8_t* data = NULL;

    if (fh == NULL) {
        return 0;
    }

    if (fh->decompressor == 0) {
        int toRead;
        toRead = fh->totalLen - fh->posDecomp;

        if (len > toRead) {
            len = toRead;
        }

        data = (uint8_t*)malloc((len + 1) * sizeof(uint8_t));

        if (data == NULL) {
            return 0;
        }

        memset(data, '\0', len + 1);

        if (fseek(fh->filefd, fh->posDecomp, SEEK_SET)) {
            printf("set %s  offset fail\n", fh->filename);
            fclose(fh->filefd);
            free(data);
            data = NULL;
            return 0;
        }

        if (fread(data, len, 1, fh->filefd) != 1) {
            printf("read %s fail\n", fh->filename);
            fclose(fh->filefd);
            free(data);
            data = NULL;
            return 0;
        }

        memcpy(buff, data, len);
        fh->posDecomp += len;
        free(data);
        return len;
    }

    return 0;
}

void espFsClose(EspFsFile* fh)
{
    if (fh == NULL) {
        return;
    }

    fclose(fh->filefd);
    free(fh->filename);
    free(fh);
}
#else

void espFsClose(EspFsFile* fh)
{
    if (fh == NULL) {
        return;
    }

    free(fh);
}

EspFsFile* espFsOpen(char* fileName)
{
    int file_len = 0;
    file_len = esp_at_fatfs_open(fileName);

    if (file_len < 0) {
        printf("Cannot open file:%s\n", fileName);
        return NULL;
    }

    EspFsFile* r = (EspFsFile*)malloc(sizeof(EspFsFile));

    if (r == NULL) {
        return NULL;
    }

    printf("FATFS open File %s len:%d\n", fileName, file_len);

    r->filename = fileName;
    r->totalLen = file_len;
    r->decompressor = 0;
    r->posDecomp = 0;

    return r;
}

int espFsRead(EspFsFile* fh, char* buff, int len)
{
    char* read_data = NULL;

    if (fh == NULL) {
        return 0;
    }

    if (fh->decompressor == 0) {
        int toRead;
        toRead = fh->totalLen - fh->posDecomp;

        if (len > toRead) {
            len = toRead;
        }

        read_data = esp_at_fatfs_read(fh->filename, fh->posDecomp, len);

        if (read_data == NULL) {
            printf("Cannot read file:%s\n", fh->filename);
            return 0;
        }

        memcpy(buff, read_data, len);
        fh->posDecomp += len;
        free(read_data);
        return len;
    }

    return 0;
}
#endif
