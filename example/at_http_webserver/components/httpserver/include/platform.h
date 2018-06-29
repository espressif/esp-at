
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdkconfig.h"
#include "stdint.h"
#include "esp_types.h"
#include "esp_attr.h"
#include "esp_spi_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#define FREERTOS 1
#define ESP32 1
#define ICACHE_RODATA_ATTR

#define HTTPD_MAX_CONNECTIONS 5
#define HTTPD_STACKSIZE 1024

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef struct RtosConnType RtosConnType;
typedef RtosConnType* ConnTypePtr;
typedef TimerHandle_t HttpdPlatTimerHandle;

#define httpd_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define ICACHE_FLASH_ATTR



