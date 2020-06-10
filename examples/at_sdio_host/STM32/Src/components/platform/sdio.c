/**
  ******************************************************************************
  * File Name          : SDIO.c
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

/* Includes ------------------------------------------------------------------*/
#include "sdio.h"

/* USER CODE BEGIN 0 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stdio.h"
#include "sdio_host_log.h"
#include "sdio_host_error.h"
#include "stm32f1xx_ll_dma.h"
/* USER CODE END 0 */

static const char TAG[] = "STM32_sdio";

#define	BIT(x)				(1UL<<(x))

#define SDMMC_R4_READYBIT BIT(31)
#define SDMMC_R4_ERRORBITS ((uint32_t)0x7F000000U)
#define SDMMC_R4_NOERROR ((uint32_t)0x20000000U)
#define SDMMC_R4_OCRBITS ((uint32_t)0x00FFFFFFU)

#define ESP_DATA_MIN_ADDRESS    0x1e800

#define CMD52_WRITE BIT(31)
#define CMD52_READAFTERWRITE BIT(27)
#define CMD53_WRITE BIT(31)
#define CMD53_BLOCKMODE BIT(27)
#define CMD53_INCREMENTING BIT(26)
#define CMD53_TIMEOUT 10000000
#define SD_IO_CCCR_BUS_WIDTH        0x07
#define CCCR_BUS_WIDTH_4           (2<<0)

#define SDIO_BLOCK_SIZE 512

static SDIO_CmdInitTypeDef sdio_cmd;
static SDIO_DataInitTypeDef sdio_data;
extern SemaphoreHandle_t sdio_recv_SemHandle;
static SemaphoreHandle_t semahandle;

static uint8_t CalcClockDivider(uint32_t freq, uint32_t *preal)
{
  int divider;
  uint32_t sdioclk;
  
  sdioclk = HAL_RCC_GetHCLKFreq();
  if (freq == 0)
    freq = 1;
  
  divider = sdioclk / freq - 2;
  if (sdioclk % freq != 0)
    divider++; 
  if (divider < 0)
    divider = 0;
  else if (divider > 255)
    divider = 255;
  
  *preal = sdioclk / (divider + 2);
  SDIO_LOGI(TAG, "[Clock] freq=%.1fkHz, requested=%.1fkHz, divider=%d\n", *preal / 1000.0f, freq / 1000.0f, divider);
  return divider & 0xff;
}

int GetITStatus(void)
{
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_SDIOIT) != RESET)
  {
		SDIO_LOGD(TAG, "Clear IT");
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_SDIOIT);
    return 1;
  }
  else
    return 0;
}

static void SdioGpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDIO GPIO Configuration    
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD 
    */
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
												|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

static int CheckError(const char *msg_title)
{
  int err = 0;
  
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CCRCFAIL) != RESET && sdio_cmd.CmdIndex != 5)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CCRCFAIL);
    err++;
    SDIO_LOGE(TAG, "%s: CMD%d CRC failed!\n", msg_title, sdio_cmd.CmdIndex);
  }
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CTIMEOUT) != RESET)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CTIMEOUT);
		
		// CMD5 no CRC
		if(sdio_cmd.CmdIndex != 5) {
      err++;
      SDIO_LOGE(TAG, "%s: CMD%d timeout!\n", msg_title, sdio_cmd.CmdIndex);
		}
  }
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_DCRCFAIL) != RESET)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_DCRCFAIL);
    err++;
    SDIO_LOGE(TAG, "%s: data CRC failed!\n", msg_title);
  }
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_DTIMEOUT) != RESET)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_DTIMEOUT);
    err++;
    SDIO_LOGE(TAG, "%s: data timeout!\n", msg_title);
  }
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_STBITERR) != RESET)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_STBITERR);
    err++;
    SDIO_LOGE(TAG, "%s: start bit error!\n", msg_title);
  }
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXUNDERR) != RESET)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_TXUNDERR);
    err++;
    SDIO_LOGE(TAG, "%s: data underrun!\n", msg_title);
  }
  if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXOVERR) != RESET)
  {
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_RXOVERR);
    err++;
    SDIO_LOGE(TAG, "%s: data overrun!\n", msg_title);
  }
  if (LL_DMA_IsActiveFlag_TE4(DMA2))
  {
    LL_DMA_ClearFlag_TE4(DMA2);
    err++;
    SDIO_LOGE(TAG, "%s: DMA transfer error!\n", msg_title);
  }
  return err;
}

static void WaitForResponse(const char *msg_title)
{
  uint8_t i = 0;
  
  do
  {
    if (i == 3)
      break;
    
    if (i != 0)
      SDIO_SendCommand(SDIO, &sdio_cmd); // resend command
    i++;
    
    while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CMDACT) != RESET); // wait command send done 
    CheckError(msg_title);
  } while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CMDREND) == RESET); // if no response
  __SDIO_CLEAR_FLAG(SDIO, SDIO_STATIC_CMD_FLAGS);
}


static uint32_t SDMMC_GetCmdError(SDIO_TypeDef *SDIOx)
{
  /* 8 is the number of required instructions cycles for the below loop statement.
  The SDIO_CMDTIMEOUT is expressed in ms */
  register uint32_t count = SDIO_CMDTIMEOUT * (SystemCoreClock / 8U /1000U);
  
  do
  {
    if (count-- == 0U)
    {
      return SDMMC_ERROR_TIMEOUT;
    }
    
  }while(!__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_CMDSENT));
  
  /* Clear all the static flags */
  __SDIO_CLEAR_FLAG(SDIOx, SDIO_STATIC_CMD_FLAGS);
  
  return SDMMC_ERROR_NONE;
}

sdio_err_t SdioDriverInit(SD_InitTypeDef init_para)
{
	SD_InitTypeDef Init = {0};
  uint32_t resp;
	uint16_t sdio_rca;
	uint8_t bus_width;
	HAL_StatusTypeDef status;
  __HAL_RCC_SDIO_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
	
	__SDIO_ENABLE_IT(SDIO, SDIO_IT_SDIOIT);

    /* Default SDIO peripheral configuration for SD card initialization */
    Init.ClockDiv = SDIO_INIT_CLK_DIV;

    /* Initialize SDIO peripheral interface with default configuration */
    status = SDIO_Init(SDIO, Init);
    if (status != HAL_OK)
    {
        return FAILURE;
    }
		
    /* Disable SDIO Clock */
    __HAL_SD_DISABLE(card);

    /* Set Power State to ON */
    (void)SDIO_PowerState_ON(SDIO);

    /* Enable SDIO Clock */
    __HAL_SD_ENABLE(card);
    /* SDIO interrupt Init */
    HAL_NVIC_SetPriority(SDIO_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);

	  //SDIO_LOGD(TAG, "HCLK : %d\r\n", HAL_RCC_GetHCLKFreq());
	  //HAL_Delay(5000);		
		__SDIO_OPERATION_ENABLE(SDIO); // ENABLE SDIO
  __SDIO_DMA_ENABLE(SDIO); // Enable SDIO DMA
		
	sdio_cmd.Argument         = 0U;
  sdio_cmd.CmdIndex         = 0;
  sdio_cmd.Response         = SDIO_RESPONSE_NO;
  sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
  sdio_cmd.CPSM             = SDIO_CPSM_ENABLE;
  SDIO_SendCommand(SDIO, &sdio_cmd);

	uint32_t errorstate = SDMMC_GetCmdError(SDIO);
	if(errorstate != SDMMC_ERROR_NONE){
		SDIO_LOGD(TAG, "Send cmd0 error\r\n");
	}
  
    /* Send CMD5: IO_SEND_OP_COND */
  sdio_cmd.Argument = 0U;
  sdio_cmd.CmdIndex = 5;
  sdio_cmd.Response = SDIO_RESPONSE_SHORT; // ?????
  sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
  sdio_cmd.CPSM = SDIO_CPSM_ENABLE;
  SDIO_SendCommand(SDIO, &sdio_cmd);
  WaitForResponse(__FUNCTION__);
	HAL_Delay(10);
  
  /* Set VDD Voltage Window: 3.2~3.4V, then resend CMD5 */
  sdio_cmd.Argument = 0x300000;
  SDIO_SendCommand(SDIO, &sdio_cmd);
  WaitForResponse(__FUNCTION__);
  resp = SDIO_GetResponse(SDIO, SDIO_RESP1);

	/* Get WiFi address (CMD3: SEND_RELATIVE_ADDR, Ask the card to publish a new relative address (RCA)) */
  sdio_cmd.Argument = 0;
  sdio_cmd.CmdIndex = 3;
  SDIO_SendCommand(SDIO, &sdio_cmd);
  WaitForResponse(__FUNCTION__);
  sdio_rca = SDIO_GetResponse(SDIO, SDIO_RESP1) >> 16;
  SDIO_LOGD(TAG, "Relative Card Address: 0x%04x\n", sdio_rca);
  
  /* Select WiFi (CMD7: SELECT/DESELECT_CARD) */
  sdio_cmd.Argument = sdio_rca << 16;
  sdio_cmd.CmdIndex = 7;
  SDIO_SendCommand(SDIO, &sdio_cmd);
  WaitForResponse(__FUNCTION__);
  SDIO_LOGD(TAG, "Card selected! RESP1_%08x\n", SDIO_GetResponse(SDIO, SDIO_RESP1));

	// resume SDIO CLK
	status = SDIO_Init(SDIO, init_para);
	if (status != HAL_OK)
	{
			return FAILURE;
	}

	bus_width = STM32ReadReg(0, SD_IO_CCCR_BUS_WIDTH);
	
	if(init_para.BusWide == SDIO_BUS_WIDE_4B){
		SDIO_LOGI(TAG, "Use 4bit bus width");
		bus_width |= CCCR_BUS_WIDTH_4;
	}else if(init_para.BusWide == SDIO_BUS_WIDE_1B){
		SDIO_LOGI(TAG, "Use 1bit bus width");
		bus_width &= ~CCCR_BUS_WIDTH_4;
	} else {
		SDIO_LOGI(TAG, "Illegal bus width");
		return FAILURE;
	}
	STM32WriteReg(0, SD_IO_CCCR_BUS_WIDTH, bus_width);

	return SDIO_SUCCESS;
}

static void SendCMD52(uint8_t func, uint32_t addr, uint8_t data, uint32_t flags)
{
  sdio_cmd.Argument = (func << 28) | (addr << 9) | data | flags;
  sdio_cmd.CmdIndex = 52;
  sdio_cmd.CPSM = SDIO_CPSM_ENABLE;
  sdio_cmd.Response = SDIO_RESPONSE_SHORT;
  sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
  SDIO_SendCommand(SDIO, &sdio_cmd);
}

static void SendCMD53(uint8_t func, uint32_t addr, uint16_t count, uint32_t flags)
{
  sdio_cmd.Argument = (func << 28) | (addr << 9) | (count & 0x1ff) | flags;
  sdio_cmd.CmdIndex = 53;
  sdio_cmd.CPSM = SDIO_CPSM_ENABLE;
  sdio_cmd.Response = SDIO_RESPONSE_SHORT;
  sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
  SDIO_SendCommand(SDIO, &sdio_cmd);
}

// If write, flag = 1; If read, flag = 0 
static uint16_t GetBlockNum(uint8_t func, uint32_t *psize, uint8_t flag)
{
  uint16_t block_num = 0;
  
	if(*psize == 4){
		sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_4B;
	} else if (*psize >= SDIO_BLOCK_SIZE || flag) {
    sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;
    
    block_num = *psize / SDIO_BLOCK_SIZE;
    if (*psize % SDIO_BLOCK_SIZE != 0)
    block_num++;
    *psize = block_num * SDIO_BLOCK_SIZE;
  } else {
		sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_1B;
    *psize = (*psize + 3) & ~3;
  }
  
  return block_num;
}

int STM32WriteData(uint8_t func, uint32_t addr, const void *data, uint32_t size)
{
  int i, err = 0;
  uint16_t block_num = 0;
  uint32_t cmd53_flags = CMD53_WRITE;
  LL_DMA_InitTypeDef dma;
  
  if ((uintptr_t)data & 3)
  {
    SDIO_LOGE(TAG, "%s: data must be 4-byte aligned!\n", __FUNCTION__);
    return -2;
  }
  if (size == 0)
  {
    SDIO_LOGE(TAG, "%s: size cannot be 0!\n", __FUNCTION__);
    return -2;
  }
	xSemaphoreTake(semahandle, portMAX_DELAY);
  block_num = GetBlockNum(func, &size, 1);
  
	cmd53_flags |= CMD53_INCREMENTING;

	__SDIO_OPERATION_DISABLE(SDIO);

	SDIO_LOGV(TAG, "Send size:%d\r\n", size);
  if (block_num)
  {
    sdio_data.TransferMode = SDIO_TRANSFER_MODE_BLOCK;
    SendCMD53(func, addr, block_num, cmd53_flags | CMD53_BLOCKMODE);
  }
  else
  {
		sdio_data.TransferMode = SDIO_TRANSFER_MODE_BLOCK;
    SendCMD53(func, addr, size, cmd53_flags);
  }
  WaitForResponse(__FUNCTION__);
  SDIO_LOGV(TAG, "CMD53 response\r\n");
	
  dma.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  dma.MemoryOrM2MDstAddress = (uint32_t)data;
  dma.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_WORD;
  dma.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  dma.Mode = LL_DMA_MODE_NORMAL;
  dma.NbData = size / 4;
  dma.PeriphOrM2MSrcAddress = (uint32_t)&SDIO->FIFO;
  dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  dma.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  dma.Priority = LL_DMA_PRIORITY_VERYHIGH;
  LL_DMA_Init(DMA2, LL_DMA_CHANNEL_4, &dma);
  LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_4);
  
  sdio_data.DataLength = size;
  sdio_data.DPSM = SDIO_DPSM_ENABLE;
  sdio_data.TransferDir = SDIO_TRANSFER_DIR_TO_CARD;
  SDIO_ConfigData(SDIO, &sdio_data);

  // Wait send done
  i = 0;
  while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_DATAEND) == RESET)
  {
    err += CheckError(__FUNCTION__);
    if (err)
      break;
    
    i++;
    if (i == CMD53_TIMEOUT)
    {
      SDIO_LOGD(TAG, "%s: timeout!\n", __FUNCTION__); 
      err++;
      break;
    }
  }

  sdio_data.DPSM = SDIO_DPSM_DISABLE;
  SDIO_ConfigData(SDIO, &sdio_data);

  LL_DMA_ClearFlag_GI4(DMA2); // Clear DMA flag
  LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_4); // Close DMA
  
  __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_DATAEND | SDIO_FLAG_DBCKEND);

	xSemaphoreGive(semahandle);
  err += CheckError(__FUNCTION__);
  if (err != 0)
    return -1;
  return 0;
}

/* Using CMD52 Write reg, return the result*/
uint8_t STM32WriteReg(uint8_t func, uint32_t addr, uint8_t value)
{
  SendCMD52(func, addr, value, CMD52_WRITE | CMD52_READAFTERWRITE);
  WaitForResponse(__FUNCTION__);
  return SDIO_GetResponse(SDIO, SDIO_RESP1) & 0xff;
}

/* Read reg */
uint8_t STM32ReadReg(uint8_t func, uint32_t addr)
{
  SendCMD52(func, addr, 0, 0);
  WaitForResponse(__FUNCTION__);
  return SDIO_GetResponse(SDIO, SDIO_RESP1) & 0xff;
}

int STM32ReadData(uint8_t func, uint32_t addr, void *data, uint32_t size)
{
  int i, err = 0;
  uint16_t block_num = 0;
  uint32_t cmd53_flags = 0;
  LL_DMA_InitTypeDef dma;
  
  if ((uintptr_t)data & 3)
  {
    SDIO_LOGE(TAG, "%s: data must be 4-byte aligned!\n", __FUNCTION__);
    return -2; 
  }
  if (size == 0)
  {
    SDIO_LOGE(TAG, "%s: size cannot be 0!\n", __FUNCTION__);
    return -2;
  }
	
	xSemaphoreTake(semahandle, portMAX_DELAY);

  block_num = GetBlockNum(func, &size, 0);

	SDIO_LOGV(TAG, "Recv size:%d\r\n", size);

  dma.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  dma.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_WORD;
  dma.Mode = LL_DMA_MODE_NORMAL;
  dma.NbData = size / 4;
  dma.PeriphOrM2MSrcAddress = (uint32_t)&SDIO->FIFO;
  dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  dma.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  dma.Priority = LL_DMA_PRIORITY_VERYHIGH;
  dma.MemoryOrM2MDstAddress = (uint32_t)data;
  dma.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;

  LL_DMA_Init(DMA2, LL_DMA_CHANNEL_4, &dma);
  LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_4);

  cmd53_flags |= CMD53_INCREMENTING;
	
  if (block_num)
  {
    sdio_data.TransferMode = SDIO_TRANSFER_MODE_BLOCK;
    SendCMD53(func, addr, block_num, cmd53_flags | CMD53_BLOCKMODE);
  }
  else
  {
		// Read can use STREAM mode , not check CRC
    sdio_data.TransferMode = SDIO_TRANSFER_MODE_STREAM;
    SendCMD53(func, addr, size, cmd53_flags);
  }
  
  sdio_data.DataLength = size;
  sdio_data.DPSM = SDIO_DPSM_ENABLE;
  sdio_data.TransferDir = SDIO_TRANSFER_DIR_TO_SDIO;
  SDIO_ConfigData(SDIO, &sdio_data);
  
  i = 0;
  while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CMDACT) != RESET || __SDIO_GET_FLAG(SDIO, SDIO_FLAG_DATAEND) == RESET)
  {
    err += CheckError(__FUNCTION__);
    if (err)
      break;
    
    i++;
    if (i == CMD53_TIMEOUT)
    {
      SDIO_LOGE(TAG, "%s: timeout!\n", __FUNCTION__);
      err++;
      break;
    }
  }
	// Wait 7 clock to write DPSM (STM32F10X SPEC SDIO_DCTRL), avoid losing last data
	uint8_t delay = 100;
	while(delay--){
	}
	sdio_data.DPSM = SDIO_DPSM_DISABLE;
  SDIO_ConfigData(SDIO, &sdio_data);
	
  LL_DMA_ClearFlag_GI4(DMA2);
  LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_4); 

  __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CMDREND | SDIO_FLAG_DATAEND | SDIO_FLAG_DBCKEND);

	// Transmit data, need enable intr
	if(addr > ESP_DATA_MIN_ADDRESS) {
		SDIO_LOGD(TAG, "Addr:0x%x,len:%d", addr, size);
		__SDIO_ENABLE_IT(SDIO, SDIO_FLAG_SDIOIT);
		__SDIO_OPERATION_ENABLE(SDIO); // ENABLE SDIO
		// Check SDIO interrupt
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == RESET){    // D1 if low, means SDIO interrupt
			SDIO_LOGD(TAG, "%s: give!\n", __FUNCTION__);
			xSemaphoreGive(sdio_recv_SemHandle);
		} else {
			SDIO_LOGD(TAG, "No intr");
		}
	}

	xSemaphoreGive(semahandle);
  err += CheckError(__FUNCTION__);
  if (err != 0)
    return -1;
  return 0;
}

/* SDIO init function */
void STM32SdioInit(sdio_init_t sdio_init)
{
	uint32_t freq;
	SD_InitTypeDef Init;
	SdioGpioInit();
	semahandle = xSemaphoreCreateMutex();
	Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	if(sdio_init.width == WIDTH_4){
		Init.BusWide = SDIO_BUS_WIDE_4B;
	} else {
    Init.BusWide = SDIO_BUS_WIDE_1B;
	}
  Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  Init.ClockDiv = CalcClockDivider(sdio_init.clock, &freq);
	sdio_data.DataTimeOut = freq ;    // Data timeout is 1s
  if (SdioDriverInit(Init) != SDIO_SUCCESS)
  {
    SDIO_LOGE(TAG, "STM32 SDIO driver init error");
		return;
  }	
}

uint8_t STM32WaitIntr(uint32_t timeout)
{
	__SDIO_DISABLE_IT(SDIO, SDIO_FLAG_SDIOIT);    //disable GPIO intrrupt
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == RESET){
		SDIO_LOGD(TAG, "Read INT");
		return 0;
	}
	xSemaphoreTake(sdio_recv_SemHandle, 0);
	__SDIO_ENABLE_IT(SDIO, SDIO_FLAG_SDIOIT);
	__SDIO_OPERATION_ENABLE(SDIO); // ENABLE SDIO
	if(xSemaphoreTake(sdio_recv_SemHandle, timeout) != pdPASS){
		return 1;
	} else {
		/* Receive a SDIO intr, first close it, and then reopen after receive or send done data*/
		__SDIO_DISABLE_IT(SDIO, SDIO_FLAG_SDIOIT);
		__SDIO_OPERATION_DISABLE(SDIO); 
	  return 0;
	}
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  hsd: Pointer to SD handle
  * @retval None
  */
void SDIO_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken;
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	SDIO_LOGD(TAG, "INT:0x%x, DCTRL:0x%x\r\n", SDIO->STA, SDIO->DCTRL);
	if(GetITStatus()){
		xSemaphoreGiveFromISR(sdio_recv_SemHandle,&xHigherPriorityTaskWoken);	
		if(xHigherPriorityTaskWoken) {
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
