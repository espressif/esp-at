#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void platform_os_delay(uint32_t milliseconds)
{
    vTaskDelay(milliseconds);
}