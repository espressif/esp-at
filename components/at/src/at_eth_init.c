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
#include "esp_netif.h"

#ifdef CONFIG_AT_ETHERNET_SUPPORT
#include "string.h"
#include "esp_eth.h"

#define CONFIG_AT_ETH_MDC_GPIO         23
#define CONFIG_AT_ETH_MDIO_GPIO        18
#define CONFIG_AT_ETH_PHY_RST_GPIO     5
#define CONFIG_AT_ETH_PHY_ADDR         1

bool esp_at_get_eth_default_config(esp_eth_config_t* config)
{
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();

    phy_config.phy_addr = CONFIG_AT_ETH_PHY_ADDR;
    phy_config.reset_gpio_num = CONFIG_AT_ETH_PHY_RST_GPIO;
    eth_esp32_emac_config_t esp32_emac_config = ETH_ESP32_EMAC_DEFAULT_CONFIG();
    esp32_emac_config.smi_mdc_gpio_num = CONFIG_AT_ETH_MDC_GPIO;
    esp32_emac_config.smi_mdio_gpio_num = CONFIG_AT_ETH_MDIO_GPIO;
    esp_eth_mac_t *mac = esp_eth_mac_new_esp32(&esp32_emac_config, &mac_config);

#if CONFIG_PHY_IP101
    esp_eth_phy_t *phy = esp_eth_phy_new_ip101(&phy_config);
#elif CONFIG_PHY_RTL8201
    esp_eth_phy_t *phy = esp_eth_phy_new_rtl8201(&phy_config);
#elif CONFIG_PHY_LAN8720
    esp_eth_phy_t *phy = esp_eth_phy_new_lan8720(&phy_config);
#elif CONFIG_PHY_DP83848
    esp_eth_phy_t *phy = esp_eth_phy_new_dp83848(&phy_config);
#endif

    esp_eth_config_t eth_config = ETH_DEFAULT_CONFIG(mac, phy);
    memcpy(config, &eth_config, sizeof(esp_eth_config_t));
    
    return true;
}
#endif
