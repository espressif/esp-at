/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file esp_at_core.h
 * @brief Public AT APIs provided by the AT core library.
 *
 * These APIs are implemented in the prebuilt AT core library and are intended
 * to be called by user applications and custom AT commands: command and
 * device-operation registration, parameter parsing, port I/O, result/error
 * handling, and the Wi-Fi / TCP-IP / HTTP / WebSocket helpers.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_partition.h"
#include "freertos/FreeRTOS.h"

/*
 * Optional-component headers are pulled in only when the matching AT command
 * feature is enabled, so that builds without those components are not forced
 * to depend on the lwIP socket API / esp_http_client / esp_websocket_client.
 */

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
#include "esp_wifi.h"
#endif

#ifdef CONFIG_AT_NET_COMMAND_SUPPORT
#include <sys/socket.h>
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#endif

#ifdef CONFIG_AT_HTTP_COMMAND_SUPPORT
#include "esp_http_client.h"
#endif

#ifdef CONFIG_AT_WS_COMMAND_SUPPORT
#include "esp_websocket_client.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 *                                Enumerations
 * ========================================================================== */

/**
 * @brief Operational status of the AT core.
 */
typedef enum {
    ESP_AT_STATUS_NORMAL    = 0x0,  /**< Command mode: processing AT commands   */
    ESP_AT_STATUS_TRANSMIT,         /**< Passthrough mode: forwarding raw data  */
} esp_at_status_t;

/**
 * @brief Supported sleep mode of the AT core.
 */
typedef enum {
    ESP_AT_SLEEP_DISABLE    = 0,    /**< Sleep disabled                         */
    ESP_AT_SLEEP_MIN_MODEM,         /**< Minimum modem sleep (DTIM-based)       */
    ESP_AT_SLEEP_LIGHT,             /**< Light sleep                            */
    ESP_AT_SLEEP_MAX_MODEM,         /**< Maximum modem sleep                    */
    ESP_AT_SLEEP_MODE_MAX,          /**< Sentinel value; not a valid mode       */
} esp_at_sleep_mode_t;

/**
 * @brief Return value of the parameter-parsing helpers.
 */
typedef enum {
    ESP_AT_PARA_PARSE_RET_FAIL      = -1,   /**< Parse failed; parameter is malformed   */
    ESP_AT_PARA_PARSE_RET_OK        =  0,   /**< Parameter parsed successfully          */
    ESP_AT_PARA_PARSE_RET_OMITTED,          /**< Parameter was omitted (empty field)    */
} esp_at_para_parse_ret_t;

/**
 * @brief Result code selectors for esp_at_write_result() and esp_at_dispatch_result().
 */
typedef enum {
    ESP_AT_RESULT_CODE_OK                  = 0x00,  /**< OK                                 */
    ESP_AT_RESULT_CODE_ERROR               = 0x01,  /**< ERROR                              */
    ESP_AT_RESULT_CODE_FAIL                = 0x02,  /**< ERROR                              */
    ESP_AT_RESULT_CODE_SEND_OK             = 0x03,  /**< SEND OK                            */
    ESP_AT_RESULT_CODE_SEND_FAIL           = 0x04,  /**< SEND FAIL                          */
    ESP_AT_RESULT_CODE_IGNORE              = 0x05,  /**< (no output)                        */
    ESP_AT_RESULT_CODE_PROCESS_DONE        = 0x06,  /**< (processing complete, no output)   */
    ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT = 0x07,  /**< OK followed by input prompt        */
    ESP_AT_RESULT_CODE_MAX,                         /**< Sentinel; not a valid code         */
} esp_at_rc_t;

/* ============================================================================
 *                              Data Structures
 * ========================================================================== */

/**
 * @brief Callback invoked while the AT core is in a specific receive mode.
 */
typedef void (*esp_at_port_specific_callback_t)(void);

/**
 * @brief Descriptor for a single AT command.
 *
 * Register an array of these via esp_at_custom_cmd_array_register().
 * Set unused command-type handlers to NULL.
 */
typedef struct {
    char    *cmd_name;                              /**< Command name string, e.g. "+MYCOMMAND" */
    uint8_t (*test_cmd)(uint8_t *cmd_name);         /**< Handler for AT+CMD=? */
    uint8_t (*query_cmd)(uint8_t *cmd_name);        /**< Handler for AT+CMD? */
    uint8_t (*setup_cmd)(uint8_t para_num);         /**< Handler for AT+CMD=\<params\> */
    uint8_t (*exe_cmd)(uint8_t *cmd_name);          /**< Handler for AT+CMD */
} esp_at_cmd_t;

/**
 * @brief I/O operation callbacks for the underlying AT transport device.
 *
 * Pass a populated instance to esp_at_device_ops_register().
 */
typedef struct {
    int32_t (*read_data)(uint8_t *data, int32_t len);       /**< Read bytes from the physical interface      */
    int32_t (*write_data)(uint8_t *data, int32_t len);      /**< Write bytes to the physical interface       */
    int32_t (*get_data_length)(void);                       /**< Return the number of bytes available        */
    bool (*wait_write_complete)(int32_t timeout_msec);      /**< Block until the transmit buffer is drained  */
} esp_at_intf_ops_t;

/**
 * @brief Callbacks for a socket-based passthrough (transparent-transmission) link.
 *
 * Register via esp_at_custom_net_ops_register().
 */
typedef struct {
    int32_t (*recv_data)(uint8_t *data, int32_t len);   /**< Invoked when data arrives on the link  */
    void (*connect_cb)(void);                           /**< Invoked when the link is established   */
    void (*disconnect_cb)(void);                        /**< Invoked when the link is torn down     */
} esp_at_net_ops_t;

/**
 * @brief Callbacks for a BLE passthrough link.
 *
 * Register via esp_at_custom_ble_ops_register().
 */
typedef struct {
    int32_t (*recv_data)(uint8_t *data, int32_t len);   /**< Invoked when data arrives on the BLE link  */
    void (*connect_cb)(void);                           /**< Invoked when the BLE link is established   */
    void (*disconnect_cb)(void);                        /**< Invoked when the BLE link is torn down     */
} esp_at_ble_ops_t;

/**
 * @brief Lifecycle and status callbacks for AT core events.
 *
 * Pass a populated instance to esp_at_custom_ops_register().
 */
typedef struct {
    void (*status_callback)(esp_at_status_t status);        /**< Called on AT core status change    */
    void (*pre_sleep_callback)(esp_at_sleep_mode_t mode);   /**< Called before entering sleep       */
    void (*pre_wakeup_callback)(void);                      /**< Called before waking from sleep    */
    void (*pre_deepsleep_callback)(void);                   /**< Called before entering deep sleep  */
    void (*pre_restart_callback)(void);                     /**< Called before system restart       */
    void (*pre_active_write_data_callback)(int32_t (*write_fn)(uint8_t *data, int32_t len));
    /**< Called before an active data write */
} esp_at_custom_ops_t;

/* ============================================================================
 *                                Error Codes
 * ========================================================================== */

/** @brief AT core module identifier, used as the high byte of error codes. */
typedef enum {
    ESP_AT_MODULE_NUM = 0x01,
} esp_at_module_t;

/** @brief AT command error sub-categories. */
typedef enum {
    ESP_AT_SUB_OK                   = 0x00, /**< No error                               */
    ESP_AT_SUB_COMMON_ERROR         = 0x01, /**< Generic error                          */
    ESP_AT_SUB_NO_TERMINATOR        = 0x02, /**< Missing line terminator                */
    ESP_AT_SUB_NO_AT                = 0x03, /**< Missing AT prefix                      */
    ESP_AT_SUB_PARA_LENGTH_MISMATCH = 0x04, /**< Parameter length out of range          */
    ESP_AT_SUB_PARA_TYPE_MISMATCH   = 0x05, /**< Parameter type mismatch                */
    ESP_AT_SUB_PARA_NUM_MISMATCH    = 0x06, /**< Wrong number of parameters             */
    ESP_AT_SUB_PARA_INVALID         = 0x07, /**< Parameter value out of valid range     */
    ESP_AT_SUB_PARA_PARSE_FAIL      = 0x08, /**< Failed to parse parameter              */
    ESP_AT_SUB_UNSUPPORT_CMD        = 0x09, /**< Command not supported                  */
    ESP_AT_SUB_CMD_EXEC_FAIL        = 0x0A, /**< Command execution failed               */
    ESP_AT_SUB_CMD_PROCESSING       = 0x0B, /**< A command is already in progress       */
    ESP_AT_SUB_CMD_OP_ERROR         = 0x0C, /**< Wrong operation type for command       */
} esp_at_errno_t;

/**
 * @brief Encode an AT error code from its constituent fields.
 *
 * @param subcategory  Error sub-category (esp_at_errno_t).
 * @param extension    Command-specific extension value.
 * @return Packed 32-bit error code: [module:8][sub:8][ext:16].
 */
#define ESP_AT_ERROR_NO(subcategory, extension) \
    ((ESP_AT_MODULE_NUM << 24) | ((subcategory) << 16) | (extension))

#define ESP_AT_CMD_ERROR_OK                          ESP_AT_ERROR_NO(ESP_AT_SUB_OK, 0x00)                                     /**< No error */
#define ESP_AT_CMD_ERROR_NON_FINISH                  ESP_AT_ERROR_NO(ESP_AT_SUB_NO_TERMINATOR, 0x00)                          /**< Command terminator ("\r\n") not found */
#define ESP_AT_CMD_ERROR_NOT_FOUND_AT                ESP_AT_ERROR_NO(ESP_AT_SUB_NO_AT, 0x00)                                  /**< Command does not start with "AT" */
#define ESP_AT_CMD_ERROR_PARA_LENGTH(which_para)     ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_LENGTH_MISMATCH, which_para)             /**< Parameter length mismatch (@p which_para: 0-based index) */
#define ESP_AT_CMD_ERROR_PARA_TYPE(which_para)       ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_TYPE_MISMATCH, which_para)               /**< Parameter type mismatch (@p which_para: 0-based index) */
#define ESP_AT_CMD_ERROR_PARA_NUM(need, given)       ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_NUM_MISMATCH, (((need) << 8) | (given))) /**< Parameter count mismatch (@p need expected, @p given received) */
#define ESP_AT_CMD_ERROR_PARA_INVALID(which_para)    ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_INVALID, which_para)                     /**< Invalid parameter value (@p which_para: 0-based index) */
#define ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(which_para) ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_PARSE_FAIL, which_para)                  /**< Failed to parse a parameter (@p which_para: 0-based index) */
#define ESP_AT_CMD_ERROR_CMD_UNSUPPORT               ESP_AT_ERROR_NO(ESP_AT_SUB_UNSUPPORT_CMD, 0x00)                          /**< Command not supported */
#define ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(result)       ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_EXEC_FAIL, result)                        /**< Command execution failed (@p result: handler-specific code) */
#define ESP_AT_CMD_ERROR_CMD_PROCESSING              ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_PROCESSING, 0x00)                         /**< A previous command is still being processed */
#define ESP_AT_CMD_ERROR_CMD_OP_ERROR                ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_OP_ERROR, 0x00)                           /**< Unsupported command operation type */

/* ============================================================================
 *                                Registration
 * ========================================================================== */

/**
 * @brief Register a custom AT command set.
 *
 * @param custom_at_cmd_array  Array of esp_at_cmd_t descriptors.
 * @param cmd_num              Number of entries in the array.
 * @return true on success, false if registration failed.
 */
bool esp_at_custom_cmd_array_register(const esp_at_cmd_t *custom_at_cmd_array, uint32_t cmd_num)
__asm__("esp_at_custom_cmd_array_regist");

/**
 * @brief Register the device I/O operations for the AT transport interface.
 *
 * @param ops  Pointer to a populated esp_at_intf_ops_t.
 */
void esp_at_device_ops_register(esp_at_intf_ops_t *ops)
__asm__("esp_at_device_ops_regist");

/**
 * @brief Register passthrough callbacks for a socket-based network link.
 *
 * @note Call this only after esp_at_module_init().
 *
 * @param link_id  Link identifier (0-based).
 * @param ops      Pointer to a populated esp_at_net_ops_t.
 * @return true on success.
 */
bool esp_at_custom_net_ops_register(int32_t link_id, esp_at_net_ops_t *ops)
__asm__("esp_at_custom_net_ops_regist");

/**
 * @brief Register passthrough callbacks for a BLE link.
 *
 * @note Call this only after esp_at_module_init().
 *
 * @param conn_index  BLE connection index (0-based).
 * @param ops         Pointer to a populated esp_at_ble_ops_t.
 * @return true on success.
 */
bool esp_at_custom_ble_ops_register(int32_t conn_index, esp_at_ble_ops_t *ops)
__asm__("esp_at_custom_ble_ops_regist");

/**
 * @brief Register lifecycle callbacks for AT core events.
 *
 * @param ops  Pointer to a populated esp_at_custom_ops_t.
 */
void esp_at_custom_ops_register(esp_at_custom_ops_t *ops)
__asm__("esp_at_custom_ops_regist");

/* ============================================================================
 *                              Command Context
 * ========================================================================== */

/**
 * @brief Return the name of the AT command currently being executed.
 *
 * @return Pointer to a NUL-terminated command name string.
 *         Valid only during command handler execution.
 */
const uint8_t *esp_at_get_current_cmd_name(void);

/**
 * @brief Look up a flash partition in the secondary (user) partition table.
 *
 * Searches only partitions defined in at_customize.csv (the secondary partition
 * table), not the primary system partition table partitions_at.csv.
 *
 * @note To look up the primary partition table first and fall back to the
 *       secondary table when no match is found, use esp_partition_find_first().
 *       In ESP-AT, this IDF API is linker-wrapped to perform that two-stage lookup.
 *
 * @param type     Partition type.
 * @param subtype  Partition subtype.
 * @param label    Partition label string.
 * @return Pointer to the matching esp_partition_t, or NULL if not found.
 */
const esp_partition_t *esp_at_custom_partition_find(esp_partition_type_t type,
                                                    esp_partition_subtype_t subtype,
                                                    const char *label);

/* ============================================================================
 *                             Parameter Parsing
 * ========================================================================== */

/**
 * @brief Parse the parameter at @p para_index as a signed 32-bit integer.
 *
 * @param para_index  Zero-based parameter position in the AT command line.
 * @param value       Output: parsed integer value.
 * @return ESP_AT_PARA_PARSE_RET_OK, _FAIL, or _OMITTED.
 */
esp_at_para_parse_ret_t esp_at_get_para_as_digit(int32_t para_index, int32_t *value);

/**
 * @brief Parse the parameter at @p para_index as a single-precision float.
 *
 * @param para_index  Zero-based parameter position.
 * @param value       Output: parsed float value.
 * @return ESP_AT_PARA_PARSE_RET_OK, _FAIL, or _OMITTED.
 */
esp_at_para_parse_ret_t esp_at_get_para_as_float(int32_t para_index, float *value);

/**
 * @brief Return a pointer to the raw string for the parameter at @p para_index.
 *
 * The returned pointer refers to the AT core's internal command buffer.
 * The caller must not free or modify the pointed-to string.
 *
 * @param para_index  Zero-based parameter position.
 * @param result      Output: pointer to the NUL-terminated parameter string.
 * @return ESP_AT_PARA_PARSE_RET_OK, _FAIL, or _OMITTED.
 */
esp_at_para_parse_ret_t esp_at_get_para_as_str(int32_t para_index, uint8_t **result);

/* ============================================================================
 *                                  Port I/O
 * ========================================================================== */

/**
 * @brief Notify the AT core that @p len bytes have arrived on the port (task context).
 *
 * @param len   Number of newly received bytes.
 * @param msec  Maximum time to wait for the core to accept the notification (ms).
 * @return true if the notification was accepted within the timeout.
 */
bool esp_at_port_recv_data_notify(int32_t len, uint32_t msec);

/**
 * @brief Read up to @p len bytes from the AT input port.
 *
 * @param data  Destination buffer.
 * @param len   Maximum number of bytes to read.
 * @return Number of bytes actually read, or negative on error.
 */
int32_t esp_at_port_read_data(uint8_t *data, int32_t len);

/*
 * The four esp_at_port_*write_data* functions below all write @p len bytes to
 * the AT output port, but differ along two independent axes:
 *
 *   - Message filtering: whether the output is subject to AT+SYSMSGFILTER.
 *       "_without_filter" variants bypass the filter; the others honor it
 *       (when filtering is enabled).
 *   - MCU wakeup: whether the host MCU is woken before the data is sent.
 *       "active_" variants invoke the pre-write callback registered via
 *       esp_at_custom_ops_register(), which wakes the MCU according to the
 *       AT+USERWKMCUCFG configuration; the others do not.
 *
 *   Function                                       | AT+SYSMSGFILTER | Wakes MCU
 *   -----------------------------------------------+-----------------+----------
 *   esp_at_port_write_data                         | applied         | no
 *   esp_at_port_write_data_without_filter          | bypassed        | no
 *   esp_at_port_active_write_data                  | applied         | yes
 *   esp_at_port_active_write_data_without_filter   | bypassed        | yes
 */

/**
 * @brief Write @p len bytes to the AT output port.
 *
 * The output is subject to AT+SYSMSGFILTER (when filtering is enabled). The
 * host MCU is not woken before the data is sent.
 *
 * @param data  Data buffer.
 * @param len   Number of bytes to write.
 * @return Number of bytes written, or negative on error.
 */
int32_t esp_at_port_write_data(uint8_t *data, int32_t len);

/**
 * @brief Write @p len bytes to the AT output port, waking the host MCU first.
 *
 * The output is subject to AT+SYSMSGFILTER (when filtering is enabled). Before
 * sending, the pre-write callback registered via esp_at_custom_ops_register()
 * is invoked, which wakes the host MCU according to the AT+USERWKMCUCFG
 * configuration.
 *
 * @param data  Data buffer.
 * @param len   Number of bytes to write.
 * @return Number of bytes written, or negative on error.
 */
int32_t esp_at_port_active_write_data(uint8_t *data, int32_t len);

/**
 * @brief Write @p len bytes to the AT output port, bypassing AT+SYSMSGFILTER.
 *
 * The output is never filtered by AT+SYSMSGFILTER. The host MCU is not woken
 * before the data is sent.
 *
 * @param data  Data buffer.
 * @param len   Number of bytes to write.
 * @return Number of bytes written, or negative on error.
 */
int32_t esp_at_port_write_data_without_filter(uint8_t *data, int32_t len);

/**
 * @brief Write @p len bytes to the AT output port, waking the host MCU first
 *        and bypassing AT+SYSMSGFILTER.
 *
 * The output is never filtered by AT+SYSMSGFILTER. Before sending, the
 * pre-write callback registered via esp_at_custom_ops_register() is invoked,
 * which wakes the host MCU according to the AT+USERWKMCUCFG configuration.
 *
 * @param data  Data buffer.
 * @param len   Number of bytes to write.
 * @return Number of bytes written, or negative on error.
 */
int32_t esp_at_port_active_write_data_without_filter(uint8_t *data, int32_t len);

/**
 * @brief Block until the AT output port transmit buffer is empty.
 *
 * @param timeout_msec  Maximum wait time in milliseconds.
 * @return true if the buffer drained before the timeout.
 */
bool esp_at_port_wait_write_complete(int32_t timeout_msec);

/**
 * @brief Return the number of bytes currently pending on the AT input port.
 *
 * @return
 *  - >= 0  : the length of the data received
 *  - others: failure
 */
int32_t esp_at_port_get_data_length(void);

/**
 * @brief Enter a special receive mode; @p callback is invoked on each data arrival.
 *
 * @param callback  Function called when new data arrives.
 */
void esp_at_port_enter_specific(esp_at_port_specific_callback_t callback);

/**
 * @brief Exit the special receive mode entered by esp_at_port_enter_specific().
 */
void esp_at_port_exit_specific(void);

/* ============================================================================
 *                              Command Response
 * ========================================================================== */

/**
 * @brief Output a standard result string (OK, ERROR, SEND OK, etc.) on the AT port.
 *
 * This only writes the corresponding result string to the AT port; it does not
 * change the state of the AT port receive task. Use it when, after emitting the
 * result string, the command handler still needs to run further logic that is
 * unrelated to receiving new data on the AT port.
 *
 * @note If the command is finished and the AT port should resume accepting new
 *       input, use esp_at_dispatch_result() instead.
 *
 * @param result_code  One of the esp_at_rc_t values cast to uint8_t.
 */
void esp_at_write_result(uint8_t result_code)
__asm__("esp_at_response_result");

/**
 * @brief Output a result string and return the AT port to the ready state.
 *
 * In addition to outputting the corresponding result string (like
 * esp_at_write_result()), this dispatches the result code through any
 * registered result handlers and brings the AT command receive task back to
 * its idle/ready state, so that the AT port can accept and process new input.
 *
 * Use it when the command handler has finished and the next step is to receive
 * fresh data from the AT port. For example, after printing "OK" and the ">"
 * data prompt, call this so the AT port is ready to receive the new input that
 * the handler expects.
 *
 * @param code  Result code to dispatch.
 * @param pbuf  Optional command-specific data; set it to NULL if not needed.
 */
void esp_at_dispatch_result(esp_at_rc_t code, void *pbuf)
__asm__("at_handle_result_code");

/* ============================================================================
 *                               System Control
 * ========================================================================== */

/**
 * @brief Restart the system with a hardware watchdog safety net.
 *
 * Arms a 3-second hardware timer before calling the esp_restart() routine. If the
 * normal restart path stalls, the watchdog forces a hard reset. No output is
 * generated on the AT port before the restart.
 */
void esp_at_restart(void) __asm__("at_restart");

/**
 * @brief Respond with OK on the AT port, run the pre_restart callback, then restart.
 *
 * Use this variant to restart in response to an AT command so that the host
 * receives an OK acknowledgement before the device reboots. Arms the same
 * 3-second hardware watchdog as esp_at_restart().
 */
void esp_at_restart_async(void) __asm__("at_handle_restart");

/* ============================================================================
 *                                 Utilities
 * ========================================================================== */

/**
 * @brief Convert a colon-separated MAC address string to a 6-byte binary array.
 *
 * Accepts the format "XX:XX:XX:XX:XX:XX" (case-insensitive hex digits).
 *
 * @param str  NUL-terminated MAC address string.
 * @param mac  Output: 6-byte binary MAC address.
 * @return true on success, false if the string is malformed.
 */
bool esp_at_str_2_mac(const char *str, uint8_t mac[6]) __asm__("at_str_2_mac");

/* ============================================================================
 *                                   Wi-Fi
 * ========================================================================== */

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT

/**
 * @brief Same as esp_wifi_connect(), but also compatible with the AT Wi-Fi
 *        command set.
 *
 * @note Use this instead of esp_wifi_connect() to start a Wi-Fi connection.
 *
 * @return ESP_OK on success, or a Wi-Fi stack error code.
 */
esp_err_t esp_at_wifi_connect(void) __asm__("at_wifi_connect");

/**
 * @brief Same as esp_wifi_disconnect(), but also compatible with the AT Wi-Fi
 *        command set.
 *
 * @note Use this instead of esp_wifi_disconnect() to stop a Wi-Fi connection.
 *
 * @return ESP_OK on success, or a Wi-Fi stack error code.
 */
esp_err_t esp_at_wifi_disconnect(void) __asm__("at_wifi_disconnect");

/**
 * @brief Initialize the Wi-Fi reconnect state machine before a connection attempt.
 *
 * Must be called before esp_at_wifi_connect(). If the connection drops or the
 * initial connect fails, the AT core automatically attempts to reconnect
 * according to the selected policy.
 *
 * @param force  true  — reconnect unconditionally until esp_at_wifi_reconnect_stop()
 *                       is called explicitly.
 *               false — reconnect using the interval and count configured via
 *                       AT+CWRECONNCFG.
 */
void esp_at_wifi_reconnect_init(bool force) __asm__("at_wifi_reconnect_init");

/**
 * @brief Stop an active Wi-Fi reconnect sequence.
 *
 * If an IP address has already been acquired, the existing connection is
 * preserved and not torn down.
 */
void esp_at_wifi_reconnect_stop(void) __asm__("at_wifi_reconnect_stop");

/**
 * @brief Start a Wi-Fi scan using the AT command calling convention.
 *
 * If a reconnect sequence is in progress, it is suspended for the duration
 * of the scan and then automatically resumed on completion.
 *
 * @param config  Scan configuration, or NULL for a default full passive scan.
 * @param block   true to block until the scan completes.
 * @return ESP_OK on success, or a Wi-Fi stack error code.
 */
esp_err_t esp_at_wifi_scan_start(const wifi_scan_config_t *config, bool block)
__asm__("at_wifi_scan_start");

#endif /* CONFIG_AT_WIFI_COMMAND_SUPPORT */

/* ============================================================================
 *                                   TCP/IP
 * ========================================================================== */

#ifdef CONFIG_AT_NET_COMMAND_SUPPORT

/**
 * @brief IP protocol family preference for DNS resolution.
 */
typedef enum {
    ESP_AT_IPPROTO_UNSPEC       = 0,    /**< No preference; use the system default      */
    ESP_AT_IPPROTO_IPV4_IPV6,           /**< Accept both IPv4 and IPv6 addresses        */
    ESP_AT_IPPROTO_IPV4_ONLY,           /**< Resolve IPv4 addresses only                */
    ESP_AT_IPPROTO_IPV6_ONLY,           /**< Resolve IPv6 addresses only                */
} esp_at_ip_proto_t;

/**
 * @brief Network interface type associated with a socket or connection.
 */
typedef enum {
    ESP_AT_NETIF_NONE   = 0,    /**< No network interface           */
    ESP_AT_NETIF_STA,           /**< Wi-Fi station interface        */
    ESP_AT_NETIF_AP,            /**< Wi-Fi soft-AP interface        */
    ESP_AT_NETIF_ETH,           /**< Ethernet interface             */
    ESP_AT_NETIF_MAX,           /**< Sentinel value                 */
} esp_at_netif_t;

/**
 * @brief Connect a socket with an explicit timeout.
 *
 * Behaves like POSIX connect() with the following differences:
 *   - Configurable timeout via @p timeout_ms.
 *   - Requires approximately 2 KB of additional heap memory.
 *   - The file descriptor lifecycle is managed internally; do not close it
 *     manually after calling this function.
 *
 * Additional error codes not returned by connect():
 *   - ESP_ERR_INVALID_ARG — invalid argument
 *   - ESP_ERR_NO_MEM      — memory allocation failed
 *   - ESP_ERR_TIMEOUT     — connection timed out
 *
 * @param fd          Socket file descriptor.
 * @param name        Destination address structure.
 * @param namelen     Size of the address structure.
 * @param timeout_ms  Connection timeout in milliseconds; must be greater than 0.
 * @return 0 on success, or a negative error code.
 */
int esp_at_connect(int fd, const struct sockaddr *name, socklen_t namelen, int timeout_ms)
__asm__("at_connect");

/**
 * @brief Return the socket file descriptor associated with a TCP/UDP/SSL link ID.
 *
 * @param link_id  AT connection link identifier (0-based).
 * @return Socket file descriptor, or a negative value if the link ID is invalid.
 */
int32_t esp_at_get_socket_by_link_id(uint8_t link_id);

/**
 * @brief Return the network interface type associated with a socket.
 *
 * @param fd  Socket file descriptor.
 * @return The corresponding esp_at_netif_t, or ESP_AT_NETIF_NONE if unknown.
 */
esp_at_netif_t esp_at_get_netif_by_socket(int fd);

/**
 * @brief Resolve a hostname to a single IP address with timeout control.
 *
 * @note @p hostname may also be an IPv4/IPv6 address literal string.
 *
 * @note When @p timeout_ms > 0, the call uses approximately 2 KB of additional heap
 *       memory. When @p timeout_ms == 0, no extra 2 KB is
 *       used and the timeout is controlled internally by lwIP.
 *
 * @param hostname    NUL-terminated hostname or IPv4/IPv6 address string.
 * @param ip_proto    Address family preference.
 * @param target_addr Output: resolved IP address.
 * @param timeout_ms  Resolution timeout in milliseconds.
 * @return
 *   - ESP_OK             — resolved successfully
 *   - ESP_ERR_NOT_FOUND  — hostname not found
 *   - ESP_ERR_TIMEOUT    — resolution timed out
 *   - ESP_ERR_NO_MEM     — memory allocation failed
 *   - Other esp_err_t codes from the underlying DNS stack
 */
esp_err_t esp_at_hostname_to_ipaddr(const char *hostname, esp_at_ip_proto_t ip_proto,
                                    ip_addr_t *target_addr, uint32_t timeout_ms)
__asm__("at_hostname_to_ipaddr");

/**
 * @brief Resolve a hostname to a linked list of addrinfo structures.
 *
 * Returns detailed address information and supports multiple resolved addresses
 * for a single hostname. The caller is responsible for freeing @p res with
 * freeaddrinfo().
 *
 * @note @p hostname may also be an IPv4/IPv6 address literal string. The timeout and
 * heap-usage behavior is the same as esp_at_hostname_to_ipaddr().
 *
 * @param hostname    NUL-terminated hostname or IPv4/IPv6 address string.
 * @param ip_proto    Address family preference.
 * @param target_addr Output: first resolved IP address (may be NULL).
 * @param res         Output: head of the resolved addrinfo linked list.
 * @param timeout_ms  Resolution timeout in milliseconds.
 * @return Same error codes as esp_at_hostname_to_ipaddr().
 */
esp_err_t esp_at_hostname_to_addrinfo(const char *hostname, esp_at_ip_proto_t ip_proto,
                                      ip_addr_t *target_addr, struct addrinfo **res,
                                      uint32_t timeout_ms)
__asm__("at_hostname_to_addrinfo");

#endif /* CONFIG_AT_NET_COMMAND_SUPPORT */

/* ============================================================================
 *                                HTTP Client
 * ========================================================================== */

#ifdef CONFIG_AT_HTTP_COMMAND_SUPPORT

/**
 * @brief Apply PKI configuration from AT+HTTPCFG to an HTTP client config struct.
 *
 * Call this before creating the HTTP client handle. The function copies
 * the certificate and key material configured via AT+HTTPCFG into @p config.
 *
 * @param config  HTTP client configuration to populate.
 * @return ESP_OK on success, or an error code.
 */
esp_err_t esp_at_http_set_pki_if_config(esp_http_client_config_t *config);

/**
 * @brief Release PKI resources previously applied by esp_at_http_set_pki_if_config().
 *
 * @param config  HTTP client configuration whose PKI fields are to be freed.
 */
void esp_at_http_free_pki_if_config(esp_http_client_config_t *config);

/**
 * @brief Apply request headers from AT+HTTPCHEAD to an HTTP client handle.
 *
 * Call this after creating the client handle and before initiating the request.
 *
 * @param handle  HTTP client handle.
 * @return ESP_OK on success, or an error code.
 */
esp_err_t esp_at_http_set_header_if_config(esp_http_client_handle_t handle);

/**
 * @brief Clear all request headers previously configured via AT+HTTPCHEAD.
 *
 * @return ESP_OK on success.
 */
esp_err_t esp_at_http_clear_header(void);

#endif /* CONFIG_AT_HTTP_COMMAND_SUPPORT */

/* ============================================================================
 *                              WebSocket Client
 * ========================================================================== */

#ifdef CONFIG_AT_WS_COMMAND_SUPPORT

/**
 * @brief Return the WebSocket client handle associated with a link ID.
 *
 * @param link_id  AT connection link identifier (0-2).
 * @return WebSocket client handle, or NULL if the link ID is invalid or inactive.
 */
esp_websocket_client_handle_t esp_at_get_ws_client_handle_by_link_id(uint8_t link_id);

/**
 * @brief Send data over a WebSocket connection with explicit opcode and FIN flag.
 *
 * @param handle   WebSocket client handle.
 * @param opcode   WebSocket frame opcode (e.g. WS_TRANSPORT_OPCODES_TEXT).
 * @param fin      true to mark the last frame; false for continuation frames.
 * @param data     Payload data buffer.
 * @param length   Payload length in bytes.
 * @param timeout  Maximum time to wait for the send to complete.
 * @return Number of bytes sent, or negative on error.
 */
int esp_at_websocket_client_send_by_opcode_fin(esp_websocket_client_handle_t handle,
                                               ws_transport_opcodes_t opcode, bool fin,
                                               const char *data, int length,
                                               TickType_t timeout);

#endif /* CONFIG_AT_WS_COMMAND_SUPPORT */

/* Deprecated aliases live in esp_at_legacy.h, included automatically via esp_at.h. */

#ifdef __cplusplus
}
#endif
