#include "FreeRTOS.h"
#include "task.h"

void platform_os_delay(uint32_t milliseconds)
{
    vTaskDelay(milliseconds);
}