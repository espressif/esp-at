/* ESP32 simple memory monitor function(s)
   August 2017
*/

// System
#include <string.h>
#include <netdb.h>
#include "stdbool.h"

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// ESP-IDF
#include "esp_system.h"
#include "esp_log.h"
#include "esp_heap_alloc_caps.h"

#define KBYTE 1024

static const char *TAG = "mem.c";

void checkmem( char* tag )
{
    ESP_LOGW(TAG, "----------memcheck: %s----------", tag);
    ESP_LOGW(TAG, "Free MALLOC_CAP_8BIT: %d", xPortGetFreeHeapSizeCaps(MALLOC_CAP_8BIT)/KBYTE);
    ESP_LOGW(TAG, "Free MALLOC_CAP_32BIT: %d", xPortGetFreeHeapSizeCaps(MALLOC_CAP_32BIT)/KBYTE);
	ESP_LOGW(TAG, "Lowest Ever Free: 8-Bit: %d, 32-Bit: %d", xPortGetMinimumEverFreeHeapSizeCaps(MALLOC_CAP_8BIT)/KBYTE,
	                                                         xPortGetMinimumEverFreeHeapSizeCaps(MALLOC_CAP_32BIT)/KBYTE);
    ESP_LOGW(TAG, "----------end memcheck: %s----------", tag);
}