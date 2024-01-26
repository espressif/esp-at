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

#if defined(CONFIG_ESP32_RTC_CLK_SRC_EXT_CRYS) || defined(CONFIG_RTC_CLK_SRC_EXT_CRYS)
#include "soc/rtc.h"
static void at_disable_rtc_clk_32k_if_no_ext_crys(void)
{
    if (rtc_clk_slow_freq_get() != RTC_SLOW_FREQ_32K_XTAL) {
        rtc_clk_32k_enable(false);
        printf("no external 32k oscillator, disable it now.\r\n");
    } else {
        printf("external 32k oscillator already started.\r\n");
    }
}
#endif

void esp_at_main_preprocess(void)
{
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

#if defined(CONFIG_ESP32_RTC_CLK_SRC_EXT_CRYS) || defined(CONFIG_RTC_CLK_SRC_EXT_CRYS)
    /**
     * A workaround for external 32 KHz oscillator.
     *
     * esp-at enabled BLE light sleep by default, which requires an external 32 KHz oscillator,
     * so that XTAL_32K_P(32K_XP) and XTAL_32K_N(32K_XN) can not used for common GPIOs
     * even if there is no a physically external 32 KHz oscillator.
     * More details are in RTC clock part of <esp_hardware_design_guidelines_en.pdf>.
     *
     * In order to make XTAL_32K_P(32K_XP) and XTAL_32K_N(32K_XN) can be used for common GPIOs,
     * esp-at will disable external rtc clock if there is no a physically external 32 KHz oscillator.
     */
    at_disable_rtc_clk_32k_if_no_ext_crys();
#endif

    ESP_LOGD(TAG, "at main pre-process done");
}
