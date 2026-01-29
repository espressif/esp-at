/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_at_types.h"
#include "esp_at_core.h"
#include "esp_at_cmd_register.h"
#include "nvs.h"

#define ESP_AT_PORT_TX_WAIT_MS_MAX          3000    // 3s
#define AT_BUFFER_ON_STACK_SIZE             128     // default maximum buffer size on task stack

#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)
#define ESP_AT_LEGACY_SUPPORT
#endif

/**
 * @brief Same to ESP_LOG_BUFFER_HEXDUMP, but only output when buffer is not NULL
*/
#define ESP_AT_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, level) \
    do { \
        if (buffer) { \
            ESP_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, level); \
        } \
    } while(0)

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

/**
 * @brief Do some things before esp-at is ready.
 *
 * @note This function can be overridden with custom implementation. For example, you can override this function to:
 *        a) execute some preset AT commands by calling at_exe_cmd() API.
 *        b) do some initializations by calling APIs from esp-idf or esp-at.
 */
void esp_at_ready_before(void);

#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
/**
 * @brief Execute AT command from self and wait for expected response.
 *
 *  AT typically communicates with the host MCU via physical interface (UART/SPI/SDIO) or virtual interface (Socket),
 *  here, we define a new API that allows users to send AT commands via esp-at self instead of a physical/virtual interface,
 *  to check the response of the AT commands. This enables users to execute certain preset AT commands before AT ready,
 *  thereby modifying the default initial configuration or status of the AT firmware.
 *
 * @param[in] cmd: AT command string
 * @param[in] expected_response: expected response string
 * @param[in] timeout_ms: timeout in milliseconds
 *
 * @note Once expected response is received, the function will return immediately.
 * @note You should not call this function directly from an AT command handler.
 *       The AT command handler typically refers to the test command, query command, set command, and execute command
 *       corresponding to the AT commands registered via esp_at_custom_cmd_array_regist().
 * @note If you want to execute an AT command in the AT command handler, The correct approach is to initialize a new queue and a new task.
 *       In the AT command handler, save the required parameters and send them to the new queue for processing, then return.
 *       The new task will fetch item from that queue, retrieve the parameters and invoke at_exe_cmd() to execute the specific AT command.
 *
 * @return
 *      - ESP_OK: the expected response is received within the timeout
 *      - others: see esp_err.h
 */
esp_err_t at_exe_cmd(const char *cmd, const char *expected_response, uint32_t timeout_ms);
#endif

#ifndef CONFIG_AT_LOG_DEFAULT_LEVEL
#define CONFIG_AT_LOG_DEFAULT_LEVEL         ESP_LOG_VERBOSE
#endif
/**
 * @brief Write message into the log
 *
 * This function is not intended to be used directly. Instead, use one of
 * ESP_AT_LOGE, ESP_AT_LOGW, ESP_AT_LOGI, ESP_AT_LOGD, ESP_AT_LOGV macros.
 *
 * This function or these macros should not be used from an interrupt.
 */
void esp_at_log_write(esp_log_level_t level, const char *tag, const char *format, ...) __attribute__((format(printf, 3, 4)));

/** runtime macro to output logs at a specified level.
 *
 * @param tag tag of the log, which can be used to change the log level by ``esp_log_level_set`` at runtime.
 * @param format format of the output log. See ``printf``
 * @param ... variables to be replaced into the log. See ``printf``
 *
 * @see ``printf``
 */
#define ESP_AT_LOGE( tag, format, ... )   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_ERROR) esp_at_log_write(ESP_LOG_ERROR, tag, format, ##__VA_ARGS__); }
#define ESP_AT_LOGW( tag, format, ... )   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_WARN) esp_at_log_write(ESP_LOG_WARN, tag, format, ##__VA_ARGS__); }
#define ESP_AT_LOGI( tag, format, ... )   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_INFO) esp_at_log_write(ESP_LOG_INFO, tag, format, ##__VA_ARGS__); }
#define ESP_AT_LOGD( tag, format, ... )   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_DEBUG) esp_at_log_write(ESP_LOG_DEBUG, tag, format, ##__VA_ARGS__); }
#define ESP_AT_LOGV( tag, format, ... )   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_VERBOSE) esp_at_log_write(ESP_LOG_VERBOSE, tag, format, ##__VA_ARGS__); }

/**
 * @brief NVS operations for string and blob
 *
 * @note These functions can be overridden with custom implementations. For example:
 *       you can override these functions to encrypt the data before writing to NVS (decrypt the data after reading from NVS).
 *
 * @see ``nvs_set_str``,``nvs_get_str``,``nvs_set_blob``,``nvs_get_blob``
 */
esp_err_t esp_at_nvs_set_str(nvs_handle_t handle, const char *key, const char *value);
esp_err_t esp_at_nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length);
esp_err_t esp_at_nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length);
esp_err_t esp_at_nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length);

/**
 * @brief Get the mount point of the filesystem
 *
 * @return pointer to the mount point string
 */
const char *esp_at_fs_get_mount_point(void);

/**
 * @brief Mount the filesystem
 *
 * @return
 *      - true: mount successfully
 *      - false: mount failed
 */
bool esp_at_fs_mount(void);

/**
 * @brief Unmount the filesystem
 *
 * @return
 *      - true: unmount successfully
 *      - false: unmount failed
 */
bool esp_at_fs_unmount(void);

/**
 * @brief Get the filesystem information
 *
 * @param[out] out_total_bytes: pointer to store total bytes of the filesystem
 * @param[out] out_free_bytes: pointer to store free bytes of the filesystem
 *
 * @return
 *      - ESP_OK: get filesystem info successfully
 *      - others: see esp_err.h
 */
esp_err_t esp_at_get_fs_info(uint32_t *out_total_bytes, uint32_t *out_free_bytes);

/**
 * @brief Get the filesystem type
 *
 * @return the filesystem type, see at_fs_type_t
 */
at_fs_type_t esp_at_fs_get_type(void);
