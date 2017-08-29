/* Basic system level utilities
   August 2017
*/

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ESP-IDF
#include "esp_system.h"

void delay_minutes( int i )
{
    for(; i>0; i--)
    {
        vTaskDelay(60000);    
    }
}

int device_reboot(void)
{
    esp_restart();
    return 1;
}
