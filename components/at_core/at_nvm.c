
/*
 * File : nv_data.c
 * This file is part of Espressif's non-volatile data operate.
 * Copyright (C) 2013 - 2016, Espressif Systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "string.h"
#include "at.h"
#include "at_nvm.h"
#include "nvs.h"
#include "lwip/ip4_addr.h"
#include "esp_partition.h"

static const char *at_nvm_ap_ip_name           = "NVM_001";
static const char *at_nvm_sta_ip_name          = "NVM_002";
static const char *at_nvm_ap_mac_name          = "NVM_003";
static const char *at_nvm_sta_mac_name         = "NVM_004";
static const char *at_nvm_trans_config_name    = "NVM_005";
static const char *at_nvm_dhcp_config_name     = "NVM_006";
static const char *at_nvm_dhcps_config_name    = "NVM_007";
static const char *at_nvm_dns_server_name      = "NVM_008";

bool at_nvm_softap_ip_config_set (at_nvm_ip_config_struct *ip_config)
{
    nvs_handle handle;
    if (ip_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_ap_ip_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "ip", ip_config->ip_addr) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "gw", ip_config->gateway) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "netmask", ip_config->netmask) != ESP_OK) {
        return false;
    }
    nvs_close(handle);

    return true;
}

bool at_nvm_softap_ip_config_get (at_nvm_ip_config_struct *ip_config)
{
    nvs_handle handle;
    if (ip_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_ap_ip_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }

    if (nvs_get_u32(handle, "ip", &ip_config->ip_addr) != ESP_OK) {
        ip_config->ip_addr = IPADDR_ANY;
    }
    if (nvs_get_u32(handle, "gw", &ip_config->gateway) != ESP_OK) {
        ip_config->gateway = IPADDR_ANY;
    }
    if (nvs_get_u32(handle, "netmask", &ip_config->netmask) != ESP_OK) {
        ip_config->netmask = IPADDR_ANY;
    }
    nvs_close(handle);
    return true;
}


bool at_nvm_station_ip_config_set (at_nvm_ip_config_struct *ip_config)
{
    nvs_handle handle;
    if (ip_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_sta_ip_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "ip", ip_config->ip_addr) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "gw", ip_config->gateway) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "netmask", ip_config->netmask) != ESP_OK) {
        return false;
    }
    nvs_close(handle);

    return true;
}

bool at_nvm_station_ip_config_get (at_nvm_ip_config_struct *ip_config)
{
    nvs_handle handle;
    if (ip_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_sta_ip_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }

    if (nvs_get_u32(handle, "ip", &ip_config->ip_addr) != ESP_OK) {
        ip_config->ip_addr = IPADDR_ANY;
    }
    if (nvs_get_u32(handle, "gw", &ip_config->gateway) != ESP_OK) {
        ip_config->gateway = IPADDR_ANY;
    }
    if (nvs_get_u32(handle, "netmask", &ip_config->netmask) != ESP_OK) {
        ip_config->netmask = IPADDR_ANY;
    }
    nvs_close(handle);
    return true;
}

bool at_nvm_softap_mac_config_set (at_nvm_mac_config_struct *mac_config)
{
    nvs_handle handle;
    if (mac_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_ap_mac_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_blob(handle, "mac", mac_config->mac, sizeof(mac_config->mac)) != ESP_OK) {
        return false;
    }

    nvs_close(handle);

    return true;
}

bool at_nvm_softap_mac_config_get (at_nvm_mac_config_struct *mac_config)
{
    nvs_handle handle;
    size_t mac_len = 0;
    if (mac_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_ap_mac_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }
    mac_len = sizeof(mac_config->mac);
    if (nvs_get_blob(handle, "mac", mac_config->mac, &mac_len) != ESP_OK) {
        return false;
    }
    nvs_close(handle);
    return true;
}


bool at_nvm_station_mac_config_set (at_nvm_mac_config_struct *mac_config)
{
    nvs_handle handle;
    if (mac_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_sta_mac_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_blob(handle, "mac", mac_config->mac, sizeof(mac_config->mac)) != ESP_OK) {
        return false;
    }

    nvs_close(handle);

    return true;
}

bool at_nvm_station_mac_config_get (at_nvm_mac_config_struct *mac_config)
{
    nvs_handle handle;
    size_t mac_len = 0;
    if (mac_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_sta_mac_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }
    mac_len = sizeof(mac_config->mac);
    if (nvs_get_blob(handle, "mac", mac_config->mac, &mac_len) != ESP_OK) {
        return false;
    }
    nvs_close(handle);
    return true;
}

bool at_nvm_dhcp_config_set (at_nvm_dhcp_config_struct *dhcp_config)
{
    nvs_handle handle;
    if (dhcp_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_dhcp_config_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "mode", dhcp_config->dhcp_mode) != ESP_OK) {
        return false;
    }

    nvs_close(handle);
    return true;
}

bool at_nvm_dhcp_config_get (at_nvm_dhcp_config_struct *dhcp_config)
{
    nvs_handle handle;
    if (dhcp_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_dhcp_config_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }

    if (nvs_get_u32(handle, "mode", &dhcp_config->dhcp_mode) != ESP_OK) {
        dhcp_config->dhcp_mode = 0;
    }

    nvs_close(handle);
    return true;
}


bool at_nvm_dhcps_config_set (at_nvm_dhcps_config_struct *dhcps_config)
{
    nvs_handle handle;
    if (dhcps_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_dhcps_config_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "start_ip", dhcps_config->start_ip) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "end_ip", dhcps_config->end_ip) != ESP_OK) {
        return false;
    }
    if (nvs_set_u32(handle, "lease_time", dhcps_config->lease_time) != ESP_OK) {
        return false;
    }
    if (nvs_set_u8(handle, "enable", dhcps_config->enable) != ESP_OK) {
        return false;
    }
    nvs_close(handle);
    return true;
}

bool at_nvm_dhcps_config_get (at_nvm_dhcps_config_struct *dhcps_config)
{
    nvs_handle handle;
    if (dhcps_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_dhcps_config_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_get_u32(handle, "start_ip", &dhcps_config->start_ip) != ESP_OK) {
        dhcps_config->start_ip = IPADDR_ANY;
    }
    if (nvs_get_u32(handle, "end_ip", &dhcps_config->end_ip) != ESP_OK) {
        dhcps_config->end_ip = IPADDR_ANY;
    }
    if (nvs_get_u32(handle, "lease_time", &dhcps_config->lease_time) != ESP_OK) {
        dhcps_config->lease_time = 0;
    }
    if (nvs_get_u8(handle, "enable", &dhcps_config->enable) != ESP_OK) {
        dhcps_config->enable = 0;
    }

    nvs_close(handle);

    return true;
}

bool at_nvm_trans_config_set (at_nvm_trans_config_struct *trans_config)
{
    nvs_handle handle;
    if (trans_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_trans_config_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_set_u8(handle, "enable", trans_config->enable) != ESP_OK) {
        return false;
    }
    if (nvs_set_u8(handle, "type", trans_config->link_type) != ESP_OK) {
        return false;
    }
    if (nvs_set_str(handle, "r_ip", (const char *)trans_config->remote_ip) != ESP_OK) {
        return false;
    }
    if (nvs_set_i32(handle, "r_port", trans_config->remote_port) != ESP_OK) {
        return false;
    }
    if (nvs_set_i32(handle, "l_port", trans_config->local_port) != ESP_OK) {
        return false;
    }
    if (nvs_set_u8(handle, "change_mode", trans_config->change_mode) != ESP_OK) {
        return false;
    }
    if (nvs_set_u16(handle, "keep_alive", trans_config->keep_alive) != ESP_OK) {
        return false;
    }

    nvs_close(handle);
    return true;
}

bool at_nvm_trans_config_get (at_nvm_trans_config_struct *trans_config)
{
    nvs_handle handle;
    size_t len = 0;
    if (trans_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_trans_config_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    if (nvs_get_u8(handle, "enable", &trans_config->enable) != ESP_OK) {
        trans_config->enable = 0;
    }
    if (nvs_get_u8(handle, "type", &trans_config->link_type) != ESP_OK) {
        trans_config->link_type = 0;
    }
    len = sizeof(trans_config->remote_ip);
    if (nvs_get_str(handle, "r_ip", (char *)trans_config->remote_ip, &len) != ESP_OK) {
        at_memset(trans_config->remote_ip, 0x0, sizeof(trans_config->remote_ip));
    }
    if (nvs_get_i32(handle, "r_port", &trans_config->remote_port) != ESP_OK) {
        trans_config->remote_port = 0;
    }
    if (nvs_get_i32(handle, "l_port", &trans_config->local_port) != ESP_OK) {
        trans_config->local_port = 0;
    }
    if (nvs_get_u8(handle, "change_mode", &trans_config->change_mode) != ESP_OK) {
        trans_config->change_mode = 0;
    }
    if (nvs_get_u16(handle, "keep_alive", &trans_config->keep_alive) != ESP_OK) {
        trans_config->keep_alive = 0;
    }

    return true;
}

bool at_nvm_dns_server_set (at_nvm_dns_server_struct *dns_server_config)
{
    nvs_handle handle;
    uint8_t loop = 0;
    uint8_t ser_name[16];
    if (dns_server_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_dns_server_name, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }

    for (loop = 0;loop < DNS_MAX_SERVERS;loop++) {
        at_snprintf(ser_name,sizeof(ser_name),"dns_ser%d",loop);
        if (nvs_set_u32(handle,(char*) ser_name, dns_server_config->dns_server[loop]) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
    }

    nvs_close(handle);
    return true;
}

bool at_nvm_dns_server_get (at_nvm_dns_server_struct *dns_server_config)
{
    nvs_handle handle;
    uint8_t loop = 0;
    uint8_t ser_name[16];
    if (dns_server_config == NULL) {
        return false;
    }

    if (nvs_open(at_nvm_dns_server_name, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }

    for (loop = 0;loop < DNS_MAX_SERVERS;loop++) {
        at_snprintf(ser_name,sizeof(ser_name),"dns_ser%d",loop);
        if (nvs_get_u32(handle, (char*)ser_name, &dns_server_config->dns_server[loop]) != ESP_OK) {
            nvs_close(handle);
            return false;
        }
    }

    nvs_close(handle);
    return true;
}


void at_nvm_restore(void)
{
    // TODO:
    const esp_partition_t *p = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
    if (p == NULL) {
        return;
    }
    esp_wifi_stop();
    esp_partition_erase_range(p, 0, p->size);
}

