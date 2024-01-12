/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "nvs_flash.h"

#include "sdio_host_log.h"
#include "sdio_host_transport.h"
#include "sdio_config.h"

#include "driver/uart.h"
#include "driver/gpio.h"
#include "time.h"

#define SLAVE_PWR_GPIO       5          // sdio board ESP32 slave, ESP32 is IO5, ESP8266 is IO18

#define WRITE_BUFFER_LEN    2048
#define READ_BUFFER_LEN     4096

uint8_t send_buffer[WRITE_BUFFER_LEN] = "";
uint8_t rcv_buffer[READ_BUFFER_LEN] = "";

static const char TAG[] = "sdio_at_host";

int sdio_debugLevel = 2;    // print info log, set to 3 if you want to debug
static SemaphoreHandle_t rdySem;
static QueueHandle_t esp_at_uart_queue = NULL;

#define SDIO_ERROR_CHECK(x) do {                                         \
        sdio_err_t __err_rc = (x);                                       \
        if (__err_rc != SUCCESS) {                                       \
            SDIO_LOGE(TAG, "file: \"%s\" line %d\nfunc: %s\n error: %d\n", __FILE__, __LINE__,    \
                      __FUNCTION__, x);                 \
        }                                                               \
    } while(0);

// ESP-SDIO-TESTBOARD_V1 board specific set, ignore if not using this board
static void slave_power_on()
{
    gpio_config_t cfg = {
        .pin_bit_mask = BIT(SLAVE_PWR_GPIO),
        .mode = GPIO_MODE_DEF_OUTPUT,
        .pull_up_en = false,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
    gpio_set_level(SLAVE_PWR_GPIO, 0);  //low active
}

static void sdio_recv_task(void* pvParameters)
{
    while (1) {
        esp_err_t ret = sdio_host_wait_int(1000 / portTICK_PERIOD_MS);

        if (ret != SUCCESS) {
            continue;
        }

        uint32_t intr_raw, intr_st;
        ret = sdio_host_get_intr(&intr_raw, &intr_st);
        SDIO_ERROR_CHECK(ret);

        if (intr_raw == 0) {
            //SDIO_LOGW(TAG, "No intr\r\n");
            continue;
        }

        ret = sdio_host_clear_intr(intr_raw);
        SDIO_ERROR_CHECK(ret);
        SDIO_LOGD(TAG, "intr raw: %x, intr_st: %x", (unsigned int)intr_raw, (unsigned int)intr_st);

        const int wait_ms = 50;

        if (intr_raw & HOST_SLC0_RX_NEW_PACKET_INT_ST) {
            SDIO_LOGD(TAG, "new packet coming");

            while (1) {
                size_t size_read = READ_BUFFER_LEN;
                ret = sdio_host_get_packet(rcv_buffer, READ_BUFFER_LEN, &size_read, wait_ms);

                if (ret == ERR_NOT_FOUND) {
                    SDIO_LOGE(TAG, "interrupt but no data can be read");
                    break;
                } else if (ret != SUCCESS && ret != ERR_NOT_FINISHED) {
                    SDIO_LOGE(TAG, "rx packet error: %08X", ret);
                    break;
                }

                {
                    printf("%s", rcv_buffer);
                    fflush(stdout);
                }

                memset(rcv_buffer, 0x0, sizeof(rcv_buffer));

                if (ret == SUCCESS) {
                    break;
                }
            }
        }
    }

    vTaskDelete(NULL);
}

void uart_task(void* pvParameters)
{
    uart_event_t event;

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void *) &event, portMAX_DELAY)) {
            switch (event.type) {
            //Event of UART receving data
            case UART_DATA:
                if (event.size) {
                    // Master want to send data, mark it.
                    xSemaphoreGive(rdySem);
                }

                break;

            case UART_PATTERN_DET:
            default:
                break;
            }
        }
    }

    vTaskDelete(NULL);
}

static void sdio_task(void* pvParameters)
{
    sdio_err_t err;
    int at_uart_len;
    printf("Start SDIO test\r\n");
    slave_power_on();
    // Make sure SDIO slave has been inited
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    SDIO_LOGI(TAG, "host ready, start initializing slave...");
    err = sdio_init();
    assert(err == ESP_OK);

    //Create the semaphore.
    rdySem = xSemaphoreCreateBinary();

    uart_driver_install(0, 2048, 8192, 10, &esp_at_uart_queue, 0);
    xTaskCreate(uart_task, "uTask", 2048, NULL, 4, NULL);
    xTaskCreate(sdio_recv_task, "sdioRecvTask", 2048, NULL, 4, NULL);

    while (1) {
        //  wait send state flag
        xSemaphoreTake(rdySem, portMAX_DELAY);

        //Read data from uart
        at_uart_len = uart_read_bytes(0, (uint8_t*) send_buffer, WRITE_BUFFER_LEN, 0);
        SDIO_LOGD(TAG, "Receive UART len:%d,data:%s", at_uart_len, send_buffer);

        err = sdio_host_send_packet(send_buffer, at_uart_len);
        // Send timeout
        if (err == ERR_TIMEOUT) {
            SDIO_LOGW(TAG, "send timeout");
        }

        memset(send_buffer, '\0', sizeof(send_buffer));
    }
    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(sdio_task, "sdio_task", 3 * 1024, NULL, 5, NULL);
}
