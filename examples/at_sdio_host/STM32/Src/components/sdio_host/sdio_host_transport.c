#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "platform_os.h"
#include "sdio_host_reg.h"
#include "sdio_host_log.h"
#include "sdio_driver_port.h"
#include "sdio_host_transport.h"

static const char TAG[] = "sdio_transport";

static uint32_t tx_sent_buffers = 0;    ///< Counter hold the amount of buffers already sent to sdio slave. Should be set to 0 when initialization.
static uint32_t rx_got_bytes   = 0;       ///< Counter hold the amount of bytes already received from sdio slave. Should be set to 0 when initialization.

/******************  Init SDIO slave *********************/
static sdio_err_t esp_slave_init_io(void)
{
    sdio_err_t err;
    uint8_t ioe;
printf("esp_slave_init_io\r\n");
    err = sdio_driver_read_byte(0, SD_IO_CCCR_FN_ENABLE, &ioe);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "IOE: 0x%02x", ioe);

    uint8_t ior = 0;
    err = sdio_driver_read_byte(0, SD_IO_CCCR_FN_READY, &ior);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "IOR: 0x%02x", ior);

    // enable function 1
    ioe = 6;
    err = sdio_driver_write_byte(0, SD_IO_CCCR_FN_ENABLE, ioe, &ioe);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "IOE: 0x%02x", ioe);

    ior = 6;
    err = sdio_driver_write_byte(0, SD_IO_CCCR_FN_READY, ioe, &ior);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "IOE: 0x%02x", ior);

    // get interrupt status
    uint8_t ie;
    err = sdio_driver_read_byte(0, SD_IO_CCCR_INT_ENABLE, &ie);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "IE: 0x%02x", ie);

    // enable interrupts for function 1&2 and master enable
    ie = 7;
    err = sdio_driver_write_byte(0, SD_IO_CCCR_INT_ENABLE, ie, &ie);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "IE: 0x%02x", ie);

    uint8_t bsl, bsh;

    bsl = 0;
    err = sdio_driver_write_byte(0, SD_IO_CCCR_BLKSIZEL, bsl, &bsl);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "Function 0 BSL: 0x%02x", bsl);

    bsh = 2;
    err = sdio_driver_write_byte(0, SD_IO_CCCR_BLKSIZEH, bsh, &bsh);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "Function 0 BSH: 0x%02x", bsh);

    uint8_t func1_bsl, func1_bsh;

    func1_bsl = 0;
    err = sdio_driver_write_byte(0, 0x110, func1_bsl, &func1_bsl);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "Function 1 BSL: 0x%02x", func1_bsl);
 
    func1_bsh = 2;         // Set block size 512 (0x200)  
    err = sdio_driver_write_byte(0, 0x111, func1_bsh, &func1_bsh);
    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "Function 1 BSH: 0x%02x", func1_bsh);

    uint8_t func2_bsl, func2_bsh;

    func2_bsl = 0;
    err = sdio_driver_write_byte(0, 0x210, func2_bsl, &func2_bsl);
    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "Function 2 BSL: 0x%02x", func2_bsl);

    func2_bsh = 2;
    err = sdio_driver_write_byte(0, 0x210, func2_bsh, &func2_bsh);

    if (err != SDIO_SUCCESS) {
        return err;
    }

    SDIO_LOGD(TAG, "Function 2 BSH: 0x%02x", func2_bsh);

    return SDIO_SUCCESS;
}

//host use this to initialize the slave as well as SDIO registers
sdio_err_t sdio_host_init(void)
{
    sdio_err_t ret;
    ret = sdio_driver_init();

    if (ret != SDIO_SUCCESS) {
        SDIO_LOGE(TAG, "sdio host init error, err: %d", ret);
        return ret;
    }

    ret = esp_slave_init_io();

    if (ret != SDIO_SUCCESS) {
        SDIO_LOGE(TAG, "esp slave init error, err: %d", ret);
        return ret;
    }

    return SDIO_SUCCESS;
}

/************************* RECEIVE ****************************/
// HOST receive data
static sdio_err_t esp_sdio_slave_get_rx_data_size(uint32_t* rx_size)
{
    uint32_t len;
    sdio_err_t err = sdio_driver_read_bytes(1, ESP_SDIO_PKT_LEN, &len, 4);
    if (err != SDIO_SUCCESS) {
        return err;
    }
    len &= RX_BYTE_MASK;
    len = (len + RX_BYTE_MAX - rx_got_bytes)%RX_BYTE_MAX;
    *rx_size = len;
    return SDIO_SUCCESS;
}

sdio_err_t sdio_host_get_packet(void* out_data, size_t size, size_t* out_length, uint32_t wait_ms)
{
    sdio_err_t err = SDIO_SUCCESS;
    uint32_t len = 0;
    uint32_t wait_time = 0;

    if (size <= 0) {
        SDIO_LOGE(TAG, "Invalid size:%d", size);
        return ERR_INVALID_ARG;
    }

    for (;;) {
        err = esp_sdio_slave_get_rx_data_size(&len);
			  
			  if(len > 4096){
			      SDIO_LOGE(TAG, "Invalid size:%d", len);
					  wait_time++;
					  continue;
			  }

        if (err == SDIO_SUCCESS && len > 0) {
            break;
        }

        if (err != SDIO_SUCCESS) {
            return err;
        }

        //not error and no data, retry ``timeout_cnt`` times.
        wait_time++;

        if (wait_time >= wait_ms) {
            return ERR_TIMEOUT;
        }

        //vTaskDelay(1);
        platform_os_delay(1);
    }

    SDIO_LOGD(TAG, "get_packet: slave len=%d, max read size=%d", len, size);

    if (len > size) {
        len = size;
        err = ERR_NOT_FINISHED;
    }

    uint32_t len_remain = len;
    uint8_t* start_ptr = (uint8_t*)out_data;

    do {
        const int block_size = 512; //currently our driver don't support block size other than 512
        int len_to_send;

        int block_n = len_remain / block_size;

        if (block_n != 0) {
            len_to_send = block_n * block_size;
            err = sdio_driver_read_blocks(1, ESP_SLAVE_CMD53_END_ADDR - len_remain, start_ptr, len_to_send);
        } else {
            len_to_send = len_remain;
            /* though the driver supports to split packet of unaligned size into length
             * of 4x and 1~3, we still get aligned size of data to get higher
             * effeciency. The length is determined by the SDIO address, and the
             * remainning will be ignored by the slave hardware.
             */
            err = sdio_driver_read_bytes(1, ESP_SLAVE_CMD53_END_ADDR - len_remain, start_ptr, (len_to_send + 3) & (~3));
        }

        if (err != SDIO_SUCCESS) {
            return err;
        }

        start_ptr += len_to_send;
        len_remain -= len_to_send;
    } while (len_remain != 0);

    *out_length = len;
    rx_got_bytes += len;
    return SDIO_SUCCESS;
}

sdio_err_t sdio_host_clear_intr(uint32_t intr_mask)
{
    SDIO_LOGD(TAG, "clear_intr: %08X", intr_mask);
    return sdio_driver_write_bytes(1, ESP_SDIO_INT_CLR, (uint8_t*)&intr_mask, 4);
}

sdio_err_t sdio_host_get_intr(uint32_t* intr_raw, uint32_t* intr_st)
{
    sdio_err_t r;
    SDIO_LOGV(TAG, "get_intr");

    if (intr_raw == NULL && intr_st == NULL) {
        return ERR_INVALID_ARG;
    }

    if (intr_raw != NULL) {
        r = sdio_driver_read_bytes(1, ESP_SDIO_INT_RAW, (uint8_t*)intr_raw, 4);
        if (r != SDIO_SUCCESS) {
            return r;
        }
    }

    if (intr_st != NULL) {
        r = sdio_driver_read_bytes(1, ESP_SDIO_INT_ST, (uint8_t*)intr_st, 4);
        if (r != SDIO_SUCCESS) {
            return r;
        }
    }

    return SDIO_SUCCESS;
}

sdio_err_t sdio_host_wait_int(uint32_t wait)
{
    return sdio_driver_wait_int(wait);
}

/*********************** SEND ***************************/

static uint32_t esp_sdio_host_get_buffer_size(void)
{
    sdio_err_t ret;
    uint32_t len;
    ret = sdio_driver_read_bytes(1, ESP_SDIO_TOKEN_RDATA, &len, 4);
    if (ret != SDIO_SUCCESS) {
        SDIO_LOGE(TAG, "Read length error, ret=%d\r\n", ret);
        return 0;
    }
    SDIO_LOGD(TAG, " Read ESP32 len: %d\r\n", len);
    len = (len >> ESP_SDIO_SEND_OFFSET) & TX_BUFFER_MASK;
    len = (len + TX_BUFFER_MAX - tx_sent_buffers) % TX_BUFFER_MAX;
    return len;
}

sdio_err_t sdio_host_send_intr(uint8_t intr_no)
{
    int ret = 0;
    uint32_t intr_mask = 0;
    if (intr_no >= 8) {
        SDIO_LOGE(TAG, " Error interrupt number\r\n");
        return ERR_INVALID_ARG;
    }

    intr_mask = 0x1 << (intr_no + ESP_SDIO_CONF_OFFSET);
    ret = sdio_driver_write_byte(1, ESP_SDIO_CONF, intr_mask, NULL);

    return ret;
}

sdio_err_t sdio_host_send_packet(const void* start, size_t length)
{
    sdio_err_t err;
    uint8_t* start_ptr = (uint8_t*)start;
    uint32_t len_remain = length;
    uint32_t block_size = 512;
    int buffer_used = (length + block_size - 1) / block_size;

    uint32_t cnt = 0xffff;               // Test 0xffff times, fail will return TIMEOUT

    while (1) {
        uint32_t num = esp_sdio_host_get_buffer_size();
        SDIO_LOGD(TAG, "Buffer size %d can be send", num);

        if (num * block_size < length) {
            if (!--cnt) {
                SDIO_LOGI(TAG, "buffer is not enough: %d, %d required.", num, buffer_used);
                return ERR_TIMEOUT;
            } else {
                SDIO_LOGD(TAG, "buffer is not enough: %d, %d required. Retry...", num, buffer_used);
            }

            platform_os_delay(1);
						
        }  else {
            break;
        }
    }

    do {
        /* Though the driver supports to split packet of unaligned size into
         * length of 4x and 1~3, we still send aligned size of data to get
         * higher effeciency. The length is determined by the SDIO address, and
         * the remainning will be discard by the slave hardware.
         */
        int block_n = len_remain / block_size;
        int len_to_send;

        if (block_n) {
            len_to_send = block_n * block_size;
            err = sdio_driver_write_blocks(1, ESP_SLAVE_CMD53_END_ADDR - len_remain, start_ptr, len_to_send);
        } else {
            len_to_send = len_remain;
            err = sdio_driver_write_bytes(1, ESP_SLAVE_CMD53_END_ADDR - len_remain, start_ptr, (len_to_send + 3) & (~3));
        }

        if (err != SDIO_SUCCESS) {
            return err;
        }

        start_ptr += len_to_send;
        len_remain -= len_to_send;
    } while (len_remain);

    if (tx_sent_buffers >= TX_BUFFER_MAX) {
        tx_sent_buffers -= TX_BUFFER_MAX;
    }

    tx_sent_buffers += buffer_used;
    return SDIO_SUCCESS;
}