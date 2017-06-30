/*
 * File : at_nv_data.h
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

#ifndef __AT_NVM_H__
#define __AT_NVM_H__
#include <esp_types.h>

typedef struct {
    uint32_t ip_addr;
    uint32_t gateway;
    uint32_t netmask;
} at_nvm_ip_config_struct;

typedef struct {
    uint8_t mac[6];
    uint8_t pad[2];
} at_nvm_mac_config_struct;


typedef struct {
    uint32_t  dhcp_mode;              // softap 0x01;sta 0x02
} at_nvm_dhcp_config_struct;


typedef struct  {
    uint8_t enable;             /* whether enter transmit transparently mode */
    uint8_t link_id;
    uint8_t link_type;
    uint8_t  change_mode;
    uint8_t remote_ip[64];
    int32_t remote_port;
    int32_t local_port;
    uint16_t keep_alive;
    uint8_t  ssl_enable;
    uint8_t  reserved;
} at_nvm_trans_config_struct;


typedef struct {
    uint8_t  enable;
    uint32_t lease_time;             /* minute */
    uint32_t start_ip;
    uint32_t end_ip;
} at_nvm_dhcps_config_struct;

typedef struct  {
    uint32_t dns_server[DNS_MAX_SERVERS];             /* whether enter transmit transparently mode */
} at_nvm_dns_server_struct;

typedef union {
    at_nvm_ip_config_struct    ip_config;
    at_nvm_mac_config_struct   mac_conig;
    at_nvm_dhcp_config_struct  dhcp_config;
    at_nvm_trans_config_struct trans_config;
    at_nvm_dhcps_config_struct  dhcps_config;
    at_nvm_dns_server_struct    dns_server_config;
} at_nvm_struct;


bool at_nvm_softap_ip_config_set (at_nvm_ip_config_struct *ip_config);

bool at_nvm_softap_ip_config_get (at_nvm_ip_config_struct *ip_config);

bool at_nvm_station_ip_config_set (at_nvm_ip_config_struct *ip_config);

bool at_nvm_station_ip_config_get (at_nvm_ip_config_struct *ip_config);

bool at_nvm_softap_mac_config_set (at_nvm_mac_config_struct *mac_config);

bool at_nvm_softap_mac_config_get (at_nvm_mac_config_struct *mac_config);

bool at_nvm_station_mac_config_set (at_nvm_mac_config_struct *mac_config);

bool at_nvm_station_mac_config_get (at_nvm_mac_config_struct *mac_config);

bool at_nvm_dhcp_config_set (at_nvm_dhcp_config_struct *dhcp_config);

bool at_nvm_dhcp_config_get (at_nvm_dhcp_config_struct *dhcp_config);

bool at_nvm_dhcps_config_set (at_nvm_dhcps_config_struct *dhcps_config);

bool at_nvm_dhcps_config_get (at_nvm_dhcps_config_struct *dhcps_config);

bool at_nvm_trans_config_set (at_nvm_trans_config_struct *trans_config);

bool at_nvm_trans_config_get (at_nvm_trans_config_struct *trans_config);

bool at_nvm_dns_server_set (at_nvm_dns_server_struct *dns_server_config);

bool at_nvm_dns_server_get (at_nvm_dns_server_struct *dns_server_config);

void at_nvm_restore(void);

#endif

