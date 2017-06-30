/*
 * File    : user_main.c
 * This file is part of Espressif's AT+ command set program.
 *
 * Copyright (c) 2013-2014 Espressif Systems
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
//#include "ets_sys.h"
#include "at.h"
#include "at_ipCmd.h"
#include "lwip/ip4_addr.h"
#include "at_nvm.h"

#ifdef MEMLEAK_DEBUG
static const char mem_debug_file[] ICACHE_RODATA_ATTR = __FILE__;
#endif

extern void trans_client_init(at_nvm_trans_config_struct *trans_config);
extern bool at_set_dhcp(uint8_t dhcp_enable, uint8_t dhcp_mode);

void at_nvm_init(void)
{
    tcpip_adapter_ip_info_t adapter_ip_info;
    uint8_t loop = 0;
    wifi_mode_t wifi_mode = WIFI_MODE_NULL;
    dhcps_lease_t lease;
    at_nvm_struct nvm;
    ip_addr_t ip_address;

    if (esp_wifi_get_mode(&wifi_mode) != ESP_OK) {
        wifi_mode = WIFI_MODE_AP;
    }
    esp_wifi_set_mode(WIFI_MODE_APSTA);
    //default broadcast netif
    //(void)wifi_set_broadcast_if(3);
    // sta mac
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_station_mac_config_get(&nvm.mac_conig)) {
        for (loop = 0; loop < 6; loop++) {
            if (nvm.mac_conig.mac[loop] != 0xFF) { // FF:FF:FF:FF:FF:FF
                break;
            }
        }

        if (loop < 6) {
            for (loop = 0; loop < 6; loop++) {
                if (nvm.mac_conig.mac[loop] != 0x00) { // 00:00:00:00:00:00
                    break;
                }
            }

            if (loop < 6) {
                esp_wifi_set_mac(WIFI_IF_STA, nvm.mac_conig.mac);
            }
        }
    }
#ifdef AT_DHCP_SUPPORT
    // stop dhcp
    at_set_dhcp(0, 3);
#endif
    // sta ip
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_station_ip_config_get(&nvm.ip_config)) {
        if ((nvm.ip_config.ip_addr != IPADDR_NONE) && (nvm.ip_config.ip_addr != IPADDR_ANY)) {
            if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &adapter_ip_info) == ESP_OK) {
                adapter_ip_info.ip.addr = nvm.ip_config.ip_addr;
                adapter_ip_info.gw.addr = nvm.ip_config.gateway;
                adapter_ip_info.netmask.addr = nvm.ip_config.netmask;

                tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &adapter_ip_info);
            }
        }
    }

    // ap mac
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_softap_mac_config_get(&nvm.mac_conig)) {
        for (loop = 0; loop < 6; loop++) {
            if (nvm.mac_conig.mac[loop] != 0xFF) { // FF:FF:FF:FF:FF:FF
                break;
            }
        }

        if (loop < 6) {
            for (loop = 0; loop < 6; loop++) {
                if (nvm.mac_conig.mac[loop] != 0x00) { // 00:00:00:00:00:00
                    break;
                }
            }

            if (loop < 6) {
                (void)esp_wifi_set_mac(WIFI_IF_AP, nvm.mac_conig.mac);
            }
        }
    }

    // ap ip
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_softap_ip_config_get(&nvm.ip_config)) {
        if ((nvm.ip_config.ip_addr != IPADDR_NONE) && (nvm.ip_config.ip_addr != IPADDR_ANY)) {
            if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info) == ESP_OK) {
                adapter_ip_info.ip.addr = nvm.ip_config.ip_addr;
                adapter_ip_info.gw.addr = nvm.ip_config.gateway;
                adapter_ip_info.netmask.addr = nvm.ip_config.netmask;

                tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &adapter_ip_info);
            }
        }
    }

#ifdef AT_DHCP_SUPPORT
    // set dhcp server pool
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_dhcps_config_get(&nvm.dhcps_config)) {
        if ((nvm.dhcps_config.lease_time > 0) && (nvm.dhcps_config.lease_time <= 2880) && (nvm.dhcps_config.enable == 1)) {
            lease.start_ip.addr = nvm.dhcps_config.start_ip;
            lease.end_ip.addr = nvm.dhcps_config.end_ip;
            lease.enable = TRUE;

            tcpip_adapter_dhcps_option(TCPIP_ADAPTER_OP_SET, TCPIP_ADAPTER_REQUESTED_IP_ADDRESS, &lease, sizeof(lease));
            tcpip_adapter_dhcps_option(TCPIP_ADAPTER_OP_SET, TCPIP_ADAPTER_IP_ADDRESS_LEASE_TIME, &nvm.dhcps_config.lease_time, sizeof(nvm.dhcps_config.lease_time));
        }
    }
    // dhcp
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_dhcp_config_get(&nvm.dhcp_config)) {
        at_set_dhcp(1, nvm.dhcp_config.dhcp_mode);
    } else {
        at_set_dhcp(1, 0x03);
    }
#endif
    esp_wifi_set_mode(wifi_mode);

    at_memset(&nvm,0x0,sizeof(nvm));
    if (at_nvm_dns_server_get(&nvm.dns_server_config) == true) {        
        for (loop = 0;loop < DNS_MAX_SERVERS;loop++) {
            ip_address.u_addr.ip4.addr = nvm.dns_server_config.dns_server[loop];
            if (ip_address.u_addr.ip4.addr != IPADDR_ANY) {
                dns_setserver(loop,&ip_address);
            }
        }
    }

    // transparently transmit
    at_memset(&nvm, 0x0, sizeof(nvm));
    if (at_nvm_trans_config_get(&nvm.trans_config)) {
        trans_client_init(&nvm.trans_config);
    }
}
