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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "esp_at.h"
#include "nvs.h"
#include "nvs_flash.h"

#ifdef CONFIG_AT_BASE_ON_UART
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "at_interface.h"

#include "at_default_config.h"

typedef struct {
    int32_t baudrate;
    int8_t data_bits;
    int8_t stop_bits;
    int8_t parity;
    int8_t flow_control;
} at_nvm_uart_config_struct; 

static const uint8_t esp_at_uart_parity_table[] = {UART_PARITY_DISABLE, UART_PARITY_ODD, UART_PARITY_EVEN};

static QueueHandle_t esp_at_uart_queue = NULL;
static bool at_default_flag = false;

#if defined(CONFIG_IDF_TARGET_ESP32)
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          17
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          16
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         15
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         14
#ifndef CONFIG_AT_UART_PORT
#define CONFIG_AT_UART_PORT                         UART_NUM_1
#endif
#elif defined(CONFIG_IDF_TARGET_ESP8266)
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          15
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          13
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         3
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         1
#ifndef CONFIG_AT_UART_PORT
#define CONFIG_AT_UART_PORT                         UART_NUM_0
#endif
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
#define CONFIG_AT_UART_PORT_TX_PIN_DEFAULT          17
#define CONFIG_AT_UART_PORT_RX_PIN_DEFAULT          18
#define CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT         20
#define CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT         19
#ifndef CONFIG_AT_UART_PORT
#define CONFIG_AT_UART_PORT                         UART_NUM_1
#endif
#endif

static uart_port_t esp_at_uart_port = CONFIG_AT_UART_PORT;

static bool at_nvm_uart_config_set (at_nvm_uart_config_struct *uart_config);
static bool at_nvm_uart_config_get (at_nvm_uart_config_struct *uart_config);


static int32_t at_port_write_data(uint8_t*data,int32_t len)
{
    uint32_t length = 0;

    length = uart_write_bytes(esp_at_uart_port,(char*)data,len);
    return length;
}

static int32_t at_port_read_data(uint8_t*buf,int32_t len)
{
    TickType_t ticks_to_wait = portTICK_RATE_MS;
    uint8_t *data = NULL;
    size_t size = 0;

    if (len == 0) {
        return 0;
    }

    if (buf == NULL) {
        if (len == -1) {
            if (ESP_OK != uart_get_buffered_data_len(esp_at_uart_port,&size)) {
                return -1;
            }
            len = size;
        }

        if (len == 0) {
            return 0;
        }

        data = (uint8_t *)malloc(len);
        if (data) {
            len = uart_read_bytes(esp_at_uart_port,data,len,ticks_to_wait);
            free(data);
            return len;
        } else {
            return -1;
        }
    } else {
        return uart_read_bytes(esp_at_uart_port,buf,len,ticks_to_wait);
    }
}

static int32_t at_port_get_data_length (void)
{
    size_t size = 0;
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    int pattern_pos = 0;
#endif

    if (ESP_OK == uart_get_buffered_data_len(esp_at_uart_port,&size)) {
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
        pattern_pos = uart_pattern_get_pos(esp_at_uart_port);
        if (pattern_pos >= 0) {
            size = pattern_pos;
        }
#endif
        return size;
    } else {
        return 0;
    }
}

static bool at_port_wait_write_complete (int32_t timeout_msec)
{
    if (ESP_OK == uart_wait_tx_done(esp_at_uart_port, timeout_msec*portTICK_PERIOD_MS)) {
        return true;
    }

    return false;
}

static void uart_task(void *pvParameters)
{
    uart_event_t event;
    uint32_t data_len = 0;
    BaseType_t retry_flag = pdFALSE;
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    int pattern_pos = -1;
    uint8_t *data = NULL;
#endif

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
retry:
            switch (event.type) {
            //Event of UART receving data
            case UART_DATA:
            case UART_BUFFER_FULL:
                data_len += event.size;
                // we can put all data together to process
                retry_flag = pdFALSE;
                while (xQueueReceive(esp_at_uart_queue, (void * )&event, (portTickType)0) == pdTRUE) {
                    if ((event.type == UART_DATA) || (event.type == UART_BUFFER_FULL)) {
                        data_len += event.size;
                    } else {
                        retry_flag = pdTRUE;
                        break;
                    }
                }
                esp_at_port_recv_data_notify (data_len, portMAX_DELAY);
                data_len = 0;

                if (retry_flag == pdTRUE) {
                    goto retry;
                }
                break;
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
            case UART_PATTERN_DET:
                pattern_pos = uart_pattern_pop_pos(esp_at_uart_port);
                if (pattern_pos >= 0) {
                    data = (uint8_t *)malloc(pattern_pos + 3);
                    uart_read_bytes(esp_at_uart_port,data,pattern_pos + 3,0);
                    free(data);
                    data = NULL;
                } else {
                    uart_flush_input(esp_at_uart_port);
                    xQueueReset(esp_at_uart_queue);
                }
                esp_at_transmit_terminal();
                break;
#endif
            //Others
            default:
                break;
            }
        }
    }
    vTaskDelete(NULL);
}
static void at_uart_init(void)
{
    at_nvm_uart_config_struct uart_nvm_config;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = CONFIG_AT_UART_DEFAULT_DATABITS - 5,
        .parity = esp_at_uart_parity_table[CONFIG_AT_UART_DEFAULT_PARITY_BITS],
        .stop_bits = CONFIG_AT_UART_DEFAULT_STOPBITS,
        .flow_ctrl = CONFIG_AT_UART_DEFAULT_FLOW_CONTROL,
        .rx_flow_ctrl_thresh = 122,
    };

    int32_t tx_pin = CONFIG_AT_UART_PORT_TX_PIN_DEFAULT;	
    int32_t rx_pin = CONFIG_AT_UART_PORT_RX_PIN_DEFAULT;
    int32_t cts_pin = CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT;
    int32_t rts_pin = CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT;

    char* data = NULL;
    const esp_partition_t * partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");

    memset(&uart_nvm_config,0x0,sizeof(uart_nvm_config));

    if (partition) {	
        data = (char*)malloc(ESP_AT_FACTORY_PARAMETER_SIZE); // Notes
        assert(data != NULL);
        if (esp_partition_read(partition, 0, data, ESP_AT_FACTORY_PARAMETER_SIZE) != ESP_OK){
            free(data);
            data = NULL;
        }
    }

    if (at_nvm_uart_config_get(&uart_nvm_config)) {
        if ((uart_nvm_config.baudrate >= 80) && (uart_nvm_config.baudrate <= 5000000)) {
            uart_config.baud_rate = uart_nvm_config.baudrate;
        }

        if ((uart_nvm_config.data_bits >= UART_DATA_5_BITS) && (uart_nvm_config.data_bits <= UART_DATA_8_BITS)) {
            uart_config.data_bits = uart_nvm_config.data_bits;
        }

        if ((uart_nvm_config.stop_bits >= UART_STOP_BITS_1) && (uart_nvm_config.stop_bits <= UART_STOP_BITS_2)) {
            uart_config.stop_bits = uart_nvm_config.stop_bits;
        }

        if ((uart_nvm_config.parity == UART_PARITY_DISABLE) 
            || (uart_nvm_config.parity == UART_PARITY_ODD)
            || (uart_nvm_config.parity == UART_PARITY_EVEN)) {
            uart_config.parity = uart_nvm_config.parity;
        }

        if ((uart_nvm_config.flow_control >= UART_HW_FLOWCTRL_DISABLE) && (uart_nvm_config.flow_control <= UART_HW_FLOWCTRL_CTS_RTS)) {
            uart_config.flow_ctrl = uart_nvm_config.flow_control;
        }
    } else {
        if (data) {
            if ((data[0] == 0xFC) && (data[1] == 0xFC)) { // check magic flag, should be 0xfc 0xfc
                if ((data[12] != 0xFF) || (data[13] != 0xFF) || (data[14] != 0xFF) || (data[15] != 0xFF)) {
                    uart_config.baud_rate = *(int32_t*)&data[12];
                }
            }
        }
        uart_nvm_config.baudrate = uart_config.baud_rate;
        uart_nvm_config.data_bits = uart_config.data_bits;
        uart_nvm_config.flow_control = uart_config.flow_ctrl;
        uart_nvm_config.parity = uart_config.parity;
        uart_nvm_config.stop_bits = uart_config.stop_bits;
        at_nvm_uart_config_set(&uart_nvm_config);
    }

    if (data) {
        if ((data[0] == 0xFC) && (data[1] == 0xFC)) { // check magic flag, should be 0xfc 0xfc
            if (data[5] != 0xFF) {
#if defined(CONFIG_IDF_TARGET_ESP32)
                assert((data[5] == 0) || (data[5] == 1) || (data[5] == 2));
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
                assert((data[5] == 0) || (data[5] == 1));
#elif defined(CONFIG_IDF_TARGET_ESP8266)
                assert(data[5] == 0);
#endif
                esp_at_uart_port = data[5];
            }
            if ((data[16] != 0xFF) && (data[17] != 0xFF)) {
                tx_pin = data[16];
                rx_pin = data[17];
            }

            if (data[18] != 0xFF) {
                cts_pin = data[18];
            } else {
                cts_pin = -1;
            }

            if (data[19] != 0xFF) {
                rts_pin = data[19];
            } else {
                rts_pin = -1;
            }

            if (data[20] != 0xFF) {
                gpio_set_direction(data[20], GPIO_MODE_OUTPUT);
                gpio_set_level(data[20], 1);
            }

            if (data[21] != 0xFF) {
                gpio_set_direction(data[21], GPIO_MODE_OUTPUT);
                gpio_set_level(data[21], 1);
            }
        }
        free(data);
        data = NULL;
    }
    //Set UART parameters
    uart_param_config(esp_at_uart_port, &uart_config);

#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    //Set UART pins,(-1: default pin, no change.)
    uart_set_pin(esp_at_uart_port, tx_pin, rx_pin, rts_pin, cts_pin);
    //Install UART driver, and get the queue.
    uart_driver_install(esp_at_uart_port, 2048, 8192, 30,&esp_at_uart_queue,0);

#elif defined(CONFIG_IDF_TARGET_ESP8266)
    //Install UART driver, and get the queue.
    uart_driver_install(esp_at_uart_port, 1024, 2048, 10,&esp_at_uart_queue, 0);
    if ((tx_pin == 15) && (rx_pin == 13)) { // sgit wap 
        uart_enable_swap();
        assert((cts_pin == -1) || (cts_pin == 3));
        assert((rts_pin == -1) || (rts_pin == 1));
    } else {
        assert((tx_pin == 1) && (rx_pin == 3));
    }
#endif
    xTaskCreate(uart_task, "uTask", 2048, (void*)esp_at_uart_port, 1, NULL);
}

static bool at_nvm_uart_config_set (at_nvm_uart_config_struct *uart_config)
{
    nvs_handle handle;
    if (uart_config == NULL) {
        return false;
    }

    if (nvs_open("UART", NVS_READWRITE, &handle) == ESP_OK) {
        if (nvs_set_i32(handle, "rate", uart_config->baudrate) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_set_i8(handle, "databits", uart_config->data_bits) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_set_i8(handle, "stopbits", uart_config->stop_bits) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_set_i8(handle, "parity", uart_config->parity) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_set_i8(handle, "flow_ctrl", uart_config->flow_control) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
    } else {
        return false;
    }
    nvs_close(handle);
    return true;
}

static bool at_nvm_uart_config_get (at_nvm_uart_config_struct *uart_config)
{
    nvs_handle handle;
    if (uart_config == NULL) {
        return false;
    }

    if (nvs_open("UART", NVS_READONLY, &handle) == ESP_OK) {
        if (nvs_get_i32(handle, "rate", &uart_config->baudrate) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_get_i8(handle, "databits", &uart_config->data_bits) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_get_i8(handle, "stopbits", &uart_config->stop_bits) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_get_i8(handle, "parity", &uart_config->parity) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
        if (nvs_get_i8(handle, "flow_ctrl", &uart_config->flow_control) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
    } else {
        return false;
    }
    nvs_close(handle);

    return true;
}

static uint8_t at_setupCmdUart(uint8_t para_num)
{
    int32_t value = 0;
    int32_t cnt = 0;

    at_nvm_uart_config_struct uart_config;

    memset(&uart_config,0x0,sizeof(uart_config));
    if (para_num != 5) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit (cnt++,&value) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((value < 80) || (value > 5000000)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    uart_config.baudrate = value;

    if (esp_at_get_para_as_digit (cnt++,&value) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((value < 5) || (value > 8)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    uart_config.data_bits = value - 5;

    if (esp_at_get_para_as_digit (cnt++,&value) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((value < 1) || (value > 3)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    uart_config.stop_bits = value;

    if (esp_at_get_para_as_digit (cnt++,&value) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((value >= 0) && (value <= 2)) {
        uart_config.parity = esp_at_uart_parity_table[value];
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit (cnt++,&value) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if ((value < 0) || (value > 3)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    uart_config.flow_control = value;

    if (at_default_flag) {
        if (at_nvm_uart_config_set(&uart_config) == false) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }
    esp_at_response_result(ESP_AT_RESULT_CODE_OK);

    uart_wait_tx_done(esp_at_uart_port,portMAX_DELAY);
    uart_set_baudrate(esp_at_uart_port,uart_config.baudrate);
    uart_set_word_length(esp_at_uart_port,uart_config.data_bits);
    uart_set_stop_bits(esp_at_uart_port,uart_config.stop_bits);
    uart_set_parity(esp_at_uart_port,uart_config.parity);
    uart_set_hw_flow_ctrl(esp_at_uart_port,uart_config.flow_control,120);

    return ESP_AT_RESULT_CODE_PROCESS_DONE;
}

static uint8_t at_setupCmdUartDef(uint8_t para_num)
{
    uint8_t ret = ESP_AT_RESULT_CODE_ERROR;
    at_default_flag = true;
    ret = at_setupCmdUart(para_num);
    at_default_flag = false;
    
    return ret;
}

static uint8_t at_queryCmdUart (uint8_t *cmd_name)
{
    uint32_t baudrate = 0;
    uart_word_length_t data_bits = UART_DATA_8_BITS;
    uart_stop_bits_t stop_bits = UART_STOP_BITS_1;
    uart_parity_t parity = UART_PARITY_DISABLE;
    uart_hw_flowcontrol_t flow_control = UART_HW_FLOWCTRL_DISABLE;

    uint8_t buffer[64];

    uart_get_baudrate(esp_at_uart_port,&baudrate);
    uart_get_word_length(esp_at_uart_port,&data_bits);
    uart_get_stop_bits(esp_at_uart_port,&stop_bits);
    uart_get_parity(esp_at_uart_port,&parity);
    uart_get_hw_flow_ctrl(esp_at_uart_port,&flow_control);

    data_bits += 5;
    if (UART_PARITY_DISABLE == parity) {
        parity = 0;
    } else if (UART_PARITY_ODD == parity) {
        parity = 1;
    } else if (UART_PARITY_EVEN == parity) {
        parity = 2;
    } else {
        parity = 0xff;
    }

    snprintf((char*)buffer,sizeof(buffer) - 1,"%s:%d,%d,%d,%d,%d\r\n",cmd_name,baudrate,data_bits,stop_bits,parity,flow_control);

    esp_at_port_write_data(buffer,strlen((char*)buffer));

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_queryCmdUartDef (uint8_t *cmd_name)
{
    at_nvm_uart_config_struct uart_nvm_config;
    uint8_t buffer[64];
    memset(&uart_nvm_config,0x0,sizeof(uart_nvm_config));
    at_nvm_uart_config_get(&uart_nvm_config);

    uart_nvm_config.data_bits += 5;
    if (UART_PARITY_DISABLE == uart_nvm_config.parity) {
        uart_nvm_config.parity = 0;
    } else if (UART_PARITY_ODD == uart_nvm_config.parity) {
        uart_nvm_config.parity = 1;
    } else if (UART_PARITY_EVEN == uart_nvm_config.parity) {
        uart_nvm_config.parity = 2;
    } else {
        uart_nvm_config.parity = 0xff;
    }
    snprintf((char*)buffer,sizeof(buffer) - 1,"%s:%d,%d,%d,%d,%d\r\n",cmd_name,uart_nvm_config.baudrate,
        uart_nvm_config.data_bits,uart_nvm_config.stop_bits,uart_nvm_config.parity,uart_nvm_config.flow_control);

    esp_at_port_write_data(buffer,strlen((char*)buffer));
    return ESP_AT_RESULT_CODE_OK;
}

static esp_at_cmd_struct at_custom_cmd[] = {
    {"+UART", NULL, at_queryCmdUart, at_setupCmdUartDef, NULL},
    {"+UART_CUR", NULL, at_queryCmdUart, at_setupCmdUart, NULL},
    {"+UART_DEF", NULL, at_queryCmdUartDef, at_setupCmdUartDef, NULL},
};

void at_status_callback (esp_at_status_type status)
{
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    switch (status) {
    case ESP_AT_STATUS_NORMAL:
        uart_disable_pattern_det_intr(esp_at_uart_port);
        break;
    case ESP_AT_STATUS_TRANSMIT:
#if defined(CONFIG_IDF_TARGET_ESP32)
        uart_enable_pattern_det_intr(esp_at_uart_port, '+', 3, ((APB_CLK_FREQ*20)/1000),((APB_CLK_FREQ*20)/1000), ((APB_CLK_FREQ*20)/1000));
#endif
        break;
    }
#endif
}

void at_pre_deepsleep_callback (void)
{
    /* Do something before deep sleep
     * Set uart pin for power saving
    */
    gpio_set_direction(CONFIG_AT_UART_PORT_TX_PIN_DEFAULT,0);
    gpio_set_direction(CONFIG_AT_UART_PORT_RX_PIN_DEFAULT,0);
    gpio_set_direction(CONFIG_AT_UART_PORT_RTS_PIN_DEFAULT,0);
    gpio_set_direction(CONFIG_AT_UART_PORT_CTS_PIN_DEFAULT,0);
}

void at_pre_restart_callback (void)
{
    /* Do something before restart
    */
    uart_disable_rx_intr(esp_at_uart_port);
    esp_at_port_wait_write_complete(portMAX_DELAY);
}

void at_interface_init (void)
{
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_port_read_data,
        .write_data = at_port_write_data,
        .get_data_length = at_port_get_data_length,
        .wait_write_complete = at_port_wait_write_complete,
    };
    
    esp_at_custom_ops_struct esp_at_custom_ops = {
        .status_callback = at_status_callback,
        .pre_deepsleep_callback = at_pre_deepsleep_callback,
        .pre_restart_callback = at_pre_restart_callback,
    };

    at_uart_init();

    esp_at_device_ops_regist (&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
}


void at_custom_init(void)
{
    esp_at_custom_cmd_array_regist (at_custom_cmd, sizeof(at_custom_cmd)/sizeof(at_custom_cmd[0]));
    esp_at_port_write_data((uint8_t *)"\r\nready\r\n",strlen("\r\nready\r\n"));
}
#endif