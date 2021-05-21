/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#include "esp_at.h"
#if defined(CONFIG_IDF_TARGET_ESP32)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_log.h"

#ifdef CONFIG_AT_BASE_ON_HSPI
#include "driver/gpio.h"
#include "at_spi_driver.h"

static const char* TAG = "HSPI-AT";
static uint8_t *recv_data;
static uint32_t notify_len = 0;

//#define LOG_LOCAL_LEVEL 4     //debug mode, it will print debug log

#define GPIO_MOSI                   CONFIG_SPI_MOSI_PIN
#define GPIO_MISO                   CONFIG_SPI_MISO_PIN
#define GPIO_SCLK                   CONFIG_SPI_SCLK_PIN
#define GPIO_CS                     CONFIG_SPI_CS_PIN
#define SPI_SLAVE_HANDSHARK_GPIO     CONFIG_SPI_HANDSHAKE_PIN

#define SPI_SLAVE_HANDSHARK_SEL      (1ULL<<CONFIG_SPI_HANDSHAKE_PIN)
#define AT_READ_STREAM_BUFFER_SIZE      CONFIG_RX_STREAM_BUFFER_SIZE
#define AT_WRITE_STREAM_BUFFER_SIZE      CONFIG_TX_STREAM_BUFFER_SIZE

static xSemaphoreHandle spi_pxMutex;

void spi_mutex_lock(void)
{
    xSemaphoreTake(spi_pxMutex, portMAX_DELAY);
}

void spi_mutex_unlock(void)
{
    xSemaphoreGive(spi_pxMutex);
}

//Called notify master can send data. We use this to set the handshake line high.
void pull_high_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, SPI_SLAVE_HANDSHARK_SEL);
}

//Called notify master need to wait. We use this to set the handshake line low.
void pull_low_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, SPI_SLAVE_HANDSHARK_SEL);
}

/* Called when spi receive a normal AT command, make sure you have added \r\n in your spi data */
static int32_t at_spi_read_data(uint8_t* data, int32_t len)
{

    if (data == NULL || len < 0) {
        return -1;
    }

    if (len == 0) {
        ESP_LOGI(TAG, "Empty read data.");
        return 0;
    }

    memcpy(data, recv_data, len);
    ESP_LOGD(TAG, "read len: %d", len);

    at_spi_slave_free_receive_buffer(HSPI_HOST);
    spi_mutex_unlock();
    return len;
}

/* Result of AT command, auto call when read_data get data */
static int32_t at_spi_write_data(uint8_t* buf, int32_t len)
{
    esp_err_t ret;
    uint8_t wait_time = 10;
    uint8_t* sendbuf = NULL;
    if (len < 0 || buf == NULL) {
        ESP_LOGE(TAG, "Cannot get write data.");
        return -1;
    }

    if (len == 0) {
        ESP_LOGE(TAG, "Empty write data.");
        return 0;
    }
    spi_mutex_lock();
    ESP_LOGD(TAG, "Write data len: %d", len);
    sendbuf = heap_caps_malloc(len, MALLOC_CAP_DMA);
    if (sendbuf == NULL) {
        ESP_LOGE(TAG , "Malloc send buffer fail!");
        spi_mutex_unlock();
        return 0;
    }
    memcpy(sendbuf, buf, len);

    while (wait_time--) {
        ret = at_spi_slave_send(HSPI_HOST, sendbuf, len, 1000 / portTICK_PERIOD_MS);
        if (ret == ESP_OK) {
            break;
        }
    }
    if (wait_time == 0) {
        ESP_LOGE(TAG, "send error");
        len = -1;
    }
    spi_mutex_unlock();

    free(sendbuf);
    return len;
}

int32_t at_spi_get_data_length(void)
{
    return notify_len;
}

static void at_spi_slave_task(void* pvParameters)
{
    esp_err_t ret;
    uint32_t recv_len = 0;

    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .intr_flags = ESP_INTR_FLAG_IRAM
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=CONFIG_SPI_MODE,
        .spics_io_num=GPIO_CS,
        .queue_size=3,
        .flags=0,
        .post_setup_cb=pull_high_cb,
        .post_trans_cb=pull_low_cb
    };

    //Configuration for the handshake line
    gpio_config_t io_conf={
        .intr_type=GPIO_INTR_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=SPI_SLAVE_HANDSHARK_SEL
    };

    //Configure handshake line as output
    gpio_config(&io_conf);
    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    spi_pxMutex = xSemaphoreCreateMutex();
    if (!spi_pxMutex) {
        ESP_LOGE(TAG, "create mutex error\r\n");
    }

    //Initialize SPI slave interface
    ret= at_spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, 1);
    assert(ret==ESP_OK);

    at_spi_write_data((uint8_t *)"ready\r\n", strlen("ready\r\n"));

    while (1) {
        ret = at_spi_slave_receive(HSPI_HOST, &recv_data, &recv_len, portMAX_DELAY);
        if(ret != ESP_OK) {
            ESP_LOGE(TAG, "Recv error: %d\r\n", ret);
            break;
        }
        spi_mutex_lock();

        // notify length to AT core
        esp_at_port_recv_data_notify(recv_len, portMAX_DELAY);
        notify_len = recv_len;
    }
    
}

void at_interface_init(void)
{
    esp_at_device_ops_struct esp_at_device_ops = {
        .read_data = at_spi_read_data,
        .write_data = at_spi_write_data,
        .get_data_length = at_spi_get_data_length,
        .wait_write_complete = NULL

    };
    
    esp_at_device_ops_regist(&esp_at_device_ops);
}

void at_custom_init(void)
{
    xTaskCreate(at_spi_slave_task , "at_spi_task" , 4096 , NULL , 10 , NULL);
}
#endif
#endif
