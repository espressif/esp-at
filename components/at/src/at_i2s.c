/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/i2s.h"

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT)
#if defined(CONFIG_AT_BT_A2DP_COMMAND_SUPPORT)
void esp_at_i2s_init(void)
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,                                  // Only TX
        .sample_rate = 44100,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .dma_buf_count = 6,
        .dma_buf_len = 60,
        .intr_alloc_flags = 0,                                                  //Default interrupt priority
        .tx_desc_auto_clear = true                                              //Auto clear tx descriptor on underflow
    };

    i2s_driver_install(0, &i2s_config, 0, NULL);

    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_AT_I2S_BCK_PIN,
        .ws_io_num = CONFIG_AT_I2S_LRCK_PIN,
        .data_out_num = CONFIG_AT_I2S_DATA_PIN,
        .data_in_num = -1                                                       //Not used
    };

    i2s_set_pin(0, &pin_config);

}
#endif
#endif
