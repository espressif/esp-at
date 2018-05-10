#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "espfs.h"

// Donnot care flag
int espFsFlags(EspFsFile* fh)
{
    return 0;
}

#if 1

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
#endif

void espFsClose(EspFsFile* fh)
{
    if (fh == NULL) {
        return;
    }

    fclose(fh->filefd);
    free(fh->filename);
    free(fh);
}

#if 0
char* fs_data = NULL;
EspFsFile* espFsOpen(char* fileName)
{
    at_cmd_arg cmd_arg;
    at_response cmd_rsp;
    int file_len = 0;
    cmd_arg.fsopen.filename = fileName;
    cmd_rsp = at_add_cmd(AT_FSOPEN, cmd_arg);

    if (cmd_rsp.rsp_flag != AT_CMD_RETURN_SUCCESS) {
        // ESP_LOGE(TAG, "Cannot open file:%s", fileName);
        printf("Cannot open file:%s\n", fileName);
        return NULL;
    }

    file_len = atoi((char*)cmd_rsp.data);
    printf("File %s len:%d\n", fileName, file_len);
    EspFsFile* r = (EspFsFile*)malloc(sizeof(EspFsFile));

    //fs_data = malloc(file_len * sizeof(char));
    if (r == NULL) {
        return NULL;
    }

    //r->header=NULL;
    r->filename = fileName;
    r->totalLen = file_len;
    r->decompressor = 0;
    r->posDecomp = 0;

    return r;
}

int espFsRead(EspFsFile* fh, char* buff, int len)
{
    int flen;
    at_cmd_arg cmd_arg;
    at_response cmd_rsp;

    if (fh == NULL) {
        return 0;
    }

//    memcpy(fs_data, cmd_rsp.data,file_len);

    if (fh->decompressor == 0) {
        int toRead;
        toRead = fh->totalLen - fh->posDecomp;

        if (len > toRead) {
            len = toRead;
        }

        cmd_arg.fsread.filename = fh->filename;
        cmd_arg.fsread.offset = fh->posDecomp;
        cmd_arg.fsread.length = len;
        cmd_rsp = at_add_cmd(AT_FSREAD, cmd_arg);

        if (cmd_rsp.rsp_flag != AT_CMD_RETURN_SUCCESS) {
            printf("Cannot read file:%s\n", fh->filename);
            return 0;
        }

        memcpy(buff, cmd_rsp.data, len);
        fh->posDecomp += len;
        //fh->posComp+=len;
        return len;
    }

    return 0;
}
#endif


