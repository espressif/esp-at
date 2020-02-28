/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include <string.h>
#include <stdio.h>
#include "task.h"
#include "stream_buffer.h"
#include "semphr.h"
#include "usart.h"
#include "gpio.h"
#include "port.h"
#include "sdspi_host.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 #define RXBUFFERSIZE 256
char RxBuffer[RXBUFFERSIZE];
char RxBufferBackup[RXBUFFERSIZE];
uint8_t aRxBuffer;
uint32_t Uart1_Rx_Cnt = 0;
uint32_t Uart1_Rx_Cnt_Backup = 0;
static const char* TAG = "sdspi";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* List of commands */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define READ_BUFFER_LEN     4096
static uint8_t esp_at_recvbuf[READ_BUFFER_LEN + 1] = "";
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
spi_context_t context;
static SemaphoreHandle_t UartBinarySemHandle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void sdspi_recv_task(void * argument);
void spi_transmit_data(uint32_t len);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	UartBinarySemHandle = xSemaphoreCreateBinary();
	esp_err_t err = at_sdspi_init();
  assert(err == ESP_OK);
  memset(&context, 0x0, sizeof(spi_context_t));
  xTaskCreate(sdspi_recv_task, "sdspi_recv_task", 128, NULL, 5, NULL);
	xSemaphoreTake(UartBinarySemHandle,0);
	CS_high();
	// wakeup uart receive intr
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);

	ESP_AT_LOGI(TAG, "Init SDIO done, now start send AT command\r\n");
  /* Infinite loop */
  for(;;)
  {
		xSemaphoreTake(UartBinarySemHandle,portMAX_DELAY);
/*		
		ESP_AT_LOGD(TAG, "UART len: %d\r\n", Uart1_Rx_Cnt_Backup);
		RxBufferBackup[Uart1_Rx_Cnt_Backup] = '\0';
		ESP_AT_LOGD(TAG, "UART DATA: %s", RxBufferBackup);
*/
		err = at_sdspi_send_packet(&context, RxBufferBackup, Uart1_Rx_Cnt_Backup, UINT32_MAX);
		if (err != ESP_OK) {
			ESP_AT_LOGE(TAG, "Send SDIO data error\r\n");
			while(1){};
		}
		ESP_AT_LOGD(TAG, "\r\nSend done\r\n");
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void sdspi_recv_task(void * argument)
{
    esp_err_t ret;
    uint32_t intr_raw;

    while (1) {

        ret = at_spi_wait_int(2000);

        if (ret == ESP_ERR_TIMEOUT) {
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
                ESP_AT_LOGE(TAG, "interrupt but no data can be read\r\n");
                break;
            } else if (err != ESP_OK && err != ESP_ERR_NOT_FINISHED) {
                ESP_AT_LOGE(TAG, "rx packet error: %08X\r\n", err);
            }

            {
                printf("%s", esp_at_recvbuf);
                fflush(stdout);
            }

            memset(esp_at_recvbuf, '\0', sizeof(esp_at_recvbuf));
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
	BaseType_t xHigherPriorityTaskWoken;
	
	if(Uart1_Rx_Cnt >= 254)  // copy before over flow
	{
		Uart1_Rx_Cnt_Backup = Uart1_Rx_Cnt;
		memcpy(RxBufferBackup, RxBuffer, Uart1_Rx_Cnt);
		xSemaphoreGiveFromISR(UartBinarySemHandle,&xHigherPriorityTaskWoken);	
		Uart1_Rx_Cnt = 0;
		memset(RxBuffer,0x00,sizeof(RxBuffer));	
        
	} else {
		RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;   //
	
		if((RxBuffer[Uart1_Rx_Cnt-1] == 0x0A)&&(RxBuffer[Uart1_Rx_Cnt-2] == 0x0D)) 
		{
      Uart1_Rx_Cnt_Backup = Uart1_Rx_Cnt;
		  memcpy(RxBufferBackup, RxBuffer, Uart1_Rx_Cnt);
			xSemaphoreGiveFromISR(UartBinarySemHandle,&xHigherPriorityTaskWoken);
			Uart1_Rx_Cnt = 0;
			memset(RxBuffer,0x00,sizeof(RxBuffer)); 
		}
	}
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); 
	
	if(xHigherPriorityTaskWoken) {
	  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
