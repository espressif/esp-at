/*
 * SPDX-FileCopyrightText: 2015-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"

#include "sdmmc_cmd.h"
#include "soc/host_reg.h"
#include "soc/sdmmc_periph.h"

#include "sdio_host_log.h"
#include "sdio_host_error.h"

sdmmc_card_t *card = NULL;

static const char TAG[] = "sdio_driver";

sdio_err_t sdio_driver_init(void)
{
    /* Probe */
    sdmmc_host_t config = SDMMC_HOST_DEFAULT();
    config.flags = SDMMC_HOST_FLAG_1BIT;
    config.max_freq_khz = SDMMC_FREQ_DEFAULT;
    //config.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    //config.max_freq_khz = SDMMC_FREQ_PROBING;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    printf("SDIO clock: %d, bit: %lu\r\n", config.max_freq_khz, config.flags);
    sdmmc_host_init();
    sdmmc_host_init_slot(SDMMC_HOST_SLOT_1, &slot_config);
    card = (sdmmc_card_t *)malloc(sizeof(sdmmc_card_t));
    if (card == NULL) {
        return ERR_NO_MEMORY;
    }
    for (;;) {
        if (sdmmc_card_init(&config, card) == ESP_OK) {
            break;
        }
        SDIO_LOGW(TAG, "slave init failed, retry...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    sdmmc_card_print_info(stdout, card);

    return SUCCESS;
}

sdio_err_t sdio_driver_write_blocks(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{
    esp_err_t err;
    err = sdmmc_io_write_blocks(card, function, addr, buffer, len);
    if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Write blocks error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}

sdio_err_t sdio_driver_read_blocks(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{
    esp_err_t err;
    err = sdmmc_io_read_blocks(card, function, addr, buffer, len);
    if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Read blocks error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}

sdio_err_t sdio_driver_read_bytes(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{
    esp_err_t err;
    err = sdmmc_io_read_bytes(card, function, addr, buffer, len);
    if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Read bytes error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}

sdio_err_t sdio_driver_write_bytes(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{
    esp_err_t err;
    err = sdmmc_io_write_bytes(card, function, addr, buffer, len);
    if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Write bytes error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}

sdio_err_t sdio_driver_read_byte(uint32_t function, uint32_t reg, uint8_t *out_byte)
{
    esp_err_t err;
    err = sdmmc_io_read_byte(card, function, reg, out_byte);
    if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Read byte error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}

sdio_err_t sdio_driver_write_byte(uint32_t function, uint32_t reg, uint8_t in_byte, uint8_t* out_byte)
{
    esp_err_t err;
    err = sdmmc_io_write_byte(card, function, reg, in_byte, out_byte);
    if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Write byte error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}

sdio_err_t sdio_driver_wait_int(uint32_t timeout)
{
    esp_err_t err = sdmmc_io_wait_int(card, timeout);
    if (err == ESP_ERR_TIMEOUT) {
        return ERR_TIMEOUT;
    } else if (err != ESP_OK) {
        SDIO_LOGE(TAG, "Wait interrupt error,ret:%d", err);
        return FAILURE;
    }
    return SUCCESS;
}
