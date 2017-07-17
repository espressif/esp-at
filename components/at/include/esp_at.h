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

#include <stdint.h>
#include <stdbool.h>

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
    int32_t (*read_data) (uint8_t *data, int32_t len);              /*!< read data from device */
    int32_t (*write_data)(uint8_t *data, int32_t len);              /*!< write data into device */

    int32_t (*get_data_length)(void);                               /*!< get the length of data received */
    bool (*wait_write_complete)(int32_t timeout_msec);              /*!< wait write finish */
} esp_at_device_ops_struct;

/**
 * @brief esp_at_status
 *  some custom function interacting with AT
 *
 */
typedef enum {
    ESP_AT_STATUS_NORMAL = 0x0,                /*!< Normal mode.Now mcu can send AT command */
    ESP_AT_STATUS_TRANSMIT,                    /*!< Transparent Transmition mode */

} esp_at_status_type;

/**
 * @brief esp_at_ops_struct
 *  some custom function interacting with AT
 *
 */
typedef struct {
    void (*status_callback) (esp_at_status_type status);              /*!< callback when AT status changes */

} esp_at_custom_ops_struct;


// error number
/**
 * @brief module number,Now just AT module
 *
 */
typedef enum {
    ESP_AT_MODULE_NUM = 0x01
} esp_at_module;

/**
 * @brief subcategory number
 *
 */
typedef enum {
    ESP_AT_SUB_OK                       = 0x00,              /*!< OK */
    ESP_AT_SUB_COMMON_ERROR             = 0x01,
    ESP_AT_SUB_NO_TERMINATOR            = 0x02,              /*!<  not end with "\r\n" */
    ESP_AT_SUB_NO_AT                    = 0x03,              /*!<  not found AT or at or At or aT */
    ESP_AT_SUB_PARA_LENGTH_MISMATCH     = 0x04,              /*!<  parameter length not match */
    ESP_AT_SUB_PARA_TYPE_MISMATCH       = 0x05,              /*!<  parameter length not match */
    ESP_AT_SUB_PARA_NUM_MISMATCH        = 0x06,              /*!<  parameter number not match */
    ESP_AT_SUB_PARA_INVALID             = 0x07,
    ESP_AT_SUB_PARA_PARSE_FAIL          = 0x08,              /*!<  parse parameter fail */
    ESP_AT_SUB_UNSUPPORT_CMD            = 0x09,
    ESP_AT_SUB_CMD_EXEC_FAIL            = 0x0A,
    ESP_AT_SUB_CMD_PROCESSING           = 0x0B,              /*!<  previous command is processing */
} esp_at_error_code;

#define ESP_AT_ERROR_NO(subcategory,extension)  \
        ((ESP_AT_MODULE_NUM << 24) | ((subcategory) << 16) | (extension))

#define ESP_AT_CMD_ERROR_OK                                   ESP_AT_ERROR_NO(ESP_AT_SUB_OK,0x00)
#define ESP_AT_CMD_ERROR_NON_FINISH                           ESP_AT_ERROR_NO(ESP_AT_SUB_NO_TERMINATOR,0x00)
#define ESP_AT_CMD_ERROR_NOT_FOUND_AT                         ESP_AT_ERROR_NO(ESP_AT_SUB_NO_AT,0x00)
#define ESP_AT_CMD_ERROR_PARA_LENGTH(which_para)              ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_LENGTH_MISMATCH,which_para)
#define ESP_AT_CMD_ERROR_PARA_TYPE(which_para)                ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_TYPE_MISMATCH,which_para)
#define ESP_AT_CMD_ERROR_PARA_NUM(need,given)                 ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_NUM_MISMATCH,(((need) << 8) | (given)))
#define ESP_AT_CMD_ERROR_PARA_INVALID(which_para)             ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_INVALID,which_para)
#define ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(which_para)          ESP_AT_ERROR_NO(ESP_AT_SUB_PARA_PARSE_FAIL,which_para)
#define ESP_AT_CMD_ERROR_CMD_UNSUPPORT                        ESP_AT_ERROR_NO(ESP_AT_SUB_UNSUPPORT_CMD,0x00)
#define ESP_AT_CMD_ERROR_CMD_EXEC_FAIL(result)                ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_EXEC_FAIL,result)
#define ESP_AT_CMD_ERROR_CMD_PROCESSING                       ESP_AT_ERROR_NO(ESP_AT_SUB_CMD_PROCESSING,0x00)

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
    ESP_AT_RESULT_CODE_IGNORE       = 0x05,       /*!< response nothing */
    ESP_AT_RESULT_CODE_PROCESS_DONE = 0x06,       /*!< response nothing */

    ESP_AT_RESULT_CODE_MAX
} esp_at_result_code_string_index;

/**
 * @brief This function should be called only once, before any other AT functions are called.
 *
 * @param netconn_max the maximum number of the link in the at module
 * @param custom_version version information by custom
 */
void esp_at_module_init(uint32_t netconn_max, const uint8_t *custom_version);

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
bool esp_at_custom_cmd_array_regist(esp_at_cmd_struct *custom_at_cmd_array, uint32_t cmd_num);

/**
 * @brief regist device operate functions set,
 *
 * @param ops device operate functions set
 *
 */
void esp_at_device_ops_regist(esp_at_device_ops_struct* ops);

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
 * @brief   read data from device,
 *
 * @param data data buffer
 * @param len data length
 *
 * @return 
 *  - >= 0 : the real length of the data read from device
 *  - others : fail
 */
int32_t esp_at_port_read_data(uint8_t*data,int32_t len);

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
 * @brief regist at base command set. If not,you can not use AT base command
 * @param NONE
 *
 */
bool esp_at_base_cmd_regist(void);
/**
 * @brief regist at wifi command set. If not,you can not use AT wifi command
 * @param NONE
 *
 */
bool esp_at_wifi_cmd_regist(void);
/**
 * @brief regist at net command set. If not,you can not use AT net command
 * @param NONE
 *
 */
bool esp_at_net_cmd_regist(void);
/**
 * @brief regist at ble command set. If not,you can not use AT ble command
 * @param NONE
 *
 */
bool esp_at_ble_cmd_regist(void);

/**
 * @brief regist at fs command set. If not,you can not use AT fs command
 * @param NONE
 *
 */
bool esp_at_fs_cmd_regist(void);
#endif

