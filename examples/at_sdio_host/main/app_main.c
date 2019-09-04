/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_slave.h"

#include "nvs_flash.h"

#include "soc/gpio_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sdmmc_struct.h"

#include "driver/periph_ctrl.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#define SLAVE_PWR_GPIO       5          // sdio board ESP32 slave
#define READ_BUFFER_LEN     4096

static const char TAG[] = "sdio_host";

static xSemaphoreHandle rdySem;
static QueueHandle_t esp_at_uart_queue = NULL;
static esp_slave_context_t context;

static uint8_t esp_at_sendbuf[READ_BUFFER_LEN] = "";
static uint8_t esp_at_recvbuf[READ_BUFFER_LEN + 1] = "";

// SDIO board specific set
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

//host use this to initialize the slave card as well as SDIO registers
static esp_err_t slave_init(esp_slave_context_t* context)
{
    /* Probe */
    sdmmc_host_t config = SDMMC_HOST_DEFAULT();
#ifdef CONFIG_SDIO_EXAMPLE_4BIT
    config.flags = SDMMC_HOST_FLAG_4BIT;
#else
    config.flags = SDMMC_HOST_FLAG_1BIT;
#endif

#ifdef CONFIG_SDIO_EXAMPLE_HIGHSPEED
    config.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
#else
    config.max_freq_khz = SDMMC_FREQ_DEFAULT;
#endif

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    sdmmc_host_init();
    sdmmc_host_init_slot(SDMMC_HOST_SLOT_1, &slot_config);
    sdmmc_card_t* card = (sdmmc_card_t*)malloc(sizeof(sdmmc_card_t));

    if (card == NULL) {
        return ESP_ERR_NO_MEM;
    }

    for (;;) {
        if (sdmmc_card_init(&config, card) == ESP_OK) {
            break;
        }

        ESP_LOGW(TAG, "slave init failed, retry...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    gpio_pullup_en(14);
    gpio_pulldown_dis(14);
    gpio_pullup_en(15);
    gpio_pulldown_dis(15);
    gpio_pullup_en(2);
    gpio_pulldown_dis(2);
    gpio_pullup_en(4);
    gpio_pulldown_dis(4);
    gpio_pullup_en(12);
    gpio_pulldown_dis(12);
    gpio_pullup_en(13);
    gpio_pulldown_dis(13);

    *context = ESP_SLAVE_DEFAULT_CONTEXT(card);
    context->buffer_size = 512;
    esp_err_t ret = esp_slave_init_io(context);

    return ret;
}

static void sdio_recv_task(void* pvParameters)
{
    const int wait_ms = 200;

    while (1) {
        esp_err_t ret = esp_slave_wait_int(&context, portMAX_DELAY);
        assert(ret == ESP_OK);

        uint32_t intr_raw, intr_st;
        ret = esp_slave_get_intr(&context, &intr_raw, &intr_st);
        assert(ret == ESP_OK);
        ret = esp_slave_clear_intr(&context, intr_raw);
        assert(ret == ESP_OK);
        ESP_LOGD(TAG, "intr: %08X", intr_raw);

        if (intr_raw & HOST_SLC0_RX_NEW_PACKET_INT_ST) {

            size_t size_read = READ_BUFFER_LEN;
            esp_err_t err = esp_slave_get_packet(&context, esp_at_recvbuf, READ_BUFFER_LEN, &size_read, wait_ms);

            if (err == ESP_ERR_NOT_FOUND) {
                ESP_LOGE(TAG, "interrupt but no data can be read");
                break;
            } else if (err != ESP_OK && err != ESP_ERR_NOT_FINISHED) {
                ESP_LOGE(TAG, "rx packet error: %08X", err);
            }

            {
                printf("%s", esp_at_recvbuf);
                fflush(stdout);
            }

            memset(esp_at_recvbuf, '\0', sizeof(esp_at_recvbuf));
        }
    }
}

void uart_task(void* pvParameters)
{
    uart_event_t event;

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(esp_at_uart_queue, (void*) &event,
                          (portTickType) portMAX_DELAY)) {
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

// reset SDIO , used for slave restart
esp_err_t esp_at_reset_sdio()
{
    esp_err_t err = ESP_OK;
    uint8_t init_count = 3;
    err = slave_init(&context);

    while (err != ESP_OK && init_count > 0) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        err = slave_init(&context);
        init_count--;
    }

    return err;
}

static void sdio_task(void* pvParameters)
{
    esp_err_t err = ESP_OK;
    int at_uart_len;
    const int wait_ms = 50;

    //enable the power if on espressif SDIO master-slave board
    slave_power_on();

    // Make sure SDIO slave has been inited
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "host ready, start initializing slave...");
    err = slave_init(&context);
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
        at_uart_len = uart_read_bytes(0, (uint8_t*) esp_at_sendbuf, READ_BUFFER_LEN, 0);
        ESP_LOGD(TAG, "Receive UART len:%d,data:%s", at_uart_len, esp_at_sendbuf);

        err = esp_slave_send_packet(&context, esp_at_sendbuf, at_uart_len, wait_ms);

        // If send timeout, the SLAVE state is not correct and need to be re-initialized
        if (err == ESP_ERR_TIMEOUT) {
            ESP_LOGW(TAG, "send timeout, maybe SDIO slave restart, reinit SDIO slave");

            err = esp_at_reset_sdio();
            assert(err == ESP_OK);
            // After re-initialized, send again
            err = esp_slave_send_packet(&context, esp_at_sendbuf, at_uart_len, wait_ms);
            assert(err == ESP_OK);
        }

        // restart or restore command need re-init SDIO
        if (at_uart_len < 15) {
            esp_at_sendbuf[13] = '\0';

            if (strstr((char*)esp_at_sendbuf, "at+rst\r\n") != NULL || strstr((char*)esp_at_sendbuf, "AT+RST\r\n") != NULL || strstr((char*)esp_at_sendbuf, "at+restore\r\n") != NULL || strstr((char*)esp_at_sendbuf, "AT+RESTORE\r\n") != NULL) {
                ESP_LOGW(TAG, "Call restart or restore, reinit SDIO slave");
                // Waiting for sdio slave restart
                vTaskDelay(1500 / portTICK_PERIOD_MS);
                err = esp_at_reset_sdio();
                assert(err == ESP_OK);
            }
        }

        memset(esp_at_sendbuf, '\0', sizeof(esp_at_sendbuf));
    }
}

//Main application
void app_main()
{
    xTaskCreate(sdio_task, "sdio_task", 2 * 1024, NULL, 5, NULL);
}
