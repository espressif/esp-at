/**
  ******************************************************************************
  * File Name          : SDIO.h
  * Description        : This file provides code for the configuration
  *                      of the SDIO instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sdio_H
#define __sdio_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>

typedef enum {
	WIDTH_1,     // 1bit
	WIDTH_4      // 4bit
} sdio_width_t;

typedef struct {
	sdio_width_t width;        // bus width
	uint32_t clock;           // SDIO clock
}sdio_init_t;

void STM32SdioInit(sdio_init_t sdio_init);
	 
uint8_t STM32WaitIntr(uint32_t timeout);

uint8_t STM32ReadReg(uint8_t func, uint32_t addr);

uint8_t STM32WriteReg(uint8_t func, uint32_t addr, uint8_t value);
	 
int STM32WriteData(uint8_t func, uint32_t addr, const void *data, uint32_t size);
	 
int STM32ReadData(uint8_t func, uint32_t addr, void *data, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif /*__ sdio_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
