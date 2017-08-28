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
//#include "at.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "apps/sntp/sntp.h"

#include "mbedtls/platform.h"
#include "mbedtls/net.h"
#include "mbedtls/esp_debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "esp_log.h"
#include "version.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_image_format.h"
//#include "nanopb.h"

#include "at_task.h"
#include "esp_system.h"
#include "at_upgrade.h"
#ifdef CONFIG_AT_BASE_ON_UART
#include "driver/uart.h"
typedef struct {
    int32_t baudrate;
    int8_t data_bits;
    int8_t stop_bits;
    int8_t parity;
    int8_t flow_control;
} at_nvm_uart_config_struct; 

QueueHandle_t esp_at_uart_queue = NULL;
static bool at_save_para_into_flash = false;
void at_port_print(uint8_t *str);

static bool at_nvm_uart_config_set (at_nvm_uart_config_struct *uart_config);
static bool at_nvm_uart_config_get (at_nvm_uart_config_struct *uart_config);


int32_t at_port_write_data(uint8_t*data,int32_t len)
{
    uint32_t length = 0;

    length = uart_write_bytes(CONFIG_AT_UART_PORT,(char*)data,len);
    return length;
}

int32_t at_port_read_data(uint8_t*buf,int32_t len)
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

int32_t at_port_get_data_length (void)
{
    size_t size = 0;
    if (ESP_OK == uart_get_buffered_data_len(CONFIG_AT_UART_PORT,&size)) {
        return size;
    } else {
        return 0;
    }
}

bool at_port_wait_write_complete (int32_t timeout_msec)
{
    if (ESP_OK == uart_wait_tx_done(CONFIG_AT_UART_PORT, timeout_msec*portTICK_PERIOD_MS)) {
        return true;
    }

    return false;
}

void uart_task(void *pvParameters)
{
    uart_event_t event;
    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            switch (event.type) {
            //Event of UART receving data
            case UART_DATA:
                if (event.size) {
                    esp_at_port_recv_data_notify (event.size, portMAX_DELAY);
                }
                break;
            case UART_PATTERN_DET:
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
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
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
    }
    //Set UART parameters
    uart_param_config(CONFIG_AT_UART_PORT, &uart_config);
    //Set UART pins,(-1: default pin, no change.)
    uart_set_pin(CONFIG_AT_UART_PORT, CONFIG_AT_UART_PORT_TX_PIN, CONFIG_AT_UART_PORT_RX_PIN, CONFIG_AT_UART_PORT_RTS_PIN, CONFIG_AT_UART_PORT_CTS_PIN);
    //Install UART driver, and get the queue.
    uart_driver_install(CONFIG_AT_UART_PORT, 2048, 8192, 10,&esp_at_uart_queue,0);
    xTaskCreate(uart_task, "uTask", 2048, (void*)CONFIG_AT_UART_PORT, 8, NULL);
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

    if (at_save_para_into_flash) {
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
    at_save_para_into_flash = true;
    ret = at_setupCmdUart(para_num);
    at_save_para_into_flash = false;
    
    return ret;
}

static uint8_t at_exeCmdCipupdate(uint8_t para_num)//add get station ip and ap ip
{
    char* Server_ip = NULL;
    char* Ota_filename = NULL;
    int32_t server_port;
    if (esp_at_get_para_as_str(0,(uint8_t**) &Server_ip) != ESP_AT_PARA_PARSE_RESULT_OK) 
        return ESP_AT_RESULT_CODE_ERROR;
    if (esp_at_get_para_as_digit(1,&server_port) != ESP_AT_PARA_PARSE_RESULT_OK) 
        return ESP_AT_RESULT_CODE_ERROR;
    if (esp_at_get_para_as_str(2,(uint8_t**) &Ota_filename) != ESP_AT_PARA_PARSE_RESULT_OK) 
        return ESP_AT_RESULT_CODE_ERROR;

    if (esp_at_upgrade_process(Server_ip, server_port, Ota_filename)) {
        esp_at_response_result(ESP_AT_RESULT_CODE_OK);
        return ESP_AT_RESULT_CODE_PROCESS_DONE;  
        /*esp_at_port_wait_write_complete(portMAX_DELAY);
        esp_restart();
        for(;;){
        }*/
    }

    return ESP_AT_RESULT_CODE_ERROR;
}

extern void lock_control(uint8_t lock_state);

static uint8_t at_lock_control(uint8_t para_num)
{
	int32_t lock_state;
    if (esp_at_get_para_as_digit(0,&lock_state) != ESP_AT_PARA_PARSE_RESULT_OK) 
        return ESP_AT_RESULT_CODE_ERROR;

    lock_control(lock_state);

    esp_at_response_result(ESP_AT_RESULT_CODE_OK);

    return ESP_AT_RESULT_CODE_PROCESS_DONE;
}


#ifndef ENABLE_BLUFI
uint8_t StartBluetooth(char* serial_number);
uint8_t StopBluetooth();
#endif 

uint8_t at_ControlBlufi(uint8_t para_num)
{
#ifdef ENABLE_BLUFI
    char* serial_number = NULL;
    int32_t enable = false;
    if (esp_at_get_para_as_digit(0,&enable) != ESP_AT_PARA_PARSE_RESULT_OK) 
        return ESP_AT_RESULT_CODE_ERROR;
    if(enable)
    {
        if (esp_at_get_para_as_str(1,(uint8_t**) &serial_number) != ESP_AT_PARA_PARSE_RESULT_OK) 
            return ESP_AT_RESULT_CODE_ERROR;

        if (StartBluetooth(serial_number)) 
        {
            esp_at_response_result(ESP_AT_RESULT_CODE_OK);
            return ESP_AT_RESULT_CODE_PROCESS_DONE;  
        }
    }
    else
    {
        if (StopBluetooth()) 
        {
            esp_at_response_result(ESP_AT_RESULT_CODE_OK);
            return ESP_AT_RESULT_CODE_PROCESS_DONE;  
        }

    }
    return ESP_AT_RESULT_CODE_ERROR;
#else 
    return 0;
#endif /* ENABLE_BLUFI */
}

extern uint8_t gattc_client_init(uint8_t* cmd_name);

uint8_t at_exeCmdSNTP(uint8_t* cmd_name)
{  
   
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    char strftime_buf[64];
    const int retry_count = 10;

	sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "us.pool.ntp.org"); 
    sntp_init();

    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        printf("Waiting for system time to be set... (%d/%d)\n", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    // Set timezone to Eastern Standard Time and print local timeinfo
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	printf("The current date/time in New York is: %s\n", strftime_buf);

    esp_at_response_result(ESP_AT_RESULT_CODE_OK);

	return ESP_AT_RESULT_CODE_PROCESS_DONE;
}

uint8_t at_getVersion(uint8_t* cmd_name)
{
	size_t image_size;
    char version_string[50];
    uint8_t version[5] = {0,0,0,0,0};
    const esp_partition_t* pPartition = esp_ota_get_next_update_partition(NULL);    
    if (esp_image_basic_verify(pPartition->address, true, &image_size) == ESP_OK)
    	spi_flash_read(pPartition->address + 0x13,&version, 5);

    sprintf(version_string, "AT Active:%d.%d.%d.%d,Inactive:%d.%d.%d.%d", MAJOR, MINOR, PATCH, BUILD, version[0], version[1], version[2], *((uint16_t*) &version[3]));
    at_port_print((uint8_t*) version_string);
    return ESP_AT_RESULT_CODE_OK;    
}

uint8_t at_get_wifi_config(uint8_t* cmd_name)
{
    char config_string[50];
    wifi_config_t sta_conf;
    esp_wifi_get_config(WIFI_IF_STA, &sta_conf);    
    sprintf(config_string, "AT ssid:%s,password:%s", sta_conf.sta.ssid, sta_conf.sta.password);
    at_port_print((uint8_t*) config_string);

    return ESP_AT_RESULT_CODE_OK;    
}

static esp_at_cmd_struct at_custom_cmd[] = {
    {"+UART", NULL, NULL, at_setupCmdUart, NULL},
    {"+UART_CUR", NULL, NULL, at_setupCmdUart, NULL},
    {"+UART_DEF", NULL, NULL, at_setupCmdUartDef, NULL},
    {"+CIUPDATE", NULL, NULL, at_exeCmdCipupdate, NULL},
    /* {"+BLUFI", NULL, NULL, at_ControlBlufi, NULL},   */ 
    {"+APPLY_UPDATE", NULL, NULL, NULL, at_upgrade_rollback},
    {"+LOCK_INIT", NULL, NULL, NULL, gattc_client_init},
    {"+LOCK", NULL, NULL, at_lock_control, NULL },
    {"+SNTP", NULL, NULL, NULL, at_exeCmdSNTP },
    {"+VERSION", NULL, NULL, NULL, at_getVersion },
    {"+WIFI_CONFIG", NULL, NULL, NULL, at_get_wifi_config },
    
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

EventGroupHandle_t at_task_init(void)
{
    EventGroupHandle_t group = NULL;
    uint8_t *version = (uint8_t *)malloc(64);
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_port_read_data,
        .write_data = at_port_write_data,
        .get_data_length = at_port_get_data_length,
        .wait_write_complete = at_port_wait_write_complete,
    };
    
    esp_at_custom_ops_struct esp_at_custom_ops = {
        .status_callback = at_status_callback,
    };

    at_uart_init();

    sprintf((char*)version, "compile time:%s %s", __DATE__, __TIME__);
    esp_at_device_ops_regist (&esp_at_device_ops);
    esp_at_custom_ops_regist(&esp_at_custom_ops);
    group = esp_at_module_init (0, version);
    free(version);

    esp_at_custom_cmd_array_regist (at_custom_cmd, sizeof(at_custom_cmd)/sizeof(at_custom_cmd[0]));
    esp_at_port_write_data((uint8_t *)"\r\nready\r\n",strlen("\r\nready\r\n"));
    return group;
}

#endif


