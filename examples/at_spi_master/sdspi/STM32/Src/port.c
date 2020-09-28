
/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
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
 #include "FreeRTOS.h"
 #include "task.h"
 #include "semphr.h"
 #include "gpio.h"
 #include "spi.h"

#include "sdspi_host.h"
#include "port.h"

SemaphoreHandle_t DataBinarySem01Handle;

/// Set CS high
void at_cs_high(void)
{
    CS_high();
}

/// Set CS low
void at_cs_low(void)
{
    CS_low();
}

esp_err_t at_spi_transmit(void* tx_buff, void* rx_buff, uint32_t len)
{
	  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, len);
	  while(HAL_SPI_STATE_READY != HAL_SPI_GetState(&hspi1));
	  //HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, len, 100);
	  return ESP_OK;
}

/*we have inited SPI in spi.c*/
esp_err_t at_spi_slot_init(void)
{
	  DataBinarySem01Handle = xSemaphoreCreateBinary();
	  assert(DataBinarySem01Handle != NULL);
	  at_cs_high();
	  xSemaphoreTake(DataBinarySem01Handle, 0);

    return ESP_OK;
}

esp_err_t at_spi_wait_int(uint32_t wait_ms)
{
    //wait for semaphore
    BaseType_t ret = xSemaphoreTake(DataBinarySem01Handle, wait_ms / portTICK_PERIOD_MS);

    if (ret == pdFALSE) {
        return ESP_ERR_TIMEOUT;
    }

    return ESP_OK;
}

void at_do_delay(uint32_t wait_ms)
{
   vTaskDelay(wait_ms);
	//HAL_Delay(wait_ms);
}


AT_MUTEX_T at_mutex_init(void)
{
    SemaphoreHandle_t xSemaphore = NULL;
    xSemaphore = xSemaphoreCreateMutex();
    if (xSemaphore == NULL) {
			printf("Mutex create error\r\n");
			return NULL;
		}
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
