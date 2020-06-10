#ifndef SDIO_CONFIG_H_
#define SDIO_CONFIG_H_

#include "sdkconfig.h"

#if CONFIG_SLAVE_ESP32
#define TARGET_ESP32  1
#elif CONFIG_SLAVE_ESP8266
#define TARGET_ESP32  0
#endif

#endif /* SDIO_CONFIG_H_ */
