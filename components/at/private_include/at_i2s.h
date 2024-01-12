/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#if defined(CONFIG_AT_BT_COMMAND_SUPPORT)
#if defined(CONFIG_AT_BT_A2DP_COMMAND_SUPPORT)
void esp_at_i2s_init(void);
#endif
#endif
