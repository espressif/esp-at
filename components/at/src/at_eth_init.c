/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
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
    esp_eth_phy_t *phy = esp_eth_phy_new_lan87xx(&phy_config);
#elif CONFIG_PHY_DP83848
    esp_eth_phy_t *phy = esp_eth_phy_new_dp83848(&phy_config);
#endif

    esp_eth_config_t eth_config = ETH_DEFAULT_CONFIG(mac, phy);
    memcpy(config, &eth_config, sizeof(esp_eth_config_t));

    return true;
}
#endif
