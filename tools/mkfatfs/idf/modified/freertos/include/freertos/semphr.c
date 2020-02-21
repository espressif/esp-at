
#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

//MVA emulate semaphore functions

SemaphoreHandle_t xSemaphoreCreateMutex() {return (SemaphoreHandle_t)1;}
void vSemaphoreDelete( SemaphoreHandle_t xSemaphore ) {}
BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xBlockTime ) {return pdTRUE;}
BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore ) {return pdTRUE;}

#ifdef __cplusplus
}
#endif


