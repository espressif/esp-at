
/* max 16 types */
typedef enum {
        SIP_CTRL = 0,
        SIP_DATA,
        SIP_DATA_AMPDU
} SIP_TYPE;

enum sip_cmd_id {
        SIP_CMD_GET_VER = 0,
        SIP_CMD_WRITE_MEMORY,//1 ROM code
        SIP_CMD_READ_MEMORY,//2
        SIP_CMD_WRITE_REG,//3 ROM code
        SIP_CMD_READ_REG,     //4
        SIP_CMD_BOOTUP,//5 ROM code
        SIP_CMD_COPYBACK,//6
        SIP_CMD_INIT,          //7
        SIP_CMD_SCAN,//8
        SIP_CMD_SETKEY,//9
        SIP_CMD_CONFIG,//10
        SIP_CMD_BSS_INFO_UPDATE,//11
        SIP_CMD_LOOPBACK,//12  ROM code
        //do not add cmd before this line
        SIP_CMD_SET_WMM_PARAM,
        SIP_CMD_AMPDU_ACTION,
        SIP_CMD_HB_REQ, //15
        SIP_CMD_RESET_MAC, //16   0x10
        SIP_CMD_PRE_DOWN,  //17
        SIP_CMD_SLEEP,        /* for sleep testing */
        SIP_CMD_WAKEUP,       /* for sleep testing */
        SIP_CMD_DEBUG,          /* for general testing */
        SIP_CMD_GET_FW_VER,  /* get fw rev. */
        SIP_CMD_SETVIF,     //0x16
        SIP_CMD_SETSTA,

        //below is for embeded driver
        SIP_CMD_RESET = 0x20,

        //For AP
        SIP_CMD_AP_CONFIG_COMMIT = 0x40,
        //For Sta
        SIP_CMD_CONNECT = 0x50,
        SIP_CMD_DISCONNECT = 0x52,
        //For Scan
        SIP_CMD_START_SCAN = 0x62,

        SIP_CMD_MAX
};

struct esp_fw_hdr {
    uint8_t magic;
    uint8_t blocks;
    uint8_t pad[2];
    uint32_t entry_addr;
};

struct esp_fw_blk_hdr {
    uint32_t load_addr;
    uint32_t data_len;
};

struct sip_cmd_write_memory {
    uint32_t addr;
    uint32_t len;
};

struct sip_hdr {
        uint8_t fc[2];
        uint16_t len;
        uint32_t cmdid;
        uint32_t seq;
};

struct sip_cmd_write_reg {
        uint32_t addr;
        uint32_t val;
};

struct sip_cmd_bootup {
        uint32_t boot_addr;
        uint32_t discard_link;
};

#define SIP_BOOT_BUF_SIZE  256                     //256

#define  roundup(x, y)  (x%y)!=0 ? (x/y+1)*y:x

#define SIP_HDR_LEN     sizeof(struct sip_hdr)
#define SIP_CTRL_HDR_LEN    SIP_HDR_LEN  /* same as sip_hdr in sip2 design */

#define SIP_TYPE_MASK 0x0f
#define SIP_HDR_SET_TYPE(fc0, type) ((fc0) = ((fc0) & ~ SIP_TYPE_MASK) | ((type) & SIP_TYPE_MASK))
