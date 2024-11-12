/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "esp_log.h"

static const char *TAG = "at-workaround";

#ifdef CONFIG_IDF_TARGET_ESP32
#include "hal/wdt_hal.h"
static void at_disable_rtc_wdt(void)
{
    /*
     * At this point, the flashboot protection of RWDT will have been
     * automatically enabled. We can disable flashboot protection as it's not
     * needed anymore.
     */
    // Disable RWDT flashboot protection.
    wdt_hal_context_t rtc_wdt_ctx = {.inst = WDT_RWDT, .rwdt_dev = &RTCCNTL};
    wdt_hal_write_protect_disable(&rtc_wdt_ctx);
    wdt_hal_set_flashboot_en(&rtc_wdt_ctx, false);
    wdt_hal_write_protect_enable(&rtc_wdt_ctx);
}
#endif

#ifdef CONFIG_AT_PROTECT_FIRST_32KB_FLASH
void at_protect_first_32kb_flash(void)
{
    bool special_flash_write_status(uint8_t command, uint32_t status, int len, bool write_en);
    bool special_flash_read_status(uint8_t command, uint32_t* status, int len);
    uint32_t spi_flash_get_id(void);

    typedef int (*__ets_printf_t)(const char *fmt, ...); 
    #define ROM_PRINTF(_fmt, ...)   ((__ets_printf_t)(0x400024cc))(_fmt, ##__VA_ARGS__)

    uint32_t flash_id = spi_flash_get_id();
    ROM_PRINTF("flash_id=0x%x\r\n", flash_id);
    if (flash_id == 0x1540a1) {
        uint32_t status[2];
        memset(status, 0x0, sizeof(status));
        special_flash_read_status(0x05, &status[0], 1);
        special_flash_read_status(0x35, &status[1], 1);

        ROM_PRINTF("status=0x%x\r\n", status[0] | (status[1]<<8));
        if ((status[0] != 0xf0) || (status[1] != 0x03)) {
            ROM_PRINTF("status2=0x%x\r\n", status[0] | (status[1]<<8));
            special_flash_write_status(0x01, 0xf0, 1, true);
            special_flash_write_status(0x31, 0x03, 1, true);
        }
    }
}
#endif

void esp_at_main_preprocess(void)
{
#ifdef CONFIG_AT_PROTECT_FIRST_32KB_FLASH
    /**
     * A workaround for the flash protection, description as the following:
     * 
     * If you use FM25Q16A flash, and you need to protect the first 32KB flash (bootloader),
     * you should enable this option.
    */
    at_protect_first_32kb_flash();
#endif

#ifdef CONFIG_IDF_TARGET_ESP32
    /**
     * A workaround for ota compatibility issue, description as the following:
     *
     * If esp32 upgraded from v1.1.3.0 to v2.2.0.0+, after the upgraded done,
     * then each call the esp_restart() will trigger the restart twice.
     * The first is normal SW_CPU_RESET, the second is extra RTCWDT_RTC_RESET.
     * (it could happen if upgraded from an old firmware to the new firmware)
     *
     * Here, disable RWDT to avoid the second restart (RTCWDT_RTC_RESET).
     */
    at_disable_rtc_wdt();
#endif

    ESP_LOGD(TAG, "at main pre-process done");
}
