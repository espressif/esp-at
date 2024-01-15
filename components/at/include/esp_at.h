/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_at_core.h"

#define ESP_AT_PORT_TX_WAIT_MS_MAX          3000    // 3s
#define AT_BUFFER_ON_STACK_SIZE             128     // default maximum buffer size on task stack

typedef enum {
    AT_PARAMS_NONE = 0,
    AT_PARAMS_IN_MFG_NVS = 1,
    AT_PARAMS_IN_PARTITION = 2,
} at_mfg_params_storage_mode_t;

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

#ifdef CONFIG_AT_RAINMAKER_COMMAND_SUPPORT
/**
 * @brief regist at rainmaker command set. If not,you can not use AT rainmaker command
 *
 */
bool esp_at_rainmaker_cmd_regist(void);
#endif

/**
 * @brief init storage mode of at parameters
 */
void at_nvs_flash_init_partition(void);

/**
 * @brief get storage mode of mfg parameters
 *
 * @return at_mfg_params_storage_mode_t
 */
at_mfg_params_storage_mode_t at_get_mfg_params_storage_mode(void);
