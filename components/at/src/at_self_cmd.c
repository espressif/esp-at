/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_at_core.h"
#include "esp_at.h"
#include "esp_at_interface.h"

#ifdef CONFIG_AT_SELF_COMMAND_SUPPORT
#define AT_CMD_RESP_BIT                 BIT(0)

typedef struct {
    EventGroupHandle_t status_bits;     /*!< status bits for self command event */
    char *cmd;         /*!< command string from self command */
    char *resp;        /*!< expected response from self command */
    bool mode;         /*!< self command mode */
} at_self_cmd_t;

at_self_cmd_t *s_self_cmd = NULL;
static const char *TAG = "at-self-cmd";

bool at_self_cmd_get_mode(void)
{
    if (!s_self_cmd) {
        return false;
    }
    return s_self_cmd->mode;
}

static void at_self_cmd_set_mode(bool mode)
{
    s_self_cmd->mode = mode;
}

int32_t at_self_cmd_read_data(uint8_t *buffer, int32_t buffer_len)
{
    int32_t len = strlen(s_self_cmd->cmd) < buffer_len ? strlen(s_self_cmd->cmd) : buffer_len;
    memcpy(buffer, s_self_cmd->cmd, len);
    return len;
}

int32_t at_self_cmd_write_data(uint8_t *data, int32_t len)
{
    at_write_data_fn_t write_fn = at_interface_get_write_fn();
    int32_t ret = write_fn(data, len);

    // check the response
    if (strstr((char *)data, s_self_cmd->resp)) {
        xEventGroupSetBits(s_self_cmd->status_bits, AT_CMD_RESP_BIT);
    }

    return ret;
}

int32_t at_self_cmd_get_data_len(void)
{
    return strlen(s_self_cmd->cmd);
}

static void at_self_cmd_cleanup(void)
{
    if (s_self_cmd) {
        if (s_self_cmd->cmd) {
            free(s_self_cmd->cmd);
            s_self_cmd->cmd = NULL;
        }
        if (s_self_cmd->resp) {
            free(s_self_cmd->resp);
            s_self_cmd->resp = NULL;
        }
        if (s_self_cmd->status_bits) {
            vEventGroupDelete(s_self_cmd->status_bits);
            s_self_cmd->status_bits = NULL;
        }
        free(s_self_cmd);
        s_self_cmd = NULL;
    }
}

esp_err_t at_exe_cmd(const char *cmd, const char *expected_response, uint32_t timeout_ms)
{
    esp_err_t ret = ESP_OK;

    // init
    s_self_cmd = (at_self_cmd_t *)malloc(sizeof(at_self_cmd_t));
    if (!s_self_cmd) {
        return ESP_ERR_NO_MEM;
    }

    s_self_cmd->status_bits = xEventGroupCreate();
    s_self_cmd->cmd = strdup(cmd);
    s_self_cmd->resp = strdup(expected_response);
    if (!s_self_cmd->status_bits || !s_self_cmd->cmd || !s_self_cmd->resp) {
        at_self_cmd_cleanup();
        return ESP_ERR_NO_MEM;
    }
    at_self_cmd_set_mode(true);

    // command notify
    esp_at_port_recv_data_notify(strlen(cmd), portMAX_DELAY);

    // wait for response
    EventBits_t uxBits = xEventGroupWaitBits(s_self_cmd->status_bits, AT_CMD_RESP_BIT, pdFALSE, pdFALSE, timeout_ms / portTICK_PERIOD_MS);
    if (!(uxBits & AT_CMD_RESP_BIT)) {
        ESP_LOGE(TAG, "<%.*s> cannot get expected response <%s> within %ums", strlen(cmd) - 2, cmd, expected_response, timeout_ms);
        ret = ESP_ERR_TIMEOUT;
    }

    // deinit
    at_self_cmd_set_mode(false);
    at_self_cmd_cleanup();
    return ret;
}
#endif
