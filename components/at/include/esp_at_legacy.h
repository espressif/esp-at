/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file esp_at_legacy.h
 * @brief Backward-compatibility declarations for previous-version AT APIs.
 *
 * Provides deprecated aliases for identifiers that were public before the
 * esp_at_* naming cleanup, so that existing code keeps compiling. Each use of
 * a legacy name still works but emits a deprecation warning; new code should
 * use the esp_at_* names declared in <esp_at.h> / <esp_at_core.h>. Newly
 * exposed APIs (e.g. esp_at_connect(), esp_at_restart(), the HTTP/WebSocket
 * helpers) had no previous public name, so they are intentionally absent.
 *
 * For APIs whose underlying symbol is still the legacy at_* name (provided by
 * the AT core library), the alias is a plain deprecated declaration that
 * resolves to that same symbol. For APIs implemented in esp-at sources and
 * renamed to esp_at_*, the alias is a thin deprecated inline forwarder.
 *
 * This file is included automatically at the end of <esp_at.h>; you do not
 * need to include it directly.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 *        Deprecated names originally from esp_at_core.h
 * ============================================================ */

/* --- Utility macros --- */
#define at_min(x, y)    esp_at_min(x, y)    /* deprecated: use esp_at_min */
#define at_max(x, y)    esp_at_max(x, y)    /* deprecated: use esp_at_max */

/* --- Struct typedefs --- */
typedef esp_at_cmd_t        esp_at_cmd_struct            __attribute__((deprecated("Use esp_at_cmd_t")));
typedef esp_at_intf_ops_t esp_at_device_ops_struct     __attribute__((deprecated("Use esp_at_intf_ops_t")));
typedef esp_at_net_ops_t    esp_at_custom_net_ops_struct __attribute__((deprecated("Use esp_at_net_ops_t")));
typedef esp_at_ble_ops_t    esp_at_custom_ble_ops_struct __attribute__((deprecated("Use esp_at_ble_ops_t")));
typedef esp_at_custom_ops_t esp_at_custom_ops_struct     __attribute__((deprecated("Use esp_at_custom_ops_t")));

/* --- Enum typedefs --- */
typedef esp_at_status_t     esp_at_status_type      __attribute__((deprecated("Use esp_at_status_t")));
typedef esp_at_module_t     esp_at_module_id        __attribute__((deprecated("Use esp_at_module_id")));
typedef esp_at_errno_t esp_at_error_code            __attribute__((deprecated("Use esp_at_errno_t")));
typedef esp_at_rc_t esp_at_result_code_string_index __attribute__((deprecated("Use esp_at_rc_t")));

/* --- Sleep-mode type and values --- */
typedef esp_at_sleep_mode_t at_sleep_mode_t __attribute__((deprecated("Use esp_at_sleep_mode_t")));
#define AT_DISABLE_SLEEP    ESP_AT_SLEEP_DISABLE    /* deprecated: use ESP_AT_SLEEP_DISABLE   */
#define AT_MIN_MODEM_SLEEP  ESP_AT_SLEEP_MIN_MODEM  /* deprecated: use ESP_AT_SLEEP_MIN_MODEM */
#define AT_LIGHT_SLEEP      ESP_AT_SLEEP_LIGHT      /* deprecated: use ESP_AT_SLEEP_LIGHT     */
#define AT_MAX_MODEM_SLEEP  ESP_AT_SLEEP_MAX_MODEM  /* deprecated: use ESP_AT_SLEEP_MAX_MODEM */
#define AT_SLEEP_MAX        ESP_AT_SLEEP_MODE_MAX   /* deprecated: use ESP_AT_SLEEP_MODE_MAX  */

/* --- Parse-result type and values --- */
typedef esp_at_para_parse_ret_t esp_at_para_parse_result_type
__attribute__((deprecated("Use esp_at_para_parse_ret_t")));
#define ESP_AT_PARA_PARSE_RESULT_FAIL     ESP_AT_PARA_PARSE_RET_FAIL    /* deprecated */
#define ESP_AT_PARA_PARSE_RESULT_OK       ESP_AT_PARA_PARSE_RET_OK      /* deprecated */
#define ESP_AT_PARA_PARSE_RESULT_OMITTED  ESP_AT_PARA_PARSE_RET_OMITTED /* deprecated */

/*
 * --- Functions ---
 * Result-output APIs renamed for clarity:
 *   - esp_at_response_result() -> esp_at_write_result()
 *   - at_handle_result_code()  -> esp_at_dispatch_result()
 * Each old name is a plain deprecated declaration that resolves to the same
 * AT core library symbol as the new name.
 */
__attribute__((deprecated("Use esp_at_write_result")))
void esp_at_response_result(uint8_t result_code);
__attribute__((deprecated("Use esp_at_dispatch_result")))
void at_handle_result_code(esp_at_rc_t code, void *pbuf);

/*
 * --- Registration functions renamed from *_regist to *_register ---
 * These symbols are provided by the AT core library under the legacy
 * *_regist name, so each old name is a plain deprecated declaration that
 * resolves to that same symbol. New code should use the *_register names
 * declared in <esp_at_core.h> / <esp_at_cmd_register.h>.
 */
__attribute__((deprecated("Use esp_at_custom_cmd_array_register")))
bool esp_at_custom_cmd_array_regist(const esp_at_cmd_t *custom_at_cmd_array, uint32_t cmd_num);
__attribute__((deprecated("Use esp_at_device_ops_register")))
void esp_at_device_ops_regist(esp_at_intf_ops_t *ops);
__attribute__((deprecated("Use esp_at_custom_net_ops_register")))
bool esp_at_custom_net_ops_regist(int32_t link_id, esp_at_net_ops_t *ops);
__attribute__((deprecated("Use esp_at_custom_ble_ops_register")))
bool esp_at_custom_ble_ops_regist(int32_t conn_index, esp_at_ble_ops_t *ops);
__attribute__((deprecated("Use esp_at_custom_ops_register")))
void esp_at_custom_ops_regist(esp_at_custom_ops_t *ops);

/*
 * --- AT command-set registration functions renamed from *_cmd_regist ---
 * Declares one deprecated alias per library-provided command-set register
 * function. Each resolves to the legacy *_cmd_regist symbol in the AT core
 * library; use the *_cmd_register names instead.
 */
#define ESP_AT_LEGACY_CMD_REGIST(old_fn, new_fn) \
    __attribute__((deprecated("Use " #new_fn))) bool old_fn(void)

ESP_AT_LEGACY_CMD_REGIST(esp_at_base_cmd_regist,        esp_at_base_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_wifi_cmd_regist,        esp_at_wifi_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_smartconfig_cmd_regist, esp_at_smartconfig_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_wps_cmd_regist,         esp_at_wps_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_eap_cmd_regist,         esp_at_eap_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_mdns_cmd_regist,        esp_at_mdns_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_net_cmd_regist,         esp_at_net_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_ping_cmd_regist,        esp_at_ping_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_mqtt_cmd_regist,        esp_at_mqtt_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_http_cmd_regist,        esp_at_http_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_ws_cmd_regist,          esp_at_ws_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_ble_cmd_regist,         esp_at_ble_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_ble_hid_cmd_regist,     esp_at_ble_hid_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_blufi_cmd_regist,       esp_at_blufi_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_ble_ota_cmd_regist,     esp_at_ble_ota_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_bt_cmd_regist,          esp_at_bt_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_bt_spp_cmd_regist,      esp_at_bt_spp_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_bt_a2dp_cmd_regist,     esp_at_bt_a2dp_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_fs_cmd_regist,          esp_at_fs_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_driver_cmd_regist,      esp_at_driver_cmd_register);
ESP_AT_LEGACY_CMD_REGIST(esp_at_eth_cmd_regist,         esp_at_eth_cmd_register);

#undef ESP_AT_LEGACY_CMD_REGIST

/* ============================================================
 *          Deprecated names originally from esp_at.h
 * ============================================================ */

/* --- Filesystem type and values --- */
typedef esp_at_fs_type_t at_fs_type_t __attribute__((deprecated("Use esp_at_fs_type_t")));
#define AT_FS_TYPE_FATFS    ESP_AT_FS_FATFS     /* deprecated: use ESP_AT_FS_FATFS    */
#define AT_FS_TYPE_LITTLEFS ESP_AT_FS_LITTLEFS  /* deprecated: use ESP_AT_FS_LITTLEFS */
#define AT_FS_TYPE_MAX      ESP_AT_FS_TYPE_MAX  /* deprecated: use ESP_AT_FS_TYPE_MAX */

/*
 * esp_at_exe_cmd() is implemented in esp-at sources (not the AT core library),
 * so the legacy name forwards to it through a thin deprecated inline wrapper
 * instead of aliasing a symbol.
 */
#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
__attribute__((deprecated("Use esp_at_exe_cmd")))
static inline esp_err_t at_exe_cmd(const char *cmd, const char *expected_response, uint32_t timeout_ms)
{
    return esp_at_exe_cmd(cmd, expected_response, timeout_ms);
}
#endif

#ifdef __cplusplus
}
#endif
