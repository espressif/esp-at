// Copyright 2018-2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include "sdkconfig.h"
#if defined(CONFIG_IDF_TARGET_ESP32) && defined(CONFIG_AT_BASE_ON_HSPI)
#include <hal/spi_ll.h>
#include <hal/spi_slave_hal.h>
#include <soc/lldesc.h>
#include "driver/spi_common_internal.h"
#include "driver/spi_slave.h"
#include "soc/spi_periph.h"
#include "esp_types.h"
#include "esp_attr.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_pm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/xtensa_api.h"
#include "freertos/task.h"
#include "soc/soc_memory_layout.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"

static const char* TAG = "at_spi_slave";
#define SPI_CHECK(a, str, ret_val) \
    if (!(a)) { \
        ESP_LOGE(TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val); \
    }

#define VALID_HOST(x) ((x>SPI_HOST) && (x<=VSPI_HOST))

// DMA max transmit length at one time is 4096 - 4
#define MAX_SPI_RECEIVE_SIZE 4092

//#define LOG_LOCAL_LEVEL 4     //debug mode, it will print debug log

typedef enum {
    SPI_STATE_IDLE = 0,
    SPI_READ_STATUS,
    SPI_READ_DATA,
    SPI_WRITE_STATUS,
    SPI_WRITE_DATA
} spi_state_t;

typedef struct {
    int id;
    spi_slave_interface_config_t cfg;
    intr_handle_t intr;
    spi_dev_t* hw;
    lldesc_t* dmadesc_tx;
    lldesc_t* dmadesc_rx;
    uint32_t flags;
    int max_transfer_sz;
    QueueHandle_t recv_queue;
    QueueHandle_t send_queue;
    spi_state_t cur_state;
    void* rx_buffer;
    void* tx_buffer;
    int dma_chan;
#ifdef CONFIG_PM_ENABLE
    esp_pm_lock_handle_t pm_lock;
#endif
} spi_slave_t;

static spi_slave_t* spihost[3];

static void IRAM_ATTR at_spi_intr(void* arg);

static inline bool bus_is_iomux(spi_slave_t* host)
{
    return host->flags & SPICOMMON_BUSFLAG_NATIVE_PINS;
}

esp_err_t at_spi_slave_initialize(spi_host_device_t host, const spi_bus_config_t* bus_config, const spi_slave_interface_config_t* slave_config, int dma_chan)
{
    bool spi_chan_claimed, dma_chan_claimed;
    esp_err_t ret = ESP_OK;
    //We only support HSPI/VSPI, period.
    SPI_CHECK(VALID_HOST(host), "invalid host", ESP_ERR_INVALID_ARG);
    SPI_CHECK(dma_chan > 0 && dma_chan <= 2, "invalid dma channel", ESP_ERR_INVALID_ARG);
    SPI_CHECK((bus_config->intr_flags & (ESP_INTR_FLAG_HIGH | ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_INTRDISABLED)) == 0, "intr flag not allowed", ESP_ERR_INVALID_ARG);

    spi_chan_claimed = spicommon_periph_claim(host, "spi slave");
    SPI_CHECK(spi_chan_claimed, "host already in use", ESP_ERR_INVALID_STATE);

    if (dma_chan != 0) {
        dma_chan_claimed = spicommon_dma_chan_claim(dma_chan);

        if (!dma_chan_claimed) {
            spicommon_periph_free(host);
            SPI_CHECK(dma_chan_claimed, "dma channel already in use", ESP_ERR_INVALID_STATE);
        }
    }

    spihost[host] = malloc(sizeof(spi_slave_t));

    if (spihost[host] == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto cleanup;
    }

    memset(spihost[host], 0, sizeof(spi_slave_t));
    memcpy(&spihost[host]->cfg, slave_config, sizeof(spi_slave_interface_config_t));
    spihost[host]->id = host;

    ret = spicommon_bus_initialize_io(host, bus_config, dma_chan, SPICOMMON_BUSFLAG_SLAVE | bus_config->flags, &spihost[host]->flags);

    if (ret != ESP_OK) {
        goto cleanup;
    }

    spicommon_cs_initialize(host, slave_config->spics_io_num, 0, !bus_is_iomux(spihost[host]));

    spihost[host]->dma_chan = dma_chan;

    if (dma_chan != 0) {
        //See how many dma descriptors we need and allocate them
        int dma_desc_ct = (bus_config->max_transfer_sz + SPI_MAX_DMA_LEN - 1) / SPI_MAX_DMA_LEN;

        if (dma_desc_ct == 0) {
            dma_desc_ct = 1;    //default to 4k when max is not given
        }

        spihost[host]->max_transfer_sz = dma_desc_ct * SPI_MAX_DMA_LEN;

        // malloc
        spihost[host]->dmadesc_tx = heap_caps_malloc(sizeof(lldesc_t) * dma_desc_ct, MALLOC_CAP_DMA);
        spihost[host]->dmadesc_rx = heap_caps_malloc(sizeof(lldesc_t) * dma_desc_ct, MALLOC_CAP_DMA);

        if (!spihost[host]->dmadesc_tx || !spihost[host]->dmadesc_rx) {
            ret = ESP_ERR_NO_MEM;
            goto cleanup;
        }
    }

#ifdef CONFIG_PM_ENABLE
    ret = esp_pm_lock_create(ESP_PM_APB_FREQ_MAX, 0, "spi_slave",
                             &spihost[host]->pm_lock);
    if (ret != ESP_OK) {
        goto cleanup;
    }

    // Lock APB frequency while SPI slave driver is in use
    esp_pm_lock_acquire(spihost[host]->pm_lock);
#endif //CONFIG_PM_ENABLE

    spihost[host]->recv_queue = xSemaphoreCreateBinary();
    spihost[host]->send_queue = xSemaphoreCreateBinary();

    if (!spihost[host]->recv_queue || !spihost[host]->send_queue) {
        ret = ESP_ERR_NO_MEM;
        goto cleanup;
    }

    // memory will always be used
    spihost[host]->rx_buffer = (uint8_t*) heap_caps_malloc(MAX_SPI_RECEIVE_SIZE, MALLOC_CAP_DMA);

    if (!spihost[host]->rx_buffer) {
        ret = ESP_ERR_NO_MEM;
        goto cleanup;
    }

    int flags = bus_config->intr_flags | ESP_INTR_FLAG_INTRDISABLED;
    ret = esp_intr_alloc(spicommon_irqsource_for_host(host), flags, at_spi_intr, (void*)spihost[host], &spihost[host]->intr);

    if (ret != ESP_OK) {
        goto cleanup;
    }

    spihost[host]->hw = spi_periph_signal[host].hw;

    //Configure slave
    spihost[host]->hw->clock.val = 0;
    spihost[host]->hw->user.val = 0;
    spihost[host]->hw->ctrl.val = 0;

    spihost[host]->hw->user.usr_command = 1;
    spihost[host]->hw->user.usr_addr = 1;
    spihost[host]->hw->user.usr_dummy = 0;
    spihost[host]->hw->user.usr_miso = 1;
    spihost[host]->hw->user.usr_mosi = 1;
    spihost[host]->hw->user.doutdin = 0;
    //spihost[host]->hw->user.rd_byte_order = 1;
    //spihost[host]->hw->user.wr_byte_order = 1;

    spihost[host]->hw->user1.usr_addr_bitlen = 0;
    spihost[host]->hw->user2.usr_command_bitlen = 7;
    spihost[host]->hw->user1.usr_dummy_cyclelen = 0;


    spihost[host]->hw->dma_conf.val |= SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST;
    spihost[host]->hw->dma_out_link.start = 0;
    spihost[host]->hw->dma_in_link.start = 0;
    spihost[host]->hw->dma_conf.val &= ~(SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);
    spihost[host]->hw->dma_conf.out_data_burst_en = 1;

    bool nodelay = true;
    spihost[host]->hw->ctrl.rd_bit_order = (slave_config->flags & SPI_SLAVE_RXBIT_LSBFIRST) ? 1 : 0;
    spihost[host]->hw->ctrl.wr_bit_order = (slave_config->flags & SPI_SLAVE_TXBIT_LSBFIRST) ? 1 : 0;

    if (slave_config->mode == 0) {
        spihost[host]->hw->pin.ck_idle_edge = 0;
        spihost[host]->hw->user.ck_i_edge = 1;
        spihost[host]->hw->ctrl2.miso_delay_mode = nodelay ? 0 : 2;
    } else if (slave_config->mode == 1) {
        spihost[host]->hw->pin.ck_idle_edge = 0;
        spihost[host]->hw->user.ck_i_edge = 0;
        spihost[host]->hw->ctrl2.miso_delay_mode = nodelay ? 0 : 1;
    } else if (slave_config->mode == 2) {
        spihost[host]->hw->pin.ck_idle_edge = 1;
        spihost[host]->hw->user.ck_i_edge = 0;
        spihost[host]->hw->ctrl2.miso_delay_mode = nodelay ? 0 : 1;
    } else if (slave_config->mode == 3) {
        spihost[host]->hw->pin.ck_idle_edge = 1;
        spihost[host]->hw->user.ck_i_edge = 1;
        spihost[host]->hw->ctrl2.miso_delay_mode = nodelay ? 0 : 2;
    }

    //Reset DMA
    spihost[host]->hw->dma_conf.val |= SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST;
    spihost[host]->hw->dma_out_link.start = 0;
    spihost[host]->hw->dma_in_link.start = 0;
    spihost[host]->hw->dma_conf.val &= ~(SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);

    //Enable intr

    spihost[host]->hw->slave.slave_mode = 1;
    spihost[host]->hw->slave.wr_rd_buf_en = 1;
    spihost[host]->hw->slave.wr_rd_sta_en = 1;
    //spihost[host]->hw->slave.wr_rd_sta_en = 0;


    spihost[host]->hw->slave.rd_buf_inten = 1;
    spihost[host]->hw->slave.wr_buf_inten = 1;
    spihost[host]->hw->slave.rd_sta_inten = 1;
    //spihost[host]->hw->slave.rd_sta_inten = 0;
    spihost[host]->hw->slave.wr_sta_inten = 1;

    spihost[host]->hw->slave.trans_inten = 0;

    spihost[host]->hw->slave1.status_bitlen = 31;
    spihost[host]->hw->slave1.rdbuf_dummy_en = 0;
    spihost[host]->hw->slave1.rdsta_dummy_en = 0;
    spihost[host]->hw->slave1.wrbuf_dummy_en = 0;
    spihost[host]->hw->slave1.wrsta_dummy_en = 0;
    spihost[host]->hw->slave1.wr_addr_bitlen = 7;
    spihost[host]->hw->slave1.rd_addr_bitlen = 7;

    // if 1 will read WR_STATUS
    spihost[host]->hw->slave1.status_readback = 0;

    spihost[host]->hw->cmd.usr = 0;
    spihost[host]->hw->slave.sync_reset = 1;
    spihost[host]->hw->slave.sync_reset = 0;

    spihost[host]->hw->cmd.usr = 1;

    spihost[host]->cur_state = SPI_STATE_IDLE;

    esp_intr_enable(spihost[host]->intr);

    return ESP_OK;

cleanup:

    if (spihost[host]) {
        free(spihost[host]->dmadesc_tx);
        free(spihost[host]->dmadesc_rx);
#ifdef CONFIG_PM_ENABLE
        if (spihost[host]->pm_lock) {
            esp_pm_lock_release(spihost[host]->pm_lock);
            esp_pm_lock_delete(spihost[host]->pm_lock);
        }
#endif
    }

    if (spihost[host]->rx_buffer) {
        free(spihost[host]->rx_buffer);
    }

    free(spihost[host]);
    spihost[host] = NULL;
    spicommon_periph_free(host);
    spicommon_dma_chan_free(dma_chan);
    return ret;
}

esp_err_t at_spi_slave_free(spi_host_device_t host)
{
    SPI_CHECK(VALID_HOST(host), "invalid host", ESP_ERR_INVALID_ARG);
    SPI_CHECK(spihost[host], "host not slave", ESP_ERR_INVALID_ARG);

    if (spihost[host]->dma_chan > 0) {
        spicommon_dma_chan_free(spihost[host]->dma_chan);
    }

    if (spihost[host]->rx_buffer) {
        free(spihost[host]->rx_buffer);
    }

    free(spihost[host]->dmadesc_tx);
    free(spihost[host]->dmadesc_rx);
    esp_intr_free(spihost[host]->intr);
#ifdef CONFIG_PM_ENABLE
    esp_pm_lock_release(spihost[host]->pm_lock);
    esp_pm_lock_delete(spihost[host]->pm_lock);
#endif //CONFIG_PM_ENABLE
    free(spihost[host]);
    spihost[host] = NULL;
    spicommon_periph_free(host);
    return ESP_OK;
}

static void IRAM_ATTR spi_load_trans_buffer(spi_slave_t* host, uint8_t* trans_data, uint32_t trans_len, bool isrx)
{
    spicommon_dmaworkaround_transfer_active(host->dma_chan);
    host->hw->slv_rd_bit.slv_rdata_bit = 0;
    host->hw->dma_conf.val |= SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST;
    host->hw->dma_out_link.start = 0;
    host->hw->dma_in_link.start = 0;
    host->hw->dma_conf.val &= ~(SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);
    host->hw->dma_conf.out_data_burst_en = 0;
    host->hw->dma_conf.indscr_burst_en = 0;
    host->hw->dma_conf.outdscr_burst_en = 0;

    host->hw->slv_wrbuf_dlen.bit_len = trans_len * 8 - 1;
    host->hw->slv_rdbuf_dlen.bit_len = trans_len * 8 - 1;
    host->hw->mosi_dlen.usr_mosi_dbitlen = trans_len * 8 - 1;
    host->hw->miso_dlen.usr_miso_dbitlen = trans_len * 8 - 1;

    //Fill DMA descriptors
    if (isrx) {
        host->hw->user.usr_miso_highpart = 0;
        lldesc_setup_link(host->dmadesc_rx, host->rx_buffer, trans_len, true);
        host->hw->dma_in_link.addr = (int)(&host->dmadesc_rx[0]) & 0xFFFFF;

        host->hw->dma_in_link.start = 1;
    } else {
        lldesc_setup_link(host->dmadesc_tx, trans_data, trans_len, false);
        host->hw->user.usr_mosi_highpart = 0;
        host->hw->dma_out_link.addr = (int)(&host->dmadesc_tx[0]) & 0xFFFFF;
        host->hw->dma_out_link.start = 1;
    }

    host->hw->slave.sync_reset = 1;
    host->hw->slave.sync_reset = 0;
}

esp_err_t IRAM_ATTR at_spi_slave_receive(spi_host_device_t host, uint8_t** recv_data, uint32_t* recv_len, TickType_t ticks_to_wait)
{
    BaseType_t r;
    SPI_CHECK(VALID_HOST(host), "invalid host", ESP_ERR_INVALID_ARG);
    SPI_CHECK(spihost[host], "host not slave", ESP_ERR_INVALID_ARG);

    // wait MCU send length
    r = xSemaphoreTake(spihost[host]->recv_queue, ticks_to_wait);

    if (!r) {
        return ESP_ERR_TIMEOUT;
    }

    if (spihost[host]->cur_state != SPI_WRITE_DATA) {
        ESP_LOGE(TAG, "Line: %d, Status error, %d\r\n", __LINE__, spihost[host]->cur_state);
        return ESP_FAIL;
    }

    *recv_len = spihost[host]->hw->slv_wr_status;
    *recv_data = spihost[host]->rx_buffer;

    return ESP_OK;
}

esp_err_t IRAM_ATTR at_spi_slave_free_receive_buffer(spi_host_device_t host)
{
    SPI_CHECK(spihost[host]->cur_state == SPI_WRITE_DATA, "SPI status error", ESP_ERR_INVALID_STATE);
    spihost[host]->hw->slv_wr_status = 0;

    memset(spihost[host]->rx_buffer, 0x0, MAX_SPI_RECEIVE_SIZE);
    spihost[host]->cur_state = SPI_STATE_IDLE;

    // notify MASTER can send again
    if (spihost[host]->cfg.post_setup_cb) {
        spihost[host]->cfg.post_setup_cb(NULL);
    }

    return ESP_OK;
}

esp_err_t IRAM_ATTR at_spi_slave_send(spi_host_device_t host, void* send_data, uint32_t send_len, TickType_t ticks_to_wait)
{
    BaseType_t r;
    SPI_CHECK(send_data == NULL || esp_ptr_dma_capable(send_data),
              "txdata not in DMA-capable memory", ESP_ERR_INVALID_ARG);
    SPI_CHECK(send_len > 0 && send_len < 4096, "Send length error", ESP_ERR_INVALID_ARG);

    //SPI_CHECK(spihost[host]->cur_state == SPI_STATE_IDLE, "SPI status error", ESP_ERR_INVALID_STATE);
    if (spihost[host]->cur_state != SPI_STATE_IDLE) {
        printf("send status error %d\r\n", spihost[host]->cur_state);
    }

    if (spihost[host]->cfg.post_trans_cb) {
        spihost[host]->cfg.post_trans_cb(NULL);
    }

    spihost[host]->hw->rd_status.status = send_len;
    spihost[host]->tx_buffer = send_data;

    // notify MASTER can receive
    if (spihost[host]->cfg.post_setup_cb) {
        spihost[host]->cfg.post_setup_cb(NULL);
    }

    r = xSemaphoreTake(spihost[host]->send_queue, ticks_to_wait);

    if (!r) {
        return ESP_ERR_TIMEOUT;
    }

    spihost[host]->hw->rd_status.status = 0;
    spihost[host]->cur_state = SPI_STATE_IDLE;

    return ESP_OK;
}

//This is run in interrupt context and apart from initialization and destruction, this is the only code
//touching the host (=spihost[x]) variable. The rest of the data arrives in queues. That is why there are
//no muxes in this code.
static void IRAM_ATTR at_spi_intr(void* arg)
{
    BaseType_t do_yield = pdFALSE;
    spi_slave_t* host = (spi_slave_t*)arg;
    uint32_t trans_len = 0;
    uint32_t cnt = 0;
    ESP_EARLY_LOGV(TAG, "intr: %x", host->hw->slave);

    if (host->hw->slave.wr_sta_done) {
        if (host->cfg.post_trans_cb) {
            host->cfg.post_trans_cb(NULL);
        }

        host->hw->slave.wr_sta_done = 0;

        if (host->cur_state != SPI_STATE_IDLE) {
            ESP_EARLY_LOGE(TAG, "WR_STA error status %d", host->cur_state);
            return;
        }

        ESP_EARLY_LOGD(TAG, "WR_DONE, len: %d", host->hw->slv_wr_status);
        trans_len = host->hw->slv_wr_status;

        if (trans_len > MAX_SPI_RECEIVE_SIZE || trans_len == 0) {
            ets_printf("recv length error: %d\r\n", trans_len);
            return;
        }

        spi_load_trans_buffer(host, host->rx_buffer, trans_len, true);
        host->cur_state = SPI_WRITE_STATUS;
    }

    if (host->hw->slave.rd_sta_done) {
        if (host->cfg.post_trans_cb) {
            host->cfg.post_trans_cb(NULL);
        }

        host->hw->slave.rd_sta_done = 0;

        if (host->cur_state != SPI_STATE_IDLE) {
            ESP_EARLY_LOGE(TAG, "RD_STA error status %d", host->cur_state);
            return;
        }

        if (host->hw->rd_status.status == 0) {
            ESP_EARLY_LOGE(TAG, "RD_STA len error");
            return;
        }

        spi_load_trans_buffer(host, host->tx_buffer, host->hw->rd_status.status, false);
        ESP_EARLY_LOGD(TAG, "RD_DONE, len: %d", host->hw->rd_status.status);
        host->cur_state = SPI_READ_STATUS;
    }

    if (host->hw->slave.wr_buf_done) {
        host->hw->slave.wr_buf_done = 0;

        if (host->cur_state != SPI_WRITE_STATUS) {
            ESP_EARLY_LOGE(TAG, "WR_BUF error status %d\r\n", host->cur_state);
            return;
        }

        host->cur_state = SPI_WRITE_DATA;
        ESP_EARLY_LOGD(TAG, "WR_BUF_DONE");

        // tell muc need wait
        if (host->cfg.post_trans_cb) {
            host->cfg.post_trans_cb(NULL);
        }

        //Okay, transaction is done.
        //Return transaction descriptor.
        xSemaphoreGiveFromISR(host->recv_queue, &do_yield);
    }

    if (host->hw->slave.rd_buf_done) {
        if (host->cur_state != SPI_READ_STATUS) {
            ESP_EARLY_LOGE(TAG, "RD_BUF error status %d", host->cur_state);
            return;
        }

        host->hw->slave.rd_buf_done = 0;
        host->cur_state = SPI_READ_DATA;
        ESP_EARLY_LOGD(TAG, "RD_BUF_DONE");

        xSemaphoreGiveFromISR(host->send_queue, &do_yield);
    }

    // After the software cleanup interrupt, the hardware may still be uncleaned, at which point you need to wait
    while (host->hw->slave.val & 0xF) {
        host->hw->slave.val &= ~0xF;
        cnt++;
        // 500 is enough for the hardware to cleanup interrupt
        if (cnt >= 500) {
            ESP_EARLY_LOGE(TAG, "intr err: %x", host->hw->slave);
            cnt = 0;
        }
    }

    if (host->cur_state == SPI_READ_STATUS || host->cur_state == SPI_WRITE_STATUS) {
        // handshake pull high
        if (host->cfg.post_setup_cb) {
            host->cfg.post_setup_cb(NULL);
        }
    }

    host->hw->cmd.usr = 1;

    if (do_yield) {
        portYIELD_FROM_ISR();
    }
}
#endif