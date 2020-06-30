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
#include "sdio_host_log.h"
#include "sdio_host_transport.h"
#include "sdio_config.h"

#include "time.h"

#if (TARGET_ESP32 == 0)
#include "guide_8266_boot.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 #define RXBUFFERSIZE 256
char RxBuffer[RXBUFFERSIZE];
char RxBufferBackup[RXBUFFERSIZE];
uint8_t aRxBuffer;
uint32_t Uart1_Rx_Cnt = 0;
uint32_t Uart1_Rx_Cnt_Backup = 0;
static const char* TAG = "sdhost";
static SemaphoreHandle_t UartBinarySemHandle;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define WRITE_BUFFER_LEN    2048
#define READ_BUFFER_LEN     4096

uint8_t send_buffer[WRITE_BUFFER_LEN] = "";
uint8_t rcv_buffer[READ_BUFFER_LEN] = "";

int sdio_debugLevel = 2;

#define SDIO_ERROR_CHECK(x) do {                                         \
        sdio_err_t __err_rc = (x);                                       \
        if (__err_rc != SDIO_SUCCESS) {                                       \
            SDIO_LOGE(TAG, "file: \"%s\" line %d\nfunc: %s\n error: %d\n", __FILE__, __LINE__,    \
                      __FUNCTION__, x);                                 \
        }                                                               \
    } while(0);

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void sdio_recv_task(void* pvParameters);
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
  /* add threads, ... */
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
	  sdio_err_t err;
	  UartBinarySemHandle = xSemaphoreCreateBinary();
    SDIO_LOGI(TAG, "host ready, start initializing slave...");
	
    err = sdio_host_init();
    if(err != SDIO_SUCCESS){
			printf("SDIO init error\r\n");
		}
		printf("Sdio init done\r\n");
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

#if (TARGET_ESP32 == 0)
    SDIO_LOGI(TAG, "********** Guide ESP8266 BOOT ***********");
    err = esp_download_fw();
    SDIO_ERROR_CHECK(err);
#endif
	  xTaskCreate(sdio_recv_task, "sdioRecvTask", 128, NULL, 5, NULL);

		xSemaphoreTake(UartBinarySemHandle,0);
	  // wakeup uart receive intr
	  HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
  /* Infinite loop */
  for(;;)
  {
		xSemaphoreTake(UartBinarySemHandle,portMAX_DELAY);
		
		SDIO_LOGD(TAG, "UART len: %d", Uart1_Rx_Cnt_Backup);
		RxBufferBackup[Uart1_Rx_Cnt_Backup] = '\0';
		SDIO_LOGD(TAG, "UART DATA: %s", RxBufferBackup);
		
		err = sdio_host_send_packet(RxBufferBackup, Uart1_Rx_Cnt_Backup);
		// Send timeout
		if (err == ERR_TIMEOUT) {
				SDIO_LOGW(TAG, "send timeout");
		}

		//memset(send_buffer, '\0', sizeof(send_buffer))
  }
	vTaskDelete(NULL);
  /* USER CODE END StartDefaultTask */
}



/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void sdio_recv_task(void* pvParameters)
{
    while (1) {
			  /* Do not set too long wait time */
        sdio_err_t ret = sdio_host_wait_int(100 / portTICK_RATE_MS);

        if (ret != SDIO_SUCCESS) {
            continue;
        }
				
        uint32_t intr_raw, intr_st;
        ret = sdio_host_get_intr(&intr_raw, &intr_st);
        SDIO_ERROR_CHECK(ret);

        if (intr_st == 0) {
            //SDIO_LOGW(TAG, "No intr\r\n");
            continue;
        }
				
        ret = sdio_host_clear_intr(intr_raw);
        SDIO_ERROR_CHECK(ret);
        //SDIO_LOGD(TAG, "intr raw: %x, intr_st: %x", intr_raw, intr_st);

        const int wait_ms = 50;

        if (intr_raw & HOST_SLC0_RX_NEW_PACKET_INT_ST) {
            SDIO_LOGD(TAG, "new packet coming");

            while (1) {
                size_t size_read = READ_BUFFER_LEN;
                ret = sdio_host_get_packet(rcv_buffer, READ_BUFFER_LEN, &size_read, wait_ms);
							  SDIO_LOGD(TAG, "Read datalen: %d", size_read);

                if (ret == ERR_NOT_FOUND) {
                    SDIO_LOGE(TAG, "interrupt but no data can be read");
                    break;
                } else if (ret != SDIO_SUCCESS && ret != ERR_NOT_FINISHED) {
                    SDIO_LOGE(TAG, "rx packet error: %08X", ret);
                    break;
                }

                {
                    printf("%s\n", rcv_buffer);
                    fflush(stdout);
                }

                memset(rcv_buffer, 0x0, sizeof(rcv_buffer));

                if (ret == SDIO_SUCCESS) {
                    break;
                }
            }
        }
    }

    vTaskDelete(NULL);
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
