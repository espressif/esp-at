/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_at.h"
#include "esp_at_core.h"

// static variables
static esp_at_device_ops_struct s_interface_ops;
static esp_at_custom_ops_struct s_interface_hooks;
static const char *TAG = "at-intf";

static int32_t at_port_read_data(uint8_t *buffer, int32_t len)
{
    if (!s_interface_ops.read_data) {
        return -1;
    }
    return s_interface_ops.read_data(buffer, len);
}

static int32_t at_port_write_data(uint8_t *data, int32_t len)
{
    if (!s_interface_ops.write_data) {
        return -1;
    }
    return s_interface_ops.write_data(data, len);
}

static int32_t at_port_get_data_len(void)
{
    if (!s_interface_ops.get_data_length) {
        return -1;
    }
    return s_interface_ops.get_data_length();
}

static bool at_port_wait_tx_done(int32_t ms)
{
    if (!s_interface_ops.wait_write_complete) {
        return true;
    }

    return s_interface_ops.wait_write_complete(ms);
}

void at_interface_ops_init(esp_at_device_ops_struct *ops)
{
    s_interface_ops.read_data = ops->read_data;
    s_interface_ops.write_data = ops->write_data;
    s_interface_ops.get_data_length = ops->get_data_length;
    s_interface_ops.wait_write_complete = ops->wait_write_complete;

    esp_at_device_ops_struct at_port_ops = {
        .read_data = at_port_read_data,
        .write_data = at_port_write_data,
        .get_data_length = at_port_get_data_len,
        .wait_write_complete = at_port_wait_tx_done,
    };
    esp_at_device_ops_regist(&at_port_ops);
}

static void at_transmit_mode_switch_cb(esp_at_status_type state)
{
    // do some special things from the interface hook when transmit mode switch
    if (s_interface_hooks.status_callback) {
        s_interface_hooks.status_callback(state);
    }
}

static void at_normal_sleep_before_cb(at_sleep_mode_t mode)
{
    // do some common things before sleep
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    at_set_mcu_state_if_sleep(mode);
#endif

    // do some special things from the interface hook before sleep
    if (s_interface_hooks.pre_sleep_callback) {
        s_interface_hooks.pre_sleep_callback(mode);
    }
}

static void at_deep_sleep_before_cb(void)
{
    // do some special things from the interface hook before deep sleep
    if (s_interface_hooks.pre_deepsleep_callback) {
        s_interface_hooks.pre_deepsleep_callback();
    }
}

static void at_restart_before_cb(void)
{
    // do some common things before restart
    at_port_wait_tx_done(ESP_AT_PORT_TX_WAIT_MS_MAX);

    // do some special things from the interface hook before restart
    if (s_interface_hooks.pre_restart_callback) {
        s_interface_hooks.pre_restart_callback();
    }
}

static void at_port_tx_data_before_cb(at_write_data_fn_t fn)
{
    // do some common things before active tx data
#ifdef CONFIG_AT_USERWKMCU_COMMAND_SUPPORT
    at_wkmcu_if_config(fn);
#endif
}

void at_interface_hooks(esp_at_custom_ops_struct *if_hooks)
{
    if (if_hooks) {
        s_interface_hooks.status_callback = if_hooks->status_callback;
        s_interface_hooks.pre_sleep_callback = if_hooks->pre_sleep_callback;
        s_interface_hooks.pre_deepsleep_callback = if_hooks->pre_deepsleep_callback;
        s_interface_hooks.pre_restart_callback = if_hooks->pre_restart_callback;
        s_interface_hooks.pre_active_write_data_callback = if_hooks->pre_active_write_data_callback;
    }

    esp_at_custom_ops_struct at_hooks = {
        .status_callback = at_transmit_mode_switch_cb,
        .pre_sleep_callback = at_normal_sleep_before_cb,
        .pre_deepsleep_callback = at_deep_sleep_before_cb,
        .pre_restart_callback = at_restart_before_cb,
        .pre_active_write_data_callback = at_port_tx_data_before_cb,
    };
    esp_at_custom_ops_regist(&at_hooks);
}
