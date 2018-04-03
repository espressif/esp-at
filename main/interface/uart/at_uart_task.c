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

#include "esp_system.h"
#include "at_upgrade.h"

#include "driver/uart.h"

#ifdef CONFIG_AT_BLE_COMMAND_SUPPORT
#include "esp_bt.h"
#endif

typedef struct {
    int32_t baudrate;
    int8_t data_bits;
    int8_t stop_bits;
    int8_t parity;
    int8_t flow_control;
} at_nvm_uart_config_struct; 

static QueueHandle_t esp_at_uart_queue = NULL;
static bool at_default_flag = false;


static bool at_nvm_uart_config_set (at_nvm_uart_config_struct *uart_config);
static bool at_nvm_uart_config_get (at_nvm_uart_config_struct *uart_config);


static int32_t at_port_write_data(uint8_t*data,int32_t len)
{
    uint32_t length = 0;

    length = uart_write_bytes(CONFIG_AT_UART_PORT,(char*)data,len);
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
            if (ESP_OK != uart_get_buffered_data_len(CONFIG_AT_UART_PORT,&size)) {
                return -1;
            }
            len = size;
        }

        if (len == 0) {
            return 0;
        }

        data = (uint8_t *)malloc(len);
        if (data) {
            len = uart_read_bytes(CONFIG_AT_UART_PORT,data,len,ticks_to_wait);
            free(data);
            return len;
        } else {
            return -1;
        }
    } else {
        return uart_read_bytes(CONFIG_AT_UART_PORT,buf,len,ticks_to_wait);
    }
}

static int32_t at_port_get_data_length (void)
{
    size_t size = 0;
    int pattern_pos = 0;

    if (ESP_OK == uart_get_buffered_data_len(CONFIG_AT_UART_PORT,&size)) {
        pattern_pos = uart_pattern_get_pos(CONFIG_AT_UART_PORT);
        if (pattern_pos >= 0) {
            size = pattern_pos;
        }
        return size;
    } else {
        return 0;
    }
}

static bool at_port_wait_write_complete (int32_t timeout_msec)
{
    if (ESP_OK == uart_wait_tx_done(CONFIG_AT_UART_PORT, timeout_msec*portTICK_PERIOD_MS)) {
        return true;
    }

    return false;
}

static void uart_task(void *pvParameters)
{
    uart_event_t event;
    int pattern_pos = -1;
    uint8_t *data = NULL;
    uint32_t data_len = 0;
    BaseType_t retry_flag = pdFALSE;

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
            case UART_PATTERN_DET:
                pattern_pos = uart_pattern_pop_pos(CONFIG_AT_UART_PORT);
                if (pattern_pos >= 0) {
                    data = (uint8_t *)malloc(pattern_pos + 3);
                    uart_read_bytes(CONFIG_AT_UART_PORT,data,pattern_pos + 3,0);
                    free(data);
                    data = NULL;
                }
                esp_at_transmit_terminal();
                break;
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
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_RTS,
        .rx_flow_ctrl_thresh = 122,
    };

    memset(&uart_nvm_config,0x0,sizeof(uart_nvm_config));
    
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
        uart_nvm_config.baudrate = 115200;
        uart_nvm_config.data_bits = UART_DATA_8_BITS;
        uart_nvm_config.flow_control = UART_HW_FLOWCTRL_RTS;
        uart_nvm_config.parity = UART_PARITY_DISABLE;
        uart_nvm_config.stop_bits = UART_STOP_BITS_1;
        at_nvm_uart_config_set(&uart_nvm_config);
    }
    //Set UART parameters
    uart_param_config(CONFIG_AT_UART_PORT, &uart_config);
    //Set UART pins,(-1: default pin, no change.)
    uart_set_pin(CONFIG_AT_UART_PORT, CONFIG_AT_UART_PORT_TX_PIN, CONFIG_AT_UART_PORT_RX_PIN, CONFIG_AT_UART_PORT_RTS_PIN, CONFIG_AT_UART_PORT_CTS_PIN);
    //Install UART driver, and get the queue.
    uart_driver_install(CONFIG_AT_UART_PORT, 2048, 8192, 30,&esp_at_uart_queue,0);
    xTaskCreate(uart_task, "uTask", 2048, (void*)CONFIG_AT_UART_PORT, 1, NULL);
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
    if (value == 0) {
        uart_config.parity = UART_PARITY_DISABLE;
    } else if (value == 1) {
        uart_config.parity = UART_PARITY_ODD;
    } else if (value == 2) {
        uart_config.parity = UART_PARITY_EVEN;
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    uart_config.parity = value;

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

    uart_wait_tx_done(CONFIG_AT_UART_PORT,portMAX_DELAY);
    uart_set_baudrate(CONFIG_AT_UART_PORT,uart_config.baudrate);
    uart_set_word_length(CONFIG_AT_UART_PORT,uart_config.data_bits);
    uart_set_stop_bits(CONFIG_AT_UART_PORT,uart_config.stop_bits);
    uart_set_parity(CONFIG_AT_UART_PORT,uart_config.parity);
    uart_set_hw_flow_ctrl(CONFIG_AT_UART_PORT,uart_config.flow_control,120);

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

    uart_get_baudrate(CONFIG_AT_UART_PORT,&baudrate);
    uart_get_word_length(CONFIG_AT_UART_PORT,&data_bits);
    uart_get_stop_bits(CONFIG_AT_UART_PORT,&stop_bits);
    uart_get_parity(CONFIG_AT_UART_PORT,&parity);
    uart_get_hw_flow_ctrl(CONFIG_AT_UART_PORT,&flow_control);

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


static uint8_t at_exeCmdCipupdate(uint8_t *cmd_name)//add get station ip and ap ip
{

    if (esp_at_upgrade_process(ESP_AT_OTA_MODE_NORMAL,NULL)) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(portMAX_DELAY);
        esp_restart();
        for(;;){
        }
    }

    return ESP_AT_RESULT_CODE_ERROR;
}

static uint8_t at_setupCmdCipupdate(uint8_t para_num)
{
    int32_t ota_mode = 0;
    int32_t cnt = 0;
    uint8_t* version = NULL;

    if (esp_at_get_para_as_digit (cnt++,&ota_mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt < para_num) {
        if(esp_at_get_para_as_str (cnt++,&version) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_upgrade_process(ota_mode,version)) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        esp_at_port_wait_write_complete(portMAX_DELAY);
        esp_restart();
        for(;;){
        }
    }

    return ESP_AT_RESULT_CODE_ERROR;
}

static esp_at_cmd_struct at_custom_cmd[] = {
    {"+UART", NULL, at_queryCmdUart, at_setupCmdUartDef, NULL},
    {"+UART_CUR", NULL, at_queryCmdUart, at_setupCmdUart, NULL},
    {"+UART_DEF", NULL, at_queryCmdUartDef, at_setupCmdUartDef, NULL},
    {"+CIUPDATE", NULL, NULL, at_setupCmdCipupdate, at_exeCmdCipupdate},
};

void at_status_callback (esp_at_status_type status)
{
    switch (status) {
    case ESP_AT_STATUS_NORMAL:
        uart_disable_pattern_det_intr(CONFIG_AT_UART_PORT);
        break;
    case ESP_AT_STATUS_TRANSMIT:
        uart_enable_pattern_det_intr(CONFIG_AT_UART_PORT, '+', 3, ((APB_CLK_FREQ*20)/1000),((APB_CLK_FREQ*20)/1000), ((APB_CLK_FREQ*20)/1000));
        break;
    }
}

void at_pre_deepsleep_callback (void)
{
    /* Do something before deep sleep
     * Set uart pin for power saving
    */
    gpio_set_direction(CONFIG_AT_UART_PORT_TX_PIN,0);
    gpio_set_direction(CONFIG_AT_UART_PORT_RX_PIN,0);
    gpio_set_direction(CONFIG_AT_UART_PORT_RTS_PIN,0);
    gpio_set_direction(CONFIG_AT_UART_PORT_CTS_PIN,0);
}

void at_pre_restart_callback (void)
{
    /* Do something before restart
    */
    uart_disable_rx_intr(CONFIG_AT_UART_PORT);
    esp_at_port_wait_write_complete(portMAX_DELAY);
}

void at_task_init(void)
{
    uint8_t *version = (uint8_t *)malloc(192);
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

#ifdef CONFIG_AT_COMMAND_TERMINATOR
    uint8_t cmd_terminator[2] = {CONFIG_AT_COMMAND_TERMINATOR,0};
#endif

    nvs_flash_init();
    at_uart_init();

    sprintf((char*)version, "compile time:%s %s\r\n", __DATE__, __TIME__);
#ifdef CONFIG_ESP_AT_FW_VERSION
    if ((strlen(CONFIG_ESP_AT_FW_VERSION) > 0) && (strlen(CONFIG_ESP_AT_FW_VERSION) <= 128)){
        printf("%s\r\n", CONFIG_ESP_AT_FW_VERSION);
        strcat((char*)version, CONFIG_ESP_AT_FW_VERSION);
    }
#endif
    esp_at_device_ops_regist (&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
    esp_at_module_init (CONFIG_LWIP_MAX_SOCKETS - 1, version);  // reserved one for server
    free(version);

#ifdef CONFIG_AT_BASE_COMMAND_SUPPORT
    if(esp_at_base_cmd_regist() == false) {
        printf("regist base cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_WIFI_COMMAND_SUPPORT
    if(esp_at_wifi_cmd_regist() == false) {
        printf("regist wifi cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_NET_COMMAND_SUPPORT
    if(esp_at_net_cmd_regist() == false) {
        printf("regist net cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_BLE_COMMAND_SUPPORT
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if(esp_at_ble_cmd_regist() == false) {
        printf("regist ble cmd fail\r\n");
    }
#endif

#ifdef CONFIG_AT_FS_COMMAND_SUPPORT
    if(esp_at_fs_cmd_regist() == false) {
        printf("regist ble cmd fail\r\n");
    }
#endif

    esp_at_custom_cmd_array_regist (at_custom_cmd, sizeof(at_custom_cmd)/sizeof(at_custom_cmd[0]));
    esp_at_port_write_data((uint8_t *)"\r\nready\r\n",strlen("\r\nready\r\n"));
#ifdef CONFIG_AT_COMMAND_TERMINATOR
    esp_at_custom_cmd_line_terminator_set((uint8_t*)&cmd_terminator);
#endif
}
