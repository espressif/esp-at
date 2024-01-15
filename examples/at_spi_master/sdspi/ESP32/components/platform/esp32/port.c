/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "driver/spi_master.h"
#include "esp_log.h"

#include "soc/gpio_reg.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"

#include "freertos/queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "sdspi_host.h"
#include "port.h"

static spi_device_handle_t spi_handle;
//The semaphore indicating the slave is ready to receive stuff.
static QueueHandle_t rdySem;

#define AT_SDSPI_MISO GPIO_NUM_2
#define AT_SDSPI_MOSI GPIO_NUM_15
#define AT_SDSPI_CLK  GPIO_NUM_14
#define AT_SDSPI_CS   GPIO_NUM_13
#define AT_SDSPI_INT  GPIO_NUM_4
#define SPI_CLOCK     5000 * 1000

/// Set CS high
void at_cs_high(void)
{
    gpio_set_level(AT_SDSPI_CS, 1);
}

/// Set CS low
void at_cs_low(void)
{
    gpio_set_level(AT_SDSPI_CS, 0);
}

esp_err_t at_spi_transmit(void* tx_buff, void* rx_buff, uint32_t len)
{
    esp_err_t ret;
    spi_transaction_t t = {
        .flags = 0,
        .length = len * 8,
        .tx_buffer = tx_buff,
        .rx_buffer = rx_buff,
    };
    ret = spi_device_transmit(spi_handle, &t);

    return ret;
}

static void gpio_intr(void* arg)
{
    BaseType_t awoken = pdFALSE;
    xSemaphoreGiveFromISR(rdySem, &awoken);
    gpio_intr_disable(AT_SDSPI_INT);

    if (awoken) {
        portYIELD_FROM_ISR();
    }
}

esp_err_t at_spi_slot_init(void)
{
    spi_bus_config_t buscfg = {
        .miso_io_num = AT_SDSPI_MISO,
        .mosi_io_num = AT_SDSPI_MOSI,
        .sclk_io_num = AT_SDSPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    // Initialize SPI bus and enable dma
    esp_err_t ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret == ESP_OK);

    // Attach the SD card to the SPI bus
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_CLOCK,          // 2M
        .mode = 0,
        // For SD cards, CS must stay low during the whole read/write operation,
        // rather than a single SPI transaction.
        .spics_io_num = -1,
        .queue_size = 4,
    };
    ret =  spi_bus_add_device(HSPI_HOST, &devcfg, &spi_handle);
    assert(ret == ESP_OK);

    // Configure CS pin
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1LL << AT_SDSPI_CS,
    };

    ret = gpio_config(&io_conf);
    assert(ret == ESP_OK);
    at_cs_high();

    io_conf = (gpio_config_t) {
        .intr_type = GPIO_INTR_LOW_LEVEL,    // low level
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = true,
        .pin_bit_mask = (1 << AT_SDSPI_INT),
    };
    ret = gpio_config(&io_conf);

    // Disable GPIO interrupt
    gpio_intr_disable(AT_SDSPI_INT);

    rdySem = xSemaphoreCreateBinary();
    assert(rdySem != NULL);

    // 1. the interrupt is better to be disabled before the ISR is registered
    // 2. the semaphore MUST be initialized before the ISR is registered
    // 3. the gpio_int member should be filled before the ISR is registered
    //TODO: the ISR is registered here, but should be registered in a more common way
    ret = gpio_install_isr_service(0);
    assert(ret == ESP_OK);
    ret = gpio_isr_handler_add(AT_SDSPI_INT, &gpio_intr, NULL);
    assert(ret == ESP_OK);

    return ESP_OK;
}

esp_err_t at_spi_wait_int(uint32_t wait_ms)
{
    //clear semaphore
    xSemaphoreTake(rdySem, 0);

    //the interrupt will disable itself
    gpio_intr_enable(AT_SDSPI_INT);

    //wait for semaphore
    BaseType_t ret = xSemaphoreTake(rdySem, wait_ms / portTICK_PERIOD_MS);

    if (ret == pdFALSE) {
        gpio_intr_disable(AT_SDSPI_INT);
        return ESP_ERR_TIMEOUT;
    }

    return ESP_OK;
}

void at_do_delay(uint32_t wait_ms)
{
    vTaskDelay(wait_ms / portTICK_PERIOD_MS);
}

AT_MUTEX_T at_mutex_init(void)
{
    SemaphoreHandle_t xSemaphore = NULL;
    xSemaphore = xSemaphoreCreateMutex();
    assert(xSemaphore);
    return (AT_MUTEX_T)xSemaphore;
}

void at_mutex_lock(AT_MUTEX_T pxMutex)
{
    xSemaphoreTake((SemaphoreHandle_t)pxMutex, portMAX_DELAY);
}

void at_mutex_unlock(AT_MUTEX_T pxMutex)
{
    xSemaphoreGive((SemaphoreHandle_t)pxMutex);
}

void at_mutex_free(AT_MUTEX_T pxMutex)
{
    vSemaphoreDelete((SemaphoreHandle_t)pxMutex);
    pxMutex = NULL;
}
