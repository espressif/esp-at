/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_attr.h"
#include "sdkconfig.h"

#include "sdspi_host.h"
#include "port.h"

#include "driver/gpio.h"
#include "driver/uart.h"

static const char TAG[] = "spi_host";

/*
   sdspi host example.

   This example is supposed to work together with the sdio slave example. It uses the pins as follows:

     *   Host      Slave
     *   IO14      CLK
     *   IO15      CMD
     *   IO2       D0
     *   IO4       D1
     *   IO12      D2
     *   IO13      D3

*/

#define READ_BUFFER_LEN     4096

spi_context_t context;

static SemaphoreHandle_t rdySem;
static QueueHandle_t esp_at_uart_queue = NULL;

static uint8_t esp_at_sendbuf[READ_BUFFER_LEN] = "";
static uint8_t esp_at_recvbuf[READ_BUFFER_LEN + 1] = "";

#define SLAVE_PWR_GPIO 5

void slave_power_on(void)
{
#ifdef SLAVE_PWR_GPIO
    gpio_config_t cfg = {
        .pin_bit_mask = BIT(SLAVE_PWR_GPIO),
        .mode = GPIO_MODE_DEF_OUTPUT,
        .pull_up_en = false,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
    gpio_set_level(SLAVE_PWR_GPIO, 0);  //low active
    vTaskDelay(100 / portTICK_PERIOD_MS);
#endif
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

static void sdspi_recv_task(void* pvParameters)
{
    esp_err_t ret;
    uint8_t flag = 1;
    uint32_t intr_raw;

    while (1) {

        if (flag) {
            ret = at_spi_wait_int(100);
        } else {
            ret = at_spi_wait_int(portMAX_DELAY);
        }

        if (ret == ESP_ERR_TIMEOUT) {
            flag = 0;
            continue;
        }

        assert(ret == ESP_OK);

        ret = at_sdspi_get_intr(&intr_raw);
        assert(ret == ESP_OK);

        ret = at_sdspi_clear_intr(intr_raw);
        assert(ret == ESP_OK);

        if (intr_raw & HOST_SLC0_RX_NEW_PACKET_INT_ST) {
            size_t size_read = READ_BUFFER_LEN;
            esp_err_t err = at_sdspi_get_packet(&context, esp_at_recvbuf, READ_BUFFER_LEN, &size_read);

            if (err == ESP_ERR_NOT_FOUND) {
                ESP_AT_LOGE(TAG, "interrupt but no data can be read");
                break;
            } else if (err != ESP_OK && err != ESP_ERR_NOT_FINISHED) {
                ESP_AT_LOGE(TAG, "rx packet error: %08X", err);
            }

            {
                printf("%s", esp_at_recvbuf);
                fflush(stdout);
            }

            memset(esp_at_recvbuf, '\0', sizeof(esp_at_recvbuf));
        }
    }
}

static void sdspi_send_task(void* pvParameters)
{
    esp_err_t err;

    int at_uart_len;
    //Create the semaphore.
    rdySem = xSemaphoreCreateBinary();

    uart_driver_install(0, 2048, 8192, 10, &esp_at_uart_queue, 0);
    xTaskCreate(uart_task, "uTask", 2048, NULL, 4, NULL);
    memset(&context, 0x0, sizeof(spi_context_t));

    while (1) {
        //  wait send state flag
        xSemaphoreTake(rdySem, portMAX_DELAY);

        //Read data from uart
        at_uart_len = uart_read_bytes(0, (uint8_t*) esp_at_sendbuf, READ_BUFFER_LEN, 0);

        if (at_uart_len < 0) {
            ESP_AT_LOGE(TAG, "Read error UART length");
            continue;
        }

        err = at_sdspi_send_packet(&context, esp_at_sendbuf, at_uart_len, UINT32_MAX);

        if (err != ESP_OK) {
            ESP_AT_LOGE(TAG, "Send error, %d\n", err);
        }

        memset(esp_at_sendbuf, '\0', sizeof(esp_at_sendbuf));
    }
    vTaskDelete(NULL);
}

void app_main()
{
    esp_err_t err;
    // SDIO TEST board need to enable slave, other develop borad ignore
    //slave_power_on();
    ESP_AT_LOGI(TAG, "host ready, start initializing slave...");
    err = at_sdspi_init();
    assert(err == ESP_OK);
    memset(&context, 0x0, sizeof(spi_context_t));
    xTaskCreate(sdspi_recv_task, "sdspi_recv_task", 4 * 1024, NULL, 6, NULL);
    xTaskCreate(sdspi_send_task, "sdspi_send_task", 4 * 1024, NULL, 5, NULL);
}
