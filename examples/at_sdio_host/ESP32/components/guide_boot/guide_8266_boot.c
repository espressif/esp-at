#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stdlib.h"

#include "eagle_fw.h"
#include "fw_boot.h"

//#include "platform_os.h"
#include "sdio_driver_port.h"
#include "sdio_host_reg.h"

#define ESP_SLVE_BLOCK 0

static sdio_err_t sip_write_memory(uint8_t* rawbuf, uint32_t txseq, uint32_t addr, const uint8_t* buf, uint16_t len)
{
    struct sip_cmd_write_memory* cmd;
    struct sip_hdr* chdr;
    uint16_t remains, hdrs, bufsize;
    uint32_t loadaddr;
    const uint8_t* src;
    int err = 0;

    memset(rawbuf, 0x0, SIP_BOOT_BUF_SIZE);

    chdr = (struct sip_hdr*)rawbuf;
    SIP_HDR_SET_TYPE(chdr->fc[0], SIP_CTRL);
    chdr->cmdid = SIP_CMD_WRITE_MEMORY;

    remains = len;
    hdrs = sizeof(struct sip_hdr) + sizeof(struct sip_cmd_write_memory);

    while (remains) {
        src = &buf[len - remains];
        loadaddr = addr + (len - remains);

        if (remains < (SIP_BOOT_BUF_SIZE - hdrs)) {
            /* aligned with 4 bytes */
            bufsize = roundup(remains, 4);
            memset(rawbuf + hdrs, 0x0, bufsize);
            remains = 0;

        } else {
            bufsize = SIP_BOOT_BUF_SIZE - hdrs;
            remains -=  bufsize;
        }

        chdr->len = bufsize + hdrs ;
        chdr->seq = txseq++;
        cmd = (struct sip_cmd_write_memory*)(rawbuf + SIP_CTRL_HDR_LEN);
        cmd->len = bufsize;
        cmd->addr = loadaddr;

        memcpy(rawbuf + hdrs, src, bufsize);


        if (ESP_SLVE_BLOCK) {
            err = sdio_driver_write_blocks(1, ESP_SLAVE_CMD53_END_ADDR - chdr->len, rawbuf, 1);
        } else {
            err = sdio_driver_write_bytes(1, ESP_SLAVE_CMD53_END_ADDR - chdr->len, rawbuf, (chdr->len + 3) & (~3));
        }

        if (err) {
            printf("Send buffer failed\n");
            return FAILURE;
        }

    }

    return SUCCESS;
}

static sdio_err_t sip_send_cmd(uint32_t txseq, int cid, uint32_t cmdlen, void* cmd)
{
    int err = 0;
    struct sip_hdr* chdr;
    uint8_t* buffer = malloc(sizeof(struct sip_hdr) + sizeof(struct sip_cmd_bootup));
    memset(buffer, 0x0, (sizeof(struct sip_hdr) + sizeof(struct sip_cmd_bootup)));
    chdr = (struct sip_hdr*)buffer;
    SIP_HDR_SET_TYPE(chdr->fc[0], SIP_CTRL);

    chdr->len = SIP_CTRL_HDR_LEN + cmdlen;

    chdr->seq = txseq++;
    chdr->cmdid = cid;

    memcpy(buffer + sizeof(struct sip_hdr), (uint8_t*)cmd, cmdlen);

    if (ESP_SLVE_BLOCK) {
        err = sdio_driver_write_blocks(1, ESP_SLAVE_CMD53_END_ADDR - chdr->len, buffer, 1);
    } else {
        err = sdio_driver_write_bytes(1, ESP_SLAVE_CMD53_END_ADDR - chdr->len, buffer, (chdr->len + 3) & (~3));
    }

    if (err) {
        printf("Send buffer failed\n");
        return FAILURE;
    }

    return SUCCESS;
}

sdio_err_t esp_download_fw(void)
{
    const unsigned char* fw_buf = NULL;
    uint32_t offset = 0;
    int ret = SUCCESS;
    uint8_t blocks;
    struct esp_fw_hdr* fhdr;
    struct esp_fw_blk_hdr* bhdr = NULL;
    struct sip_cmd_bootup bootcmd;

    uint32_t txseq = 0;
    fw_buf = &eagle_fw[0];

    uint8_t* rawbuf = malloc(SIP_BOOT_BUF_SIZE);

    if (rawbuf == NULL) {
        printf("sip rawbuf memory not enough\n");
        return ERR_NO_MEMORY;
    }

    fhdr = (struct esp_fw_hdr*)fw_buf;

    if (fhdr->magic != 0xE9) {
        printf("wrong magic! \n");
        return FAILURE;
    }

    blocks = fhdr->blocks;
    offset += sizeof(struct esp_fw_hdr);

    while (blocks) {
        bhdr = (struct esp_fw_blk_hdr*)(&fw_buf[offset]);
        offset += sizeof(struct esp_fw_blk_hdr);

        printf("blocks:%x ->%x,%x\n", blocks, bhdr->load_addr, bhdr->data_len);

        ret = sip_write_memory(rawbuf, txseq, bhdr->load_addr, &fw_buf[offset], bhdr->data_len);

        blocks--;
        offset += bhdr->data_len;
    }

    // send boot up command
    bootcmd.boot_addr = fhdr->entry_addr;
    ret = sip_send_cmd(txseq, SIP_CMD_BOOTUP, sizeof(struct sip_cmd_bootup), &bootcmd);

    return ret;
}