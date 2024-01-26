/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include "esp_err.h"

/**
 * @brief Main AT initialization function, called in app_main()
*/
void esp_at_init(void);

/**
 * @brief The netif initialization function, called in esp_at_init()
*/
esp_err_t esp_at_netif_init(void);

/**
 * @brief This function is used to register all the at command sets.
*/
void esp_at_cmd_set_register(void);
