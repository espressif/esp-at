
// This define is done in Makefile. If you do not use default Makefile, uncomment
// to be able to use Heatshrink-compressed espfs images.

#include<stdio.h>
#include<stdint.h>
#include "esp_vfs.h"
#include "esp_vfs_fat.h"


typedef struct {
    int32_t totalLen;     // file total size
    char decompressor;    // no use
    int32_t posDecomp;    // file offset
    char *filename;       // file name
    FILE* filefd;       
    void *decompData;     // no use
}EspFsFile;

int espFsInit();
EspFsFile *espFsOpen(char *fileName);
int espFsFlags(EspFsFile *fh);
int espFsRead(EspFsFile *fh, char *buff, int len);
void espFsClose(EspFsFile *fh);


