#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import sys, os, argparse

def ESP_LOGE(x):
    print('\033[31m{}\033[0m'.format(x))

wlanif_c_file = 'wlanif.c'

# Find the debug snippet entry
at_snippet_pos_pattern = 'In this function, the hardware should be initialized.'
at_snippet_pos_offset = 8
at_net_debug_snippet = """
#include "esp_log.h"
#include "sdkconfig.h"

/* IPv4+ protocol check */
typedef enum {
    IP4_PROTO_ICMP = 1,
    IP4_PROTO_IGMP = 2,
    IP4_PROTO_TCP  = 6, // TCP or protocol based on TCP (SSL/HTTP/MQTT/WebSocket/etc)
    IP4_PROTO_UDP = 17, // UDP or protocol based on UDP (DHCP/DNS/SNTP/mDNS/etc)
} ip4_proto_t;

static bool at_pkt_is_monitored(ip4_proto_t type, uint16_t src_port, uint16_t dst_port, bool tx)
{
    if (type == IP4_PROTO_TCP) {
#if CONFIG_AT_NET_TCP_DEBUG
        uint16_t tcp_ports[] = {CONFIG_AT_NET_TCP_DEBUG_PORT_LIST};
        if (tcp_ports[0] == 0) {
            return true;
        }
        for (int i = 0; i < sizeof(tcp_ports) / sizeof(tcp_ports[0]); ++i) {
            if (src_port == tcp_ports[i] || dst_port == tcp_ports[i]) {
                return true;
            }
        }
#endif
    } else if (type == IP4_PROTO_UDP) {
#if CONFIG_AT_NET_UDP_DEBUG
        uint16_t udp_tx_ports[] = {CONFIG_AT_NET_UDP_DEBUG_TX_PORT_LIST};
        uint16_t udp_rx_ports[] = {CONFIG_AT_NET_UDP_DEBUG_RX_PORT_LIST};
        if (tx) {
            if (udp_tx_ports[0] == 0) {
                return true;
            }
            for (int i = 0; i < sizeof(udp_tx_ports) / sizeof(udp_tx_ports[0]); ++i) {
                if (src_port == udp_tx_ports[i] || dst_port == udp_tx_ports[i]) {
                    return true;
                }
            }
        } else {
            if (udp_rx_ports[0] == 0) {
                return true;
            }
            for (int i = 0; i < sizeof(udp_rx_ports) / sizeof(udp_rx_ports[0]); ++i) {
                if (src_port == udp_rx_ports[i] || dst_port == udp_rx_ports[i]) {
                    return true;
                }
            }
        }
#endif
    }

    return false;
}

/* only support tcp, udp, icmp now. attention: don't support ip fragment and ipv6 now. */
void at_print_pkt_info(void *buf, bool tx)
{
    if (!buf) {
        return;
    }
    // length check
    struct pbuf *p = (struct pbuf *)buf;
    if (p->tot_len <= 34) { /* not a valid IPv4 header */
        return;
    }

    // network protocol check
    enum {
        NET_PROTO_IPV4 = 0x0800,
        NET_PROTO_IPV6 = 0x86DD,
        NET_PROTO_ARP  = 0x0806,
    };
    uint16_t net_proto;
    /* p->payload consists of sequential: dst mac(6B) + src mac(6B) + proto type (2B) + ip4 header (20B+) + etc */
    net_proto = *((uint8_t *)p->payload + 12);
    net_proto <<= 8;
    net_proto += *((uint8_t *)p->payload + 13);
    if (net_proto != NET_PROTO_IPV4) { /* only support IPv4+ protocol now */
        return;
    }

    uint8_t *ip4 = (uint8_t *)p->payload + 14;
    uint8_t ip_proto = *(ip4 + 9);
    uint16_t ip_tlen = *(ip4 + 2);
    ip_tlen <<= 8;
    ip_tlen += *(ip4 + 3);
    uint16_t ip_hlen = (*ip4 & 0x0F) * 4;
    uint16_t ip_dlen = ip_tlen - ip_hlen;

    switch (ip_proto) {

#if CONFIG_AT_NET_TCP_DEBUG
    case IP4_PROTO_TCP: {
        if (ip_dlen < 20) {
            return;
        }
        uint8_t *tcp = ip4 + ip_hlen;
        uint16_t src_port = *tcp; src_port <<= 8; src_port += *(tcp + 1);
        uint16_t dst_port = *(tcp + 2); dst_port <<= 8; dst_port += *(tcp + 3);
        uint32_t seq = *(tcp + 4); seq <<= 8; seq += *(tcp + 5); seq <<= 8; seq += *(tcp + 6); seq <<= 8; seq += *(tcp + 7);
        uint32_t ack = *(tcp + 8); ack <<= 8; ack += *(tcp + 9); ack <<= 8; ack += *(tcp + 10); ack <<= 8; ack += *(tcp + 11);
        uint8_t tcp_hlen = ((*(tcp + 12) & 0xF0) >> 4) * 4;
        uint8_t flags = *(tcp + 13);
        uint16_t tcp_dlen = ip_dlen - tcp_hlen;

        if (!at_pkt_is_monitored(ip_proto, src_port, dst_port, tx)) {
            return;
        }

        if (tx) {
            ESP_LOGI("@@tcp-tx", "IPL:%u, S:%u, A:%u, SP:%u, DP:%u, F:0x%x, TDL:%u", ip_tlen, seq, ack, src_port, dst_port, flags, tcp_dlen);
        } else {
            ESP_LOGI("@@tcp-rx", "IPL:%u, S:%u, A:%u, SP:%u, DP:%u, F:0x%x, TDL:%u", ip_tlen, seq, ack, src_port, dst_port, flags, tcp_dlen);
        }
    }
        break;
#endif

#if CONFIG_AT_NET_UDP_DEBUG
    case IP4_PROTO_UDP: {
        if (ip_dlen < 8) {
            return;
        }
        uint8_t *udp = ip4 + ip_hlen;
        uint16_t src_port = *udp; src_port <<= 8; src_port += *(udp + 1);
        uint16_t dst_port = *(udp + 2); dst_port <<= 8; dst_port += *(udp + 3);
        uint16_t udp_dlen = ip_dlen - 8; /* fixed length: 8 bytes for udp header */

        if (!at_pkt_is_monitored(ip_proto, src_port, dst_port, tx)) {
            return;
        }

        if (tx) {
            ESP_LOGI("udp-tx", "IPL:%u, SP:%u, DP:%u, UDL:%u", ip_tlen, src_port, dst_port, udp_dlen);
        } else {
            ESP_LOGI("udp-rx", "IPL:%u, SP:%u, DP:%u, UDL:%u", ip_tlen, src_port, dst_port, udp_dlen);
        }
    }
        break;
#endif

#if CONFIG_AT_NET_ICMP_DEBUG
    case IP4_PROTO_ICMP: {
        if (ip_dlen < 8) {
            return;
        }
        uint8_t *icmp = ip4 + ip_hlen;
        uint8_t type = *icmp;
        uint8_t code = *(icmp + 1);
        bool valid_type = (type == 0 || type == 8); /* icmp echo-request or echo-reply */
        bool valid_code = (code == 0);
        uint16_t icmp_dlen = ip_dlen - 8; /* fixed length: 8 bytes for icmp echo-request or echo-reply */
        if (!valid_type || !valid_code) {
            return;
        }
        uint16_t id = *(icmp + 4); id <<= 8; id += *(icmp + 5);
        uint16_t seq = *(icmp + 6); seq <<= 8; seq += *(icmp + 7);
        if (tx) {
            ESP_LOGI("icmp-tx", "%s, IPL:%u, ID:0x%x, S:%u PDL:%u", (type == 8) ? "Echo" : "Echo Reply", ip_tlen, id, seq, icmp_dlen);
        } else {
            ESP_LOGI("icmp-rx", "%s, IPL:%u, ID:0x%x, S:%u PDL:%u", (type == 8) ? "Echo" : "Echo Reply", ip_tlen, id, seq, icmp_dlen);
        }
    }
        break;
#endif

    default:
        return;
    }
}
"""

# Find the entry of outgoing packets
at_net_tx_pos_pattern_idf = 'if(q->next == NULL) {'
at_net_tx_pos_pattern_rtos = 'if (!netif_is_up(netif)) {'
at_net_tx_caller = """
    at_print_pkt_info(p, true);
"""

# Find the entry of netif tx returns
at_net_tx_ret_pos_pattern_v54_before = 'if (ret == ESP_OK) {'   # < v5.4
at_net_tx_ret_pos_pattern_v54_after = 'return ret;'             # >= v5.4
at_net_tx_ret_caller = """
    if (ret != ESP_OK) {
        ESP_LOGE("@@if-tx", "netif tx error, tot_len:%d len:%d ret: %d", p->tot_len, p->len, ret);
    }
"""

# Find the entry of incoming packets
at_net_rx_pos_pattern = '/* full packet send to tcpip_thread to process */'
at_net_rx_caller = """
    at_print_pkt_info(p, false);
"""

def at_findfile(start, name):
    for relpath, dirs, files in os.walk(start):
        if name in files:
            full_path = os.path.join(start, relpath, name)
            return os.path.normpath(os.path.abspath(full_path))
    return None

def at_update_args_if_sdkconfig(args):
    args.no_tcp = True
    args.no_udp = True
    args.no_icmp = True

    with args.sdkconfig as f:
        data = f.read()

    # remove the duplicate lines
    dup_data = data.split('\n')
    uni_data = []
    for item in dup_data:
        if item not in uni_data or item.startswith('#'):
            uni_data.append(item)
    data = '\n'.join(uni_data)
    data = '[saved-args]' + data

    import configparser
    config = configparser.ConfigParser()
    config.optionxform = str
    config.read_string(data)

    for section in config.sections():
        for key, value in config.items(section):
            if key == 'CONFIG_AT_NET_TCP_DEBUG':
                args.no_tcp = False
            elif key == 'CONFIG_AT_NET_UDP_DEBUG':
                args.no_udp = False
            elif key == 'CONFIG_AT_NET_ICMP_DEBUG':
                args.no_icmp = False
            elif key == 'CONFIG_AT_NET_TCP_DEBUG_PORT_LIST':
                args.tcp_port = value.replace('"', '')
            elif key == 'CONFIG_AT_NET_UDP_DEBUG_TX_PORT_LIST':
                args.udp_tx_port = value.replace('"', '')
            elif key == 'CONFIG_AT_NET_UDP_DEBUG_RX_PORT_LIST':
                args.udp_rx_port = value.replace('"', '')
    return args

def at_restore_net_debug_if_config():
    import subprocess
    wlanif_path = at_findfile(os.getcwd(), wlanif_c_file)
    if not wlanif_path:
        raise Exception('No {} found in {}'.format(wlanif_c_file, os.getcwd()))

    has_patch = False
    with open(wlanif_path, 'rt') as f:
        data = f.read()
        if data.find('at_print_pkt_info') >= 0:
            wlanif_dir = os.path.dirname(wlanif_path)
            wlanif_name = os.path.basename(wlanif_path)
            has_patch = True
    if has_patch:
        cmd = 'cd {} && git restore {} && cd -'.format(wlanif_dir, wlanif_name)
        if subprocess.call(cmd, shell = True, stdout=subprocess.DEVNULL):
            raise Exception('restore failed: {}.'.format(cmd))

    return wlanif_path

def at_update_net_debug_snippet(snippet, args):
    if args.no_tcp:
        snippet = snippet.replace('CONFIG_AT_NET_TCP_DEBUG', '0')
    else:
        snippet = snippet.replace('CONFIG_AT_NET_TCP_DEBUG_PORT_LIST', args.tcp_port)
        snippet = snippet.replace('CONFIG_AT_NET_TCP_DEBUG', '1')

    if args.no_udp:
        snippet = snippet.replace('CONFIG_AT_NET_UDP_DEBUG', '0')
    else:
        snippet = snippet.replace('CONFIG_AT_NET_UDP_DEBUG_TX_PORT_LIST', args.udp_tx_port)
        snippet = snippet.replace('CONFIG_AT_NET_UDP_DEBUG_RX_PORT_LIST', args.udp_rx_port)
        snippet = snippet.replace('CONFIG_AT_NET_UDP_DEBUG', '1')

    if args.no_icmp:
        snippet = snippet.replace('CONFIG_AT_NET_ICMP_DEBUG', '0')
    else:
        snippet = snippet.replace('CONFIG_AT_NET_ICMP_DEBUG', '1')
    return snippet

def at_patch_net_debug_snippet(args):
    wlanif_path = at_restore_net_debug_if_config()

    with open(wlanif_path, 'rt+') as f:
        data = f.read()

        # add definition of at_print_pkt_info()
        pos = data.find(at_snippet_pos_pattern) - at_snippet_pos_offset
        debug_snippet = at_update_net_debug_snippet(at_net_debug_snippet, args)
        data = data[:pos] + debug_snippet + data[pos:]

        # add tx caller of at_print_pkt_info()
        pos = data.find(at_net_tx_pos_pattern_idf)
        if pos < 0:
            pos = data.find(at_net_tx_pos_pattern_rtos)
            if pos < 0:
                raise Exception('No TX caller entry found.')
        data = data[:pos] + at_net_tx_caller + '\n    '  + data[pos:]

        # add tx caller of netif tx returns for esp-idf version < v5.4
        pos = data.find(at_net_tx_ret_pos_pattern_v54_before)
        if pos > 0:
            data = data[:pos] + at_net_tx_ret_caller + '\n    '  + data[pos:]

        # add tx caller of netif tx returns for esp-idf version >= v5.4
        pos = data.find(at_net_tx_ret_pos_pattern_v54_after)
        if pos > 0:
            data = data[:pos] + at_net_tx_ret_caller + '\n    '  + data[pos:]

        # add rx caller of at_print_pkt_info()
        pos = data.find(at_net_rx_pos_pattern)
        if pos < 0:
            raise Exception('No RX caller entry found.')
        data = data[:pos] + at_net_rx_caller + '\n    ' + data[pos:]

        f.seek(0)
        f.write(data)
    print('The network debug has been successfully enabled!')

def main():
    parser = argparse.ArgumentParser(description='Enter ESP-IDF directory and run this script. This script helps debuging TCP/UDP/ICMP and above protocol. (Attention: this script does not support IP fragment and IPv6 till now.)')
    parser.add_argument('--no-tcp', action='store_true', help='Default: False. It will print the outgoing (TCP TX) and incoming (TCP RX) packets information by default, which includes ip total len, tcp data len, tcp seq, tcp ack, src port, dest port, and tcp flags. TCP flags is the accumulation of CWR:128, ECN:64, Urgent:32, Ack:16, Push:8, Reset:4, SYN:2, FIN:1.')
    parser.add_argument('--tcp-port', type=str, default='0', help='Default: "0". Specify the list of TCP port numbers to monitor. 0 means all ports, and the port numbers should be separated by commas if monitor multiple ports.')
    parser.add_argument('--no-udp', action='store_true', help='Default: False. It will print the outgoing (UDP TX) and incoming (UDP RX) packets information by default, which includes ip total len, src port, dest port, and udp data len.')
    parser.add_argument('--udp-tx-port', type=str, default='0', help='Default: "0". Specify the list of outgoing UDP (UDP TX) port numbers to monitor. 0 means all ports, and the port numbers should be separated by commas if monitor multiple ports.')
    parser.add_argument('--udp-rx-port', type=str, default='53, 67, 68, 123', help='Default: "53, 67, 68, 123". Specify the list of incoming UDP (UDP RX) port numbers to monitor.The port numbers should be separated by commas if monitor multiple ports. You should not set the port number to 0 to prevent UDP RX flooding.')
    parser.add_argument('--no-icmp', action='store_true', help='Default: False. It will print the outgoing (ICMP TX) and incoming (ICMP RX) packets information by default, which includes ip total len, icmp type, id, seq, and icmp data len.')
    parser.add_argument('--sdkconfig', type=argparse.FileType('r'), default=None, help='Mutually exclusive parameter. It should be set alone. The path to the sdkconfig file.')
    parser.add_argument('--restore', action='store_true', help='Mutually exclusive parameter. It should be set alone. It will restore the wlanif.c file to the original state.')
    args = parser.parse_args()

    # check the arguments
    argc = len(sys.argv)
    if args.restore and argc != 2:
        raise Exception('The --restore option should be used alone.')
    if args.sdkconfig and argc != 3:
        raise Exception('The --sdkconfig option should be used alone.')

    if args.restore:
        at_restore_net_debug_if_config()
        sys.exit(0)

    # update the arguments
    if args.sdkconfig:
        at_update_args_if_sdkconfig(args)

    at_patch_net_debug_snippet(args)

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE('{}'.format(e))
        sys.exit(-1)
