/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file esp_at_internal.h
 * @brief AT APIs shared between esp-at internal components.
 *
 * These declarations are used internally across the esp-at framework (init
 * code, the communication interfaces, and the bundled components). They are
 * not part of the stable public API and are not recommended for application
 * use. Including this header is technically allowed, but do so with caution:
 * the symbols and signatures declared here may change without notice. Prefer
 * <esp_at.h> / <esp_at_core.h> for application code.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "esp_at_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 *                        Platform support
 * ============================================================ */
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || \
    defined(CONFIG_IDF_TARGET_ESP32C2) || defined(CONFIG_IDF_TARGET_ESP32C3) || \
    defined(CONFIG_IDF_TARGET_ESP32C6)
#define ESP_AT_LEGACY_SUPPORT                   /**< Defined when the target uses the legacy AT feature set. */
#endif

/* ============================================================================
 *                    Raw transport function-pointer types
 * ========================================================================== */

typedef int32_t (*at_read_data_fn_t)(uint8_t *data, int32_t len);       /**< Reads raw bytes from the AT port. */
typedef int32_t (*at_write_data_fn_t)(uint8_t *data, int32_t len);      /**< Writes raw bytes to the AT port. */
typedef int32_t (*at_get_data_len_fn_t)(void);                          /**< Returns the number of pending bytes on the AT port. */

/* ============================================================================
 *                    Core engine bring-up and versioning
 * ========================================================================== */

/**
 * @brief Apply platform-specific workarounds required at AT startup.
 *
 * Called by the AT framework during initialization. External repositories
 * may override this with a weak-linked implementation.
 */
void esp_at_main_preprocess(void);

/**
 * @brief Initialize the AT core engine. Must be called once during startup.
 *
 * @param custom_version  Version string appended to the AT version response,
 *                        or NULL for no custom suffix.
 */
void esp_at_module_init(const uint8_t *custom_version);

/**
 * @brief Return the AT firmware version as a packed 32-bit word.
 *
 * @return bit31~24: main version, bit23~16: sub version,
 *         bit15~8: test version, bit7~0: custom version.
 */
uint32_t esp_at_get_version(void);

/**
 * @brief Copy the AT core library version string into a caller-supplied buffer.
 *
 * @param buffer  Destination buffer.
 * @param size    Buffer size in bytes.
 * @return Number of bytes written, excluding the NUL terminator.
 */
int32_t esp_at_get_core_version(char *buffer, uint32_t size);

/* ============================================================================
 *                              Module identity
 * ========================================================================== */

/**
 * @brief Return the currently selected module ID.
 */
uint32_t esp_at_get_module_id(void);

/**
 * @brief Set the active module ID.
 *
 * @param id  Module identifier to activate.
 */
void esp_at_set_module_id(uint32_t id);

/**
 * @brief Set the active module by name string.
 *
 * @param buffer  NUL-terminated module name string.
 */
void esp_at_set_module_id_by_str(const char *buffer);

/* ============================================================================
 *                          Command-line terminator
 * ========================================================================== */

/**
 * @brief Override the command-line terminator (default: CR+LF).
 *
 * @param terminator  NUL-terminated byte sequence to use as the line terminator.
 * @return true on success, false if the terminator is invalid.
 */
bool esp_at_custom_cmd_line_terminator_set(uint8_t *terminator);

/**
 * @brief Return the current command-line terminator string.
 */
uint8_t *esp_at_custom_cmd_line_terminator_get(void);

/* ============================================================================
 *                          Transparent transmission
 * ========================================================================== */

/**
 * @brief Terminate the active passthrough-transmission session (task context).
 */
void esp_at_transmit_terminate(void) __asm__("esp_at_transmit_terminal");

/* ============================================================================
 *                          Manufacturing parameters
 * ========================================================================== */

/**
 * @brief Storage backend for manufacturing (calibration) parameters.
 */
typedef enum {
    AT_PARAMS_NONE          = 0,    /**< No manufacturing parameters storage     */
    AT_PARAMS_IN_MFG_NVS    = 1,    /**< Parameters stored in the mfg NVS        */
    AT_PARAMS_IN_PARTITION  = 2,    /**< Parameters stored in a custom partition */
} at_mfg_params_storage_mode_t;

/**
 * @brief Return the storage backend used for manufacturing parameters.
 */
at_mfg_params_storage_mode_t at_get_mfg_params_storage_mode(void);

/* ============================================================================
 *        MCU wake-up interface (CONFIG_AT_USERWKMCU_COMMAND_SUPPORT)
 * ========================================================================== */

#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
/**
 * @brief Configure the MCU wake-up data-write callback.
 *
 * @param write_data_fn  Callback used to write wake-up data.
 */
void at_wkmcu_if_config(at_write_data_fn_t write_data_fn);

/**
 * @brief Notify the MCU wake-up module of the current sleep mode.
 *
 * @param mode  New sleep mode.
 */
void at_set_mcu_state_if_sleep(esp_at_sleep_mode_t mode);
#endif

#ifdef __cplusplus
}
#endif
