/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

/**
 * Internal structure describing ESP_AT_CMD_SET_FIRST_INIT_FN/ESP_AT_CMD_SET_INIT_FN/ESP_AT_CMD_SET_LAST_INIT_FN register functions.
 */
typedef struct {
    bool (*fn)(void);    /*!< Pointer to an AT command set register function */
    const char *name;    /*!< Name of the AT command set */
} at_cmd_set_register_t;

// Forces data into an at_cmd_set_first_init_fn section
#define AT_CMD_SET_FIRST_INIT_ATTR  __attribute__((used)) _SECTION_ATTR_IMPL(".at_cmd_set_first_init_fn", __COUNTER__)

// Forces data into an at_cmd_set_init_fn section
#define AT_CMD_SET_INIT_ATTR  __attribute__((used)) _SECTION_ATTR_IMPL(".at_cmd_set_init_fn", __COUNTER__)

// Forces data into an at_cmd_set_last_init_fn section
#define AT_CMD_SET_LAST_INIT_ATTR  __attribute__((used)) _SECTION_ATTR_IMPL(".at_cmd_set_last_init_fn", __COUNTER__)

/**
 * @brief Define an initialization function for AT command set register function which will be auto-executed in the first one.
 *
 * @note The AT command set from inside the esp-at project register functions should be initialized by this macro,
 *       to make sure it will be executed after AT initialization.
 *
 * @note The AT command set from outside the esp-at project register function should not initialized by this macro,
 *       and you can use ESP_AT_CMD_SET_INIT_FN or ESP_AT_CMD_SET_LAST_INIT_FN instead.
 *
 * @param f: The function name to be register (identifier)
 *
 * The function defined using this macro must return true on success. Any other value will be
 * logged and the register process will be skipped.
 *
 * Initialization functions should be placed in a compilation unit where at least one other
 * symbol is referenced in another compilation unit. This means that the reference should not itself
 * get optimized out by the compiler or discarded by the linker if the related feature is used.
 * It is, on the other hand, a good practice to make sure the initialization function does get
 * discarded if the related feature is not used.
 */
#define ESP_AT_CMD_SET_FIRST_INIT_FN(f)        \
    bool (f)(void);                            \
    static AT_CMD_SET_FIRST_INIT_ATTR at_cmd_set_register_t s_cmd_set_first_##f = {.fn = (f), .name = #f}

/**
 * @brief Define an initialization function for AT command set register function which will be auto-executed in the second one.
 *
 * @note The AT command set from outside the esp-at project register functions are recommended to be initialized by this macro.
 *
 * @param f: The function name to be register (identifier)
 *
 * The function defined using this macro must return true on success. Any other value will be
 * logged and the register process will be skipped.
 *
 * Initialization functions should be placed in a compilation unit where at least one other
 * symbol is referenced in another compilation unit. This means that the reference should not itself
 * get optimized out by the compiler or discarded by the linker if the related feature is used.
 * It is, on the other hand, a good practice to make sure the initialization function does get
 * discarded if the related feature is not used.
 */
#define ESP_AT_CMD_SET_INIT_FN(f)        \
    bool (f)(void);                      \
    static AT_CMD_SET_INIT_ATTR at_cmd_set_register_t s_cmd_set_##f = {.fn = (f), .name = #f}

/**
 * @brief Define an initialization function for AT command set register function which will be auto-executed in the last one.
 *
 * @param f: The function name to be register (identifier)
 *
 * The function defined using this macro must return true on success. Any other value will be
 * logged and the register process will be skipped.
 *
 * Initialization functions should be placed in a compilation unit where at least one other
 * symbol is referenced in another compilation unit. This means that the reference should not itself
 * get optimized out by the compiler or discarded by the linker if the related feature is used.
 * It is, on the other hand, a good practice to make sure the initialization function does get
 * discarded if the related feature is not used.
 */
#define ESP_AT_CMD_SET_LAST_INIT_FN(f)        \
    bool (f)(void);                           \
    static AT_CMD_SET_LAST_INIT_ATTR at_cmd_set_register_t s_cmd_set_last_##f = {.fn = (f), .name = #f}

/********************************************************************************
**
** The following are a series of registration functions from the esp-at project.
**
** Once it is initialized by ESP_AT_CMD_SET_FIRST_INIT_FN/ESP_AT_CMD_SET_INIT_FN,
** you can use the corresponding AT command set from inside the esp-at project.
**
********************************************************************************/

/**
 * @brief Register the basic AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_base_cmd_regist(void);

/**
 * @brief Register the Wi-Fi AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_wifi_cmd_regist(void);

/**
 * @brief Register the smartconfig AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_smartconfig_cmd_regist(void);

/**
 * @brief Register the WPS AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_wps_cmd_regist(void);

/**
 * @brief Register the Enterprise-AP AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_eap_cmd_regist(void);

/**
 * @brief Register the mDNS AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_mdns_cmd_regist(void);

/**
 * @brief Register the TCP-IP AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_net_cmd_regist(void);

/**
 * @brief Register the PING AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_ping_cmd_regist(void);

/**
 * @brief Register the MQTT AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_mqtt_cmd_regist(void);

/**
 * @brief Register the HTTP AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_http_cmd_regist(void);

/**
 * @brief Register the WebSocket AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_ws_cmd_regist(void);

/**
 * @brief Register the BLE AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_ble_cmd_regist(void);

/**
 * @brief Register the BLE HID AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_ble_hid_cmd_regist(void);

/**
 * @brief Register the BluFi AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_blufi_cmd_regist(void);

/**
 * @brief Register the Classic Bluetooth AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_bt_cmd_regist(void);

/**
 * @brief Register the Classic Bluetooth SPP AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_bt_spp_cmd_regist(void);

/**
 * @brief Register the Classic Bluetooth A2DP AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_bt_a2dp_cmd_regist(void);

/**
 * @brief Register the file system AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_fs_cmd_regist(void);

/**
 * @brief Register the driver AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_driver_cmd_regist(void);

/**
 * @brief Register the ethernet AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_eth_cmd_regist(void);

/**
 * @brief Register the signaling test AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_fact_cmd_regist(void);

/**
 * @brief Register the OTA AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_ota_cmd_regist(void);

/**
 * @brief Register the UART AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_uart_cmd_regist(void);

/**
 * @brief Register the user AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_user_cmd_regist(void);

/**
 * @brief Register the web server AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_web_server_cmd_regist(void);

/**
 * @brief Register the rainmaker AT commands.
 *
 * @return true if success, otherwise false.
 */
bool esp_at_rainmaker_cmd_regist(void);
