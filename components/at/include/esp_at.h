/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "sdkconfig.h"
#include "esp_at_core.h"
#include "esp_at_cmd_register.h"

#define ESP_AT_PORT_TX_WAIT_MS_MAX          3000    // 3s
#define AT_BUFFER_ON_STACK_SIZE             128     // default maximum buffer size on task stack

typedef enum {
    AT_PARAMS_NONE = 0,
    AT_PARAMS_IN_MFG_NVS = 1,
    AT_PARAMS_IN_PARTITION = 2,
} at_mfg_params_storage_mode_t;

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
 */
uint32_t esp_at_get_module_id(void);

/**
 * @brief Set current module id
 *
 * @param[in] id: the module id to set
*/
void esp_at_set_module_id(uint32_t id);

/**
 * @brief Get module id by module name
 *
 * @param[in] buffer: pointer to a module name string
*/
void esp_at_set_module_id_by_str(const char *buffer);

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

/**
 * @brief get storage mode of mfg parameters
 *
 * @return at_mfg_params_storage_mode_t
 */
at_mfg_params_storage_mode_t at_get_mfg_params_storage_mode(void);
