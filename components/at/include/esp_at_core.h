/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_partition.h"
#include "esp_wifi.h"

#define at_min(x,y)     ((x)<(y)?(x):(y))
#define at_max(x,y)     ((x)>(y)?(x):(y))

/**
 * @brief esp_at_cmd_struct
 *  used for define at command
 *
 */
typedef struct {
    char *at_cmdName;                               /*!< at command name */
    uint8_t (*at_testCmd)(uint8_t *cmd_name);       /*!< Test Command function pointer */
    uint8_t (*at_queryCmd)(uint8_t *cmd_name);      /*!< Query Command function pointer */
    uint8_t (*at_setupCmd)(uint8_t para_num);       /*!< Setup Command function pointer */
    uint8_t (*at_exeCmd)(uint8_t *cmd_name);        /*!< Execute Command function pointer */
} esp_at_cmd_struct;

/**
 * @brief esp_at_device_ops_struct
 *  device operate functions struct for AT
 *
 */
typedef struct {
    int32_t (*read_data)(uint8_t *data, int32_t len);               /*!< read data from device */
    int32_t (*write_data)(uint8_t *data, int32_t len);              /*!< write data into device */

    int32_t (*get_data_length)(void);                               /*!< get the length of data received */
    bool (*wait_write_complete)(int32_t timeout_msec);              /*!< wait write finish */
} esp_at_device_ops_struct;

typedef int32_t (*at_read_data_fn_t)(uint8_t *data, int32_t len);
typedef int32_t (*at_write_data_fn_t)(uint8_t *data, int32_t len);
typedef int32_t (*at_get_data_len_fn_t)(void);

/**
 * @brief esp_at_custom_net_ops_struct
 * custom socket callback for AT
 */
typedef struct {
    int32_t (*recv_data)(uint8_t*data, int32_t len); /*!< callback when socket received data */
    void (*connect_cb)(void);                        /*!< callback when socket connection is built */
    void (*disconnect_cb)(void);                     /*!< callback when socket connection is disconnected */
} esp_at_custom_net_ops_struct;

/**
 * @brief esp_at_custom_ble_ops_struct
 * custom ble callback for AT
 *
 */
typedef struct {
    int32_t (*recv_data)(uint8_t*data, int32_t len); /*!< callback when ble received data */
    void (*connect_cb)(void);                        /*!< callback when ble connection is built */
    void (*disconnect_cb)(void);                     /*!< callback when ble connection is disconnected */
} esp_at_custom_ble_ops_struct;

/**
 * @brief esp_at_status
 *  some custom function interacting with AT
 *
 */
typedef enum {
    ESP_AT_STATUS_NORMAL = 0x0,                /*!< Normal mode.Now mcu can send AT command */
    ESP_AT_STATUS_TRANSMIT,                    /*!< Transparent Transmition mode */

} esp_at_status_type;

typedef enum {
    AT_DISABLE_SLEEP = 0,
    AT_MIN_MODEM_SLEEP,
    AT_LIGHT_SLEEP,
    AT_MAX_MODEM_SLEEP,
    AT_SLEEP_MAX,
} at_sleep_mode_t;

/**
 * @brief esp_at_ops_struct
 *  some custom function interacting with AT
 *
 */
typedef struct {
    void (*status_callback)(esp_at_status_type status);               /*!< callback when AT status changes */
    void (*pre_sleep_callback)(at_sleep_mode_t mode);                 /*!< callback before entering light sleep */
    void (*pre_wakeup_callback)(void);                                /*!< callback before waking up from light sleep */
    void (*pre_deepsleep_callback)(void);                             /*!< callback before enter deep sleep */
    void (*pre_restart_callback)(void);                               /*!< callback before restart */
    void (*pre_active_write_data_callback)(at_write_data_fn_t);       /*!< callback before write data */
} esp_at_custom_ops_struct;

/**
 * @brief AT specific callback type
 *
 */
typedef void (*esp_at_port_specific_callback_t)(void);
// error number
/**
 * @brief module number,Now just AT module
 *
 */
typedef enum {
    ESP_AT_MODULE_NUM = 0x01   /*!< AT module */
} esp_at_module;

/**
 * @brief subcategory number
 *
 */
typedef enum {
    ESP_AT_SUB_OK                       = 0x00,              /*!< OK */
    ESP_AT_SUB_COMMON_ERROR             = 0x01,              /*!< reserved */
    ESP_AT_SUB_NO_TERMINATOR            = 0x02,              /*!< terminator character not found ("\r\n" expected) */
    ESP_AT_SUB_NO_AT                    = 0x03,              /*!< Starting "AT" not found (or at, At or aT entered) */
    ESP_AT_SUB_PARA_LENGTH_MISMATCH     = 0x04,              /*!< parameter length mismatch */
    ESP_AT_SUB_PARA_TYPE_MISMATCH       = 0x05,              /*!< parameter type mismatch */
    ESP_AT_SUB_PARA_NUM_MISMATCH        = 0x06,              /*!< parameter number mismatch */
    ESP_AT_SUB_PARA_INVALID             = 0x07,              /*!< the parameter is invalid */
    ESP_AT_SUB_PARA_PARSE_FAIL          = 0x08,              /*!< parse parameter fail */
    ESP_AT_SUB_UNSUPPORT_CMD            = 0x09,              /*!< the command is not supported */
    ESP_AT_SUB_CMD_EXEC_FAIL            = 0x0A,              /*!< the command execution failed */
    ESP_AT_SUB_CMD_PROCESSING           = 0x0B,              /*!< processing of previous command is in progress */
    ESP_AT_SUB_CMD_OP_ERROR             = 0x0C,              /*!< the command operation type is error */
} esp_at_error_code;

#define ESP_AT_ERROR_NO(subcategory,extension)  \
        ((ESP_AT_MODULE_NUM << 24) | ((subcategory) << 16) | (extension))

#define ESP_AT_CMD_ERROR_OK                           ESP_AT_ERROR_NO(ESP_AT_SUB_OK,0x00)                                       /*!< No Error */
#define ESP_AT_CMD_ERROR_NON_FINISH                   ESP_AT_ERROR_NO(ESP_AT_SUB_NO_TERMINATOR,0x00)                            /*!< terminator character not found ("\r\n" expected) */
#define ESP_AT_CMD_ERROR_NOT_FOUND_AT                 ESP_AT_ERROR_NO(ESP_AT_SUB_NO_AT,0x00)                                    /*!< Starting "AT" not found (or at, At or aT entered) */
#define ESP_AT_CMD_ERROR_PARA_LENGTH(which_para)      ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_LENGTH_MISMATCH,which_para)               /*!< parameter length mismatch */
#define ESP_AT_CMD_ERROR_PARA_TYPE(which_para)        ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_TYPE_MISMATCH,which_para)                 /*!< parameter type mismatch */
#define ESP_AT_CMD_ERROR_PARA_NUM(need,given)         ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_NUM_MISMATCH,(((need) << 8) | (given)))   /*!< parameter number mismatch */
#define ESP_AT_CMD_ERROR_PARA_INVALID(which_para)     ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_INVALID,which_para)                       /*!< the parameter is invalid */
#define ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(which_para)  ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_PARSE_FAIL,which_para)                    /*!< parse parameter fail */
#define ESP_AT_CMD_ERROR_CMD_UNSUPPORT                ESP_AT_ERROR_NO(ESP_AT_SUB_UNSUPPORT_CMD,0x00)                            /*!< the command is not supported */
#define ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(result)        ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_EXEC_FAIL,result)                          /*!< the command execution failed */
#define ESP_AT_CMD_ERROR_CMD_PROCESSING               ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_PROCESSING,0x00)                           /*!< processing of previous command is in progress */
#define ESP_AT_CMD_ERROR_CMD_OP_ERROR                 ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_OP_ERROR,0x00)                             /*!< the command operation type is error */

/**
 * @brief the result of AT parse
 *
 */
typedef enum {
    ESP_AT_PARA_PARSE_RESULT_FAIL = -1,           /*!< parse fail,Maybe the type of parameter is mismatched,or out of range */
    ESP_AT_PARA_PARSE_RESULT_OK = 0,              /*!< Successful */
    ESP_AT_PARA_PARSE_RESULT_OMITTED,             /*!< the parameter is OMITTED. */
} esp_at_para_parse_result_type;

/**
 * @brief the result code of AT command processing
 *
 */
typedef enum {
    ESP_AT_RESULT_CODE_OK           = 0x00,       /*!< "OK" */
    ESP_AT_RESULT_CODE_ERROR        = 0x01,       /*!< "ERROR" */
    ESP_AT_RESULT_CODE_FAIL         = 0x02,       /*!< "ERROR" */
    ESP_AT_RESULT_CODE_SEND_OK      = 0x03,       /*!< "SEND OK" */
    ESP_AT_RESULT_CODE_SEND_FAIL    = 0x04,       /*!< "SEND FAIL" */
    ESP_AT_RESULT_CODE_IGNORE       = 0x05,       /*!< response nothing, just change internal status */
    ESP_AT_RESULT_CODE_PROCESS_DONE = 0x06,       /*!< response nothing, just change internal status */
    ESP_AT_RESULT_CODE_OK_AND_INPUT_PROMPT = 0x07,    // "OK" and ">"
    ESP_AT_RESULT_CODE_MAX
} esp_at_result_code_string_index;

/**
 * @brief This function should be called only once.
 *
 * @param custom_version version information by custom
 */
void esp_at_module_init(const uint8_t *custom_version);

/**
 * @brief Parse digit parameter from command string.
 *
 * @param para_index the index of parameter
 * @param value the value parsed
 *
 * @return
 *  - ESP_AT_PARA_PARSE_RESULT_OK : succeed
 *  - ESP_AT_PARA_PARSE_RESULT_FAIL : fail
 *  - ESP_AT_PARA_PARSE_RESULT_OMITTED : this parameter is OMITTED
 */
esp_at_para_parse_result_type esp_at_get_para_as_digit(int32_t para_index, int32_t *value);

/**
 * @brief Parse float parameter from command string.
 *
 * @param para_index the index of parameter
 * @param value the value parsed
 *
 * @return
 *  - ESP_AT_PARA_PARSE_RESULT_OK : succeed
 *  - ESP_AT_PARA_PARSE_RESULT_FAIL : fail
 *  - ESP_AT_PARA_PARSE_RESULT_OMITTED : this parameter is OMITTED
 */
esp_at_para_parse_result_type esp_at_get_para_as_float(int32_t para_index, float *value);

/**
 * @brief Parse string parameter from command string.
 *
 * @param para_index the index of parameter
 * @param result the pointer that point to the result.
 *
 * @return
 *  - ESP_AT_PARA_PARSE_RESULT_OK : succeed
 *  - ESP_AT_PARA_PARSE_RESULT_FAIL : fail
 *  - ESP_AT_PARA_PARSE_RESULT_OMITTED : this parameter is OMITTED
 */
esp_at_para_parse_result_type esp_at_get_para_as_str(int32_t para_index, uint8_t **result);

/**
 * @brief Calling the esp_at_port_recv_data_notify_from_isr to notify at module that at port received data.
 *        When received this notice,at task will get data by calling get_data_length and read_data in esp_at_device_ops.
 *        This function MUST be used in isr.
 *
 * @param len data length
 *
 */
void esp_at_port_recv_data_notify_from_isr(int32_t len);

/**
 * @brief Calling the esp_at_port_recv_data_notify to notify at module that at port received data.
 *        When received this notice,at task will get data by calling get_data_length and read_data in esp_at_device_ops.
 *        This function MUST NOT be used in isr.
 *
 * @param len data length
 * @param msec timeout time,The unit is millisecond. It waits forever,if msec is portMAX_DELAY.
 *
 * @return
 *  - true : succeed
 *  - false : fail
 */
bool esp_at_port_recv_data_notify(int32_t len, uint32_t msec);

/**
 * @brief terminal transparent transmit mode,This function MUST be used in isr.
 *
 */
void esp_at_transmit_terminal_from_isr(void);

/**
 * @brief   terminal transparent transmit mode,This function MUST NOT be used in isr.
 *
 */
void esp_at_transmit_terminal(void);

/**
 * @brief regist at command set, which defined by custom,
 *
 * @param custom_at_cmd_array at command set
 * @param cmd_num command number
 *
 */
bool esp_at_custom_cmd_array_regist(const esp_at_cmd_struct *custom_at_cmd_array, uint32_t cmd_num);

/**
 * @brief regist device operate functions set,
 *
 * @param ops device operate functions set
 *
 */
void esp_at_device_ops_regist(esp_at_device_ops_struct* ops);

/*
 *  @brief regist custom callback about socket status,
 *
 *  @param link_id the link id
 *  @param ops custom operate functions set
 *
 *  Note: Make sure this API call after esp_at_module_init.
 */
bool esp_at_custom_net_ops_regist(int32_t link_id, esp_at_custom_net_ops_struct* ops);

/*
 *  @brief regist custom callback about ble status,
 *
 *  @param ops custom operate functions set
 *
 *  Note: Make sure this API call after esp_at_module_init.
 */
bool esp_at_custom_ble_ops_regist(int32_t conn_index, esp_at_custom_ble_ops_struct* ops);

/**
 * @brief regist custom operate functions set interacting with AT,
 *
 * @param ops custom operate functions set
 *
 */
void esp_at_custom_ops_regist(esp_at_custom_ops_struct* ops);

/**
 * @brief get at module version number,
 *
 * @return at version bit31~bit24: at main version
 *                    bit23~bit16: at sub version
 *                    bit15~bit8 : at test version
 *                    bit7~bit0 : at custom version
 */
uint32_t esp_at_get_version(void);

/**
 * @brief response AT process result,
 *
 * @param result_code see esp_at_result_code_string_index
 *
 */
void esp_at_response_result(uint8_t result_code);

/**
 * @brief write data into device,
 *
 * @param data data buffer to be written
 * @param len data length
 *
 * @return
 *  - >= 0 : the real length of the data written
 *  - others : fail.
 */
int32_t  esp_at_port_write_data(uint8_t *data, int32_t len);

/**
 * @brief call pre_active_write_data_callback() first and then write data into device,
 *
 * @param data data buffer to be written
 * @param len data length
 *
 * @return
 *  - >= 0 : the real length of the data written
 *  - others : fail.
 */
int32_t  esp_at_port_active_write_data(uint8_t *data, int32_t len);

/**
 * @brief   read data from device,
 *
 * @param data data buffer
 * @param len data length
 *
 * @return
 *  - >= 0 : the real length of the data read from device
 *  - others : fail
 */
int32_t esp_at_port_read_data(uint8_t*data, int32_t len);

/**
 * @brief wait for transmitting data completely to peer device,
 *
 * @param timeout_msec timeout time,The unit is millisecond.
 *
 * @return
 *  - true : succeed,transmit data completely
 *  - false : fail
 */
bool esp_at_port_wait_write_complete(int32_t timeout_msec);

/**
 * @brief get the length of the data received,
 *
 * @return
 *  - >= 0   : the length of the data received
 *  - others : fail
 */
int32_t esp_at_port_get_data_length(void);

/**
 * @brief Set AT command terminator, by default, the terminator is "\r\n"
 * You can change it by calling this function, but it just supports one character now.
 * @param terminator: the line terminator
 *
 * @return
 *  - true : succeed,transmit data completely
 *  - false : fail
 */
bool esp_at_custom_cmd_line_terminator_set(uint8_t* terminator);

/**
 * @brief Get AT command line terminator,by default, the return string is "\r\n"
 *
 * @return the command line terminator
 */
uint8_t* esp_at_custom_cmd_line_terminator_get(void);

/**
 * @brief Find the partition which is defined in at_customize.csv
 * @param type: the type of the partition
 * @param subtype: the subtype of the partition
 * @param label: Partition label
 *
 * @return pointer to esp_partition_t structure, or NULL if no partition is found.
 *         This pointer is valid for the lifetime of the application
 */
const esp_partition_t* esp_at_custom_partition_find(esp_partition_type_t type, esp_partition_subtype_t subtype, const char* label);

/**
 * @brief Set AT core as specific status, it will call callback if receiving data.
 * for example:
 *
  * @code{c}
 * static void wait_data_callback (void)
 * {
 *     xSemaphoreGive(sync_sema);
 * }
 *
 * void process_task(void* para)
 * {
 *     vSemaphoreCreateBinary(sync_sema);
 *     xSemaphoreTake(sync_sema,portMAX_DELAY);
 *     esp_at_port_write_data((uint8_t *)">",strlen(">"));
 *     esp_at_port_enter_specific(wait_data_callback);
 *     while(xSemaphoreTake(sync_sema,portMAX_DELAY)) {
 *         len = esp_at_port_read_data(data, data_len);
 *         // TODO:
 *     }
 * }
 * @endcode
 * @param callback: which will be called when received data from AT port
 *
 */
void esp_at_port_enter_specific(esp_at_port_specific_callback_t callback);

/**
 * @brief Exit AT core as specific status.
 *
 */
void esp_at_port_exit_specific(void);

/**
 * @brief Get current AT command name.
 *
 */
const uint8_t* esp_at_get_current_cmd_name(void);

/**
 * @brief Get the version of the AT core library
 *
 * @param buffer: buffer to store the version string
 * @param size: size of the buffer
 *
 * @return
 * - > 0 : the real length of the version string
 * - others : fail
 */
int32_t esp_at_get_core_version(char *buffer, uint32_t size);

void at_handle_result_code(esp_at_result_code_string_index code, void *pbuf);
