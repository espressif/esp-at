#pragma once

#include "FreeRTOS.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

//MVA leave only necessary definitions

typedef QueueHandle_t SemaphoreHandle_t;

SemaphoreHandle_t xSemaphoreCreateMutex();
void vSemaphoreDelete( SemaphoreHandle_t xSemaphore );
BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xBlockTime );
BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore );

#ifdef __cplusplus
}
#endif




