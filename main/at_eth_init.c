/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "tcpip_adapter.h"

#ifdef CONFIG_AT_ETHERNET_SUPPORT

#ifdef CONFIG_PHY_LAN8720
#include "eth_phy/phy_lan8720.h"
#define DEFAULT_ETHERNET_PHY_CONFIG phy_lan8720_default_ethernet_config
#endif
#ifdef CONFIG_PHY_TLK110
#include "eth_phy/phy_tlk110.h"
#define DEFAULT_ETHERNET_PHY_CONFIG phy_tlk110_default_ethernet_config
#endif

#define PIN_PHY_POWER CONFIG_PHY_POWER_PIN
#define PIN_SMI_MDC   CONFIG_PHY_SMI_MDC_PIN
#define PIN_SMI_MDIO  CONFIG_PHY_SMI_MDIO_PIN

#ifdef CONFIG_PHY_USE_POWER_PIN
/* This replaces the default PHY power on/off function with one that
   also uses a GPIO for power on/off.

   If this GPIO is not connected on your device (and PHY is always powered), you can use the default PHY-specific power
   on/off function rather than overriding with this one.
*/
static void phy_device_power_enable_via_gpio(bool enable)
{
    assert(DEFAULT_ETHERNET_PHY_CONFIG.phy_power_enable);

    if (!enable) {
        /* Do the PHY-specific power_enable(false) function before powering down */
        DEFAULT_ETHERNET_PHY_CONFIG.phy_power_enable(false);
    }

    gpio_pad_select_gpio(PIN_PHY_POWER);
    gpio_set_direction(PIN_PHY_POWER,GPIO_MODE_OUTPUT);
    if(enable == true) {
        gpio_set_level(PIN_PHY_POWER, 1);
        printf("phy_device_power_enable(TRUE)");
    } else {
        gpio_set_level(PIN_PHY_POWER, 0);
        printf("power_enable(FALSE)");
    }

    // Allow the power up/down to take effect, min 300us
    vTaskDelay(1);

    if (enable) {
        /* Run the PHY-specific power on operations now the PHY has power */
        DEFAULT_ETHERNET_PHY_CONFIG.phy_power_enable(true);
    }
}
#endif

static void eth_gpio_config_rmii(void)
{
    phy_rmii_configure_data_interface_pins();
    phy_rmii_smi_configure_pins(PIN_SMI_MDC, PIN_SMI_MDIO);
}

bool at_eth_init(void)
{
    esp_err_t ret = ESP_OK;
    eth_config_t config = DEFAULT_ETHERNET_PHY_CONFIG;
    /* Set the PHY address in the configuration */
    config.phy_addr = CONFIG_PHY_ADDRESS;
    config.gpio_config = eth_gpio_config_rmii;
    config.tcpip_input = tcpip_adapter_eth_input;
    config.clock_mode = CONFIG_PHY_CLOCK_MODE;

#ifdef CONFIG_PHY_USE_POWER_PIN
    /* Replace the default 'power enable' function with an specific
       one that toggles a power GPIO. */
    config.phy_power_enable = phy_device_power_enable_via_gpio;
#endif

    ret = esp_eth_init(&config);

    if(ret == ESP_OK) {
        esp_eth_enable();
        return true;
    }

    return false;
}
#endif

