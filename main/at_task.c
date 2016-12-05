/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "driver/uart.h"
#include "nvs_flash.h"
#include "at.h"

#if 0
#include "esp_ring_buffer.h"

#define AT_RX_BUFFER_SIZE       2048
#define AT_TX_BUFFER_SIZE       (1024*8)

esp_ring_buffer_t* at_rx_buffer = NULL;
esp_ring_buffer_t* at_tx_buffer = NULL;


uint8_t uart_flow_ctrl_enable_state(uint8_t uart_no)
{
    return 0;
}

///////////////////////////
// TODO:
uint32_t uart_get_tx_fifo_left_len(enum_uart_port_t uart_no);

//////////////////////////////

void IRAM_ATTR at_uart_intr_cb(uart_event_msg_t*msg)
{
	uint32_t length = 0;
    uint32_t send_len = 0;
    uint8_t buffer[UART_FIFO_LEN];

    if (msg == NULL) {
        return;
    }
    if (msg->uart_port != UART0) {
        return;
    }
    
    if (msg->event_type == UART_EVENT_RX_INT) {
        uint8_t* data = (uint8_t*)msg->buffer;
        uint32_t len = msg->buffer_cnt;
		if ((len != 0) && (data != NULL)) {
			if (esp_ring_get_left_length(at_rx_buffer) >= len) {
                length = esp_ring_enq(at_rx_buffer,data,len);
				at_port_recv_data_notify_from_isr(length);
			} else {
				if ((len == 3) && (at_memcmp(data,"+++",3) == 0)) {
					at_rx_buf_transmit_terminal_from_isr();
				}
            }
		}
    } else if (msg->event_type == UART_EVENT_TX_INT) {
        send_len = esp_ring_get_data_length(at_tx_buffer);
        length = esp_ring_deq(at_tx_buffer,buffer,uart_get_tx_fifo_left_len(UART0));
        // TODO:
        if (length > 0) {
    	    uart_tx_chars(UART0,(char*)buffer,length);
            uart_enable_tx_intr(UART0);
        } else if (send_len > 0) {
            uart_enable_tx_intr(UART0);
        }
    }
}

uint32_t at_port_write_data(uint8_t*data,uint32_t len)
{
    uint32_t length = 0;
    uint32_t send_len = 0;
    uint8_t buffer[UART_FIFO_LEN];
	if ((data == NULL) || (len == 0)) {
		return 0;
	}
    
    length = esp_ring_enq(at_tx_buffer,data,len);
    uart_disable_tx_intr(UART0);
    send_len = esp_ring_deq(at_tx_buffer,buffer,uart_get_tx_fifo_left_len(UART0));
    // TODO:
    if (send_len > 0) {
	    uart_tx_chars(UART0,(char*)buffer,send_len);
    }
    uart_enable_tx_intr(UART0);

    return length;
}

bool at_port_wait_tx_finish(void)
{
    while (esp_ring_get_data_length(at_tx_buffer)) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    uart_wait_tx_fifo_empty(UART0);
    return true;
}


void at_uart_rx_threshold_cb_func(esp_ring_event_t event,uint32_t size)
{
    switch (event) {
        case ESP_RING_UPPER_THRESHOLD_EVENT:
            // TODO: flow control
            uart_disable_rx_intr(UART0);
            break;
        case ESP_RING_LOW_THRESHOLD_EVENT:
            // TODO: flow control
            uart_enable_rx_intr(UART0);
            break;
    }
}

void at_uart_tx_threshold_cb_func(esp_ring_event_t event,uint32_t size)
{
    switch (event) {
        case ESP_RING_UPPER_THRESHOLD_EVENT:
            // TODO: flow control
            break;
        case ESP_RING_LOW_THRESHOLD_EVENT:
            // TODO: flow control
            break;
    }
}


int32_t at_rx_buf_data_deq(uint8_t*buf,int32_t len)
{
    return esp_ring_deq(at_rx_buffer,buf,len);
}

void at_uart_init(void)
{
    uart_set_flow_control(0,0,120);
    uart1_tx_pad_init(16);  
    uart_set_print_port(UART0);
    uart_param_init_default(UART0);
    uart_param_init_default(UART1);
    uart_intr_init_default(UART0,ETS_UART0_INUM);
    uart_event_callback_register(at_uart_intr_cb);
}

void at_ring_buffer_init(void)
{
    at_rx_buffer = esp_ring_buffer_init(AT_RX_BUFFER_SIZE);
    if (at_rx_buffer == NULL) {
        abort();
    }
    
    at_tx_buffer = esp_ring_buffer_init(AT_TX_BUFFER_SIZE);
    if (at_tx_buffer == NULL) {
        abort();
    }
    esp_ring_threshold_cb_func_regist(at_rx_buffer,at_uart_rx_threshold_cb_func,128,128);
    esp_ring_threshold_cb_func_regist(at_tx_buffer,at_uart_tx_threshold_cb_func,128,128);

}

void at_task_init(void)
{
    uint8_t* version = (uint8_t*)at_malloc(64);

	at_uart_init();
    
    at_ring_buffer_init();

	at_sprintf(version,"compile time:%s %s",__DATE__,__TIME__);
    at_init(0,version);
	at_free(version);
    
    at_port_print((uint8_t*)"\r\nready\r\n");

}
#else
QueueHandle_t uart0_queue;

uint32_t at_port_write_data(uint8_t*data,uint32_t len)
{
    uint32_t length = 0;
	
	length = uart_write_bytes(UART_NUM_0,(char*)data,len);
    return length;
}

int32_t at_rx_buf_data_deq(uint8_t*buf,int32_t len)
{
    return uart_read_bytes(UART_NUM_0,buf,len,0);
}


void uart_task(void *pvParameters)
{
	uart_event_t event;
	for(;;) {
		 //Waiting for UART event.
		if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
			switch(event.type) {
				//Event of UART receving data
				case UART_DATA:
					if (event.size) {
						//esp_dev_event_report(&esp_uart_dev,ESP_DEVICE_EVT_RCVPLUS);
						at_port_recv_data_notify(event.size,portMAX_DELAY);
					}
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
	int uart_num = 0;
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.rx_flow_ctrl_thresh = 122,
	};
	//Set UART parameters
	uart_param_config(uart_num, &uart_config);
	//Set UART pins,(-1: default pin, no change.)
	uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_set_pin(UART_NUM_1, 16, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	//Install UART driver, and get the queue.
	uart_driver_install(uart_num, 2048, 8192, 10, 17, &uart0_queue);
	xTaskCreate(uart_task, "uTask", 2048, (void*)uart_num, 8, NULL);
}



void at_task_init(void)
{
    uint8_t* version = (uint8_t*)at_malloc(64);

	// nvs_flash_init_custom(CONFIG_AT_NVS_SECTOR_BASE, CONFIG_AT_NVS_SECTOR_COUNT);
	at_uart_init();
    
	at_sprintf(version,"compile time:%s %s",__DATE__,__TIME__);
    at_init(0,version);
	at_free(version);
    
    at_port_print((uint8_t*)"\r\nready\r\n");

}


#endif


