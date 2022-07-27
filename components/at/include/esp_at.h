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

#ifndef __ESP_AT_H__
#define __ESP_AT_H__

#include "esp_at_core.h"

#define ESP_AT_PORT_TX_WAIT_MS_MAX          3000    // 3s
#define ESP_AT_FACTORY_PARAMETER_SIZE       4096

#ifdef CONFIG_AT_OTA_SUPPORT
/**
 * @brief regist at ota command set. If not,you can not use AT OTA command
 *
 */
bool esp_at_ota_cmd_regist(void);
#endif

#ifdef CONFIG_AT_SIGNALING_COMMAND_SUPPORT
/**
 * @brief regist at fact command set. If not,you can not use signaling command
 *
 */
bool esp_at_fact_cmd_regist(void);
#endif

/**
 * @brief get current module name
 *
 */
const char* esp_at_get_current_module_name(void);

/**
 * @brief get module name by index
 *
 */
const char* esp_at_get_module_name_by_id(uint32_t id);

/**
 * @brief get current module id
 *
 */
uint32_t esp_at_get_module_id(void);

/**
 * @brief init peripheral and default parameters in factory_param.bin
 *
 */
void esp_at_board_init(void);

/**
* @brief regist WiFi config via web command. If not,you can not use web server to config wifi connect
*
*/
bool esp_at_web_server_cmd_regist(void);

#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
/**
 * @brief wake up MCU if AT is ready to send data to MCU
 *
 */
void at_wkmcu_if_config(at_write_data_fn_t write_data_fn);

/**
 * @brief set MCU awake state according to AT+SLEEP command
 *
 */
void at_set_mcu_state_if_sleep(at_sleep_mode_t mode);
#endif

/**
 * @brief some workarounds for esp-at project
 *
 */
void esp_at_main_preprocess(void);
#endif
