/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __AT_UPGRADE_H__
#define __AT_UPGRADE_H__

typedef enum {
    AT_UPGRADE_SYSTEM_FIRMWARE = 0,         /**< upgrade type is system firmware */
    AT_UPGRADE_CUSTOM_PARTITION,            /**< upgrade type is custom partition */
} at_upgrade_type_t;

typedef enum {
    ESP_AT_OTA_MODE_NORMAL = 0,             /**< upgrade mode is TCP */
    ESP_AT_OTA_MODE_SSL,                    /**< upgrade mode is TLS */
    ESP_AT_OTA_MODE_MAX,
} esp_at_ota_mode_type;

typedef enum {
    ESP_AT_OTA_STATE_IDLE = 0,              /**< not in upgrade */
    ESP_AT_OTA_STATE_FOUND_SERVER,          /**< the upgrade server found */
    ESP_AT_OTA_STATE_CONNECTED_TO_SERVER,      /**< connected to the server */
    ESP_AT_OTA_STATE_GOT_VERSION,           /**< got the version information to be upgraded */
    ESP_AT_OTA_STATE_DONE,                  /**< upgrade succeeded */
    ESP_AT_OTA_STATE_FAILED = -1,           /**< upgrade failed */
} esp_at_ota_state_t;

/**
 * @brief      upgrade process
 *
 * @param[in]  ota_mode  use this mode to upgrade
 * @param[in]  version  this version to upgrade
 * @param[in]  partition_name  this partition to upgrade
 * 
 * @return
 *    - true
 *    - false
 */
bool esp_at_upgrade_process(esp_at_ota_mode_type ota_mode, uint8_t *version, const char *partition_name);

/**
 * @brief      Set upgrade status
 *
 * @param[in]  status  upgrade progress
 * 
 * @noreturn 
 */
void esp_at_set_upgrade_state(esp_at_ota_state_t status);

/**
 * @brief      Get upgrade status
 *
 * @return
 *    - esp_at_ota_state_t
 */
esp_at_ota_state_t esp_at_get_upgrade_state(void);

#endif
