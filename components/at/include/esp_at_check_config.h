/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include "sdkconfig.h"

#ifdef CONFIG_LWIP_ENABLE
#include <sys/socket.h>
#include "lwipopts.h"
#endif

#ifdef CONFIG_LWIP_ENABLE
#if (CONFIG_LWIP_MAX_SOCKETS + 6 > FD_SETSIZE)
// 6 fds for stdin, stdout, stderr, /dev/uart/, /dev/secondary, /dev/null
#error "CONFIG_LWIP_MAX_SOCKETS + 6 > FD_SETSIZE! Please reduce the value of CONFIG_LWIP_MAX_SOCKETS in (Top) > Component config > LWIP > Max number of open sockets."
#endif

#ifdef CONFIG_AT_SOCKET_MAX_CONN_NUM
#define AT_LINK_NUM     CONFIG_AT_SOCKET_MAX_CONN_NUM
#else
#define AT_LINK_NUM     0
#endif

#ifdef CONFIG_AT_HTTP_COMMAND_SUPPORT
#define AT_HTTP_NUM     1
#else
#define AT_HTTP_NUM     0
#endif

#ifdef CONFIG_AT_MQTT_COMMAND_SUPPORT
#define AT_MQTT_NUM     1
#else
#define AT_MQTT_NUM     0
#endif

#ifdef CONFIG_AT_WS_COMMAND_SUPPORT
#define AT_WS_NUM       3
#else
#define AT_WS_NUM       0
#endif

#ifdef CONFIG_AT_WEB_SERVER_SUPPORT
#define AT_WEB_NUM       2
#else
#define AT_WEB_NUM       0
#endif

#if (CONFIG_LWIP_MAX_SOCKETS < (AT_LINK_NUM + AT_HTTP_NUM + AT_MQTT_NUM + AT_WS_NUM + AT_WEB_NUM + 4))
// 4 fds for internal communication
#error "CONFIG_LWIP_MAX_SOCKETS < (AT_LINK_NUM + AT_HTTP_NUM + AT_MQTT_NUM + AT_WS_NUM + AT_WEB_NUM + 4)! Please enlarge the value of CONFIG_LWIP_MAX_SOCKETS in (Top) > Component config > LWIP > Max number of open sockets."
#endif

#endif
