#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

extern void at_task_init(void);
void app_main() 
{
    // system_init();
    at_task_init();
}

