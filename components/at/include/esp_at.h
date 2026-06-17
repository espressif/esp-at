/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file esp_at.h
 * @brief Public AT APIs provided by the esp-at project (application layer).
 *
 * These APIs are implemented in the esp-at sources, on top of the AT core
 * library, and are intended to be called by user applications and custom
 * AT commands (logging, module identity helpers, NVS wrappers, the
 * filesystem API, and self-command execution). Including this header also
 * brings in <esp_at_core.h> and the backward-compatibility aliases in
 * <esp_at_legacy.h>.
 */
#pragma once

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_at_types.h"
#include "esp_at_core.h"
#include "esp_at_cmd_register.h"
#include "nvs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 *                    Compile-time constants
 * ============================================================ */

#define ESP_AT_PORT_TX_WAIT_MS_MAX      3000    /**< Maximum time (ms) to wait for the AT port TX buffer to drain. */
#define ESP_AT_BUF_ON_STACK_SIZE         128    /**< Default maximum stack-allocated buffer size for AT command handlers. */

/* ============================================================
 *                        Utility macros
 * ============================================================ */

#define esp_at_min(x, y)    ((x) < (y) ? (x) : (y))    /**< Returns the smaller of two values. */
#define esp_at_max(x, y)    ((x) > (y) ? (x) : (y))    /**< Returns the larger of two values. */

/* ============================================================
 *                           Logging
 * ============================================================ */

/**
 * @brief NULL-safe wrapper around ESP_LOG_BUFFER_HEXDUMP.
 *
 * Outputs a hexadecimal buffer dump only when @p buffer is non-NULL.
 */
#define ESP_AT_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, level) \
    do { \
        if (buffer) { \
            ESP_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, level); \
        } \
    } while (0)

#ifndef CONFIG_AT_LOG_DEFAULT_LEVEL
/** @brief Compile-time fallback for the AT log verbosity level when not set via Kconfig. */
#define CONFIG_AT_LOG_DEFAULT_LEVEL     ESP_LOG_VERBOSE
#endif

/**
 * @brief Write a formatted message to the AT log output (weak, overridable).
 *
 * Not intended to be called directly; use the ESP_AT_LOGE/W/I/D/V macros
 * instead. Must not be called from an interrupt context. As a weak symbol,
 * the default implementation can be overridden to redirect AT log output.
 *
 * @param level   Log severity level.
 * @param tag     Log tag string.
 * @param format  printf-style format string.
 * @param ...     Arguments referenced by @p format.
 */
void esp_at_log_write(esp_log_level_t level, const char *tag, const char *format, ...)
__attribute__((format(printf, 3, 4)));

/** @brief Log at ERROR level, gated by CONFIG_AT_LOG_DEFAULT_LEVEL. */
#define ESP_AT_LOGE(tag, format, ...)   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_ERROR)   esp_at_log_write(ESP_LOG_ERROR,   tag, format, ##__VA_ARGS__); }
/** @brief Log at WARN level, gated by CONFIG_AT_LOG_DEFAULT_LEVEL. */
#define ESP_AT_LOGW(tag, format, ...)   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_WARN)    esp_at_log_write(ESP_LOG_WARN,    tag, format, ##__VA_ARGS__); }
/** @brief Log at INFO level, gated by CONFIG_AT_LOG_DEFAULT_LEVEL. */
#define ESP_AT_LOGI(tag, format, ...)   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_INFO)    esp_at_log_write(ESP_LOG_INFO,    tag, format, ##__VA_ARGS__); }
/** @brief Log at DEBUG level, gated by CONFIG_AT_LOG_DEFAULT_LEVEL. */
#define ESP_AT_LOGD(tag, format, ...)   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_DEBUG)   esp_at_log_write(ESP_LOG_DEBUG,   tag, format, ##__VA_ARGS__); }
/** @brief Log at VERBOSE level, gated by CONFIG_AT_LOG_DEFAULT_LEVEL. */
#define ESP_AT_LOGV(tag, format, ...)   { if (CONFIG_AT_LOG_DEFAULT_LEVEL >= ESP_LOG_VERBOSE) esp_at_log_write(ESP_LOG_VERBOSE, tag, format, ##__VA_ARGS__); }

/* ============================================================
 *                       Module identity
 * ============================================================ */

/**
 * @brief Return the name of the currently selected AT module.
 *
 * @return NUL-terminated module name string.
 */
const char *esp_at_get_current_module_name(void);

/* ============================================================
 *                   AT initialization hooks
 * ============================================================ */

/**
 * @brief Hook invoked immediately before the AT stack signals ready.
 *
 * Override this weak function to:
 *   a) Execute preset AT commands via esp_at_exe_cmd().
 *   b) Perform additional initialization using ESP-IDF or AT APIs.
 */
void esp_at_ready_before(void);

/* ============================================================
 *  AT self-command execution (CONFIG_AT_SELF_COMMAND_SUPPORT)
 * ============================================================ */

#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
/**
 * @brief Execute an AT command from firmware and wait for the expected response.
 *
 * Allows the firmware itself to send AT commands through the AT engine without
 * a physical or virtual host interface. Intended for executing preset commands
 * during initialization, before the AT stack is fully ready.
 *
 * @note Do not call this function from within an AT command handler
 *       (test, query, set, or execute handler). To invoke AT commands from a
 *       handler, dispatch via a separate task and queue instead.
 *
 * @param cmd               AT command string to execute.
 * @param expected_response NUL-terminated expected response substring.
 * @param timeout_ms        Timeout in milliseconds.
 * @return ESP_OK if the expected response was received within the timeout,
 *         or an error code otherwise.
 */
esp_err_t esp_at_exe_cmd(const char *cmd, const char *expected_response, uint32_t timeout_ms);
#endif

/* ============================================================
 *                         NVS helpers
 * ============================================================ */

/**
 * @brief Store a NUL-terminated string in NVS (weak, overridable).
 *
 * The AT core uses this helper internally to persist string data to NVS. It is
 * a weak symbol whose default implementation simply forwards to nvs_set_str().
 * Override it to hook the storage path, for example to encrypt the value before
 * it is written to flash (override esp_at_nvs_get_str() to decrypt on read).
 *
 * @param handle  NVS handle opened with nvs_open().
 * @param key     Key name (NUL-terminated).
 * @param value   NUL-terminated string to store.
 * @return ESP_OK on success, or an NVS error code otherwise.
 * @see nvs_set_str
 */
esp_err_t esp_at_nvs_set_str(nvs_handle_t handle, const char *key, const char *value);

/**
 * @brief Read a NUL-terminated string from NVS (weak, overridable).
 *
 * Counterpart of esp_at_nvs_set_str(): the AT core uses it internally to load
 * string data from NVS. It is a weak symbol whose default implementation simply
 * forwards to nvs_get_str(). Override it together with esp_at_nvs_set_str() to,
 * for example, decrypt the value after reading it from flash.
 *
 * @param handle     NVS handle opened with nvs_open().
 * @param key        Key name (NUL-terminated).
 * @param out_value  Buffer that receives the string; pass NULL to query the required length.
 * @param length     In/out: capacity of @p out_value in bytes, updated with the actual length.
 * @return ESP_OK on success, or an NVS error code otherwise.
 * @see nvs_get_str
 */
esp_err_t esp_at_nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length);

/**
 * @brief Store a binary blob in NVS (weak, overridable).
 *
 * The AT core uses this helper internally to persist binary data to NVS. It is
 * a weak symbol whose default implementation simply forwards to nvs_set_blob().
 * Override it to hook the storage path, for example to encrypt the blob before
 * it is written to flash (override esp_at_nvs_get_blob() to decrypt on read).
 *
 * @param handle  NVS handle opened with nvs_open().
 * @param key     Key name (NUL-terminated).
 * @param value   Pointer to the blob to store.
 * @param length  Size of @p value in bytes.
 * @return ESP_OK on success, or an NVS error code otherwise.
 * @see nvs_set_blob
 */
esp_err_t esp_at_nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length);

/**
 * @brief Read a binary blob from NVS (weak, overridable).
 *
 * Counterpart of esp_at_nvs_set_blob(): the AT core uses it internally to load
 * binary data from NVS. It is a weak symbol whose default implementation simply
 * forwards to nvs_get_blob(). Override it together with esp_at_nvs_set_blob() to,
 * for example, decrypt the blob after reading it from flash.
 *
 * @param handle     NVS handle opened with nvs_open().
 * @param key        Key name (NUL-terminated).
 * @param out_value  Buffer that receives the blob; pass NULL to query the required length.
 * @param length     In/out: capacity of @p out_value in bytes, updated with the actual length.
 * @return ESP_OK on success, or an NVS error code otherwise.
 * @see nvs_get_blob
 */
esp_err_t esp_at_nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length);

/* ============================================================
 *                       CPU yield helper
 * ============================================================ */

/**
 * @brief Yield the current task if the FreeRTOS idle task has not run recently.
 *
 * Measures the time since the last idle hook execution. If that interval
 * exceeds @p idle_timeout_ms, the calling task delays for @p yield_ticks RTOS
 * ticks to allow lower-priority tasks (including the idle task) to run.
 *
 * Intended to prevent high-throughput tasks from starving the idle task and
 * triggering the task watchdog timer under sustained CPU load.
 *
 * @note Must not be called from interrupt context.
 *
 * @param idle_timeout_ms  Threshold (ms) since last idle hook execution.
 * @param yield_ticks      RTOS ticks to delay when a yield is triggered.
 */
void esp_at_yield_if_idle_timeout(uint32_t idle_timeout_ms, uint32_t yield_ticks);

/* ============================================================
 *                        Filesystem API
 * ============================================================ */

/**
 * @brief Return the mount point of the AT filesystem partition.
 *
 * @return NUL-terminated mount point path string (e.g. "/littlefs").
 */
const char *esp_at_fs_get_mount_point(void);

/**
 * @brief Mount the AT filesystem partition.
 *
 * @return true on success, false on failure.
 */
bool esp_at_fs_mount(void);

/**
 * @brief Unmount the AT filesystem partition.
 *
 * @return true on success, false on failure.
 */
bool esp_at_fs_unmount(void);

/**
 * @brief Query the total and free capacity of the AT filesystem.
 *
 * @param out_total_bytes  Output: total filesystem size in bytes.
 * @param out_free_bytes   Output: available free space in bytes.
 * @return ESP_OK on success, or an error code.
 */
esp_err_t esp_at_get_fs_info(uint32_t *out_total_bytes, uint32_t *out_free_bytes);

/**
 * @brief Return the filesystem type in use.
 *
 * @return The active esp_at_fs_type_t value.
 */
esp_at_fs_type_t esp_at_fs_get_type(void);

#include "esp_at_legacy.h"

#ifdef __cplusplus
}
#endif
