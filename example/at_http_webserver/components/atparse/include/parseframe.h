#ifndef PARSEFRAME_H
#define PARSEFRAME_H

#include<stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"

#include"atparse.h"

#define CONFIG_AT_BASE_COMMAND_SUPPORT 1

#define CONFIG_AT_WIFI_COMMAND_SUPPORT 1

#define CONFIG_AT_NET_COMMAND_SUPPORT 1

#define CONFIG_AT_BLE_COMMAND_SUPPORT 1

int8_t call_lib(uint8_t* data, int32_t len);

#endif
