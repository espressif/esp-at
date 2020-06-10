// Copyright 2015-2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "sdio_host_log.h"
#include "sdio_host_error.h"
#include "sdio.h"
#include "gpio.h"
#include "platform_os.h"

static const char TAG[] = "sdio_driver";

SemaphoreHandle_t sdio_recv_SemHandle;

sdio_err_t sdio_driver_init(void)
{
	  sdio_init_t sdio_init = {
			.width = WIDTH_1,
			.clock = 400000    // 12M clock
		};
    sdio_recv_SemHandle = xSemaphoreCreateBinary();
	  STM32SdioInit(sdio_init);
    return SDIO_SUCCESS;
}

sdio_err_t sdio_driver_read_bytes(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{
    int ret = STM32ReadData(function, addr, buffer, len);
	  if(ret < 0){
		  printf("CMD53 read error, count: %d\r\n", len);
		  return FAILURE;
	  }
    return SDIO_SUCCESS;
}

sdio_err_t sdio_driver_write_bytes(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{
    int ret = STM32WriteData(function, addr, buffer, len);
	  if(ret < 0){
		  printf("CMD53 write error, count: %d\r\n", len);
		  return FAILURE;
	  }
    return SDIO_SUCCESS;
}

sdio_err_t sdio_driver_write_blocks(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{

    return sdio_driver_write_bytes(function, addr, buffer, len); 
}

sdio_err_t sdio_driver_read_blocks(uint32_t function, uint32_t addr, void* buffer, uint32_t len)
{

    return sdio_driver_read_bytes(function, addr, buffer, len); 
}

sdio_err_t sdio_driver_read_byte(uint32_t function, uint32_t reg, uint8_t *out_byte)
{
	  uint8_t func = function & 0x8;
	  uint8_t result = STM32ReadReg(func, reg);
	  if(out_byte != NULL){
			*out_byte = result;
		}
    return SDIO_SUCCESS;
}

sdio_err_t sdio_driver_write_byte(uint32_t function, uint32_t reg, uint8_t in_byte, uint8_t* out_byte)
{
	  uint8_t func = function & 0x8;
	  uint8_t result = STM32WriteReg(func, reg, in_byte);
	  if (out_byte != NULL) {
			*out_byte = result;
		}
    return SDIO_SUCCESS;
}

sdio_err_t sdio_driver_wait_int(uint32_t timeout)
{
	uint8_t res = STM32WaitIntr(timeout);
	if (res == 0) {
		return SDIO_SUCCESS;
	} else {
		return ERR_TIMEOUT;
	}
}