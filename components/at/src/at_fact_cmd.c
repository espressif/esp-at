/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "sdkconfig.h"
#include "esp_at.h"

#ifdef CONFIG_AT_SIGNALING_COMMAND_SUPPORT

static uint8_t at_setupCmdFactPlcp(uint8_t para_num)
{
    int32_t enable = 0;
    int32_t tx_with_long = 0;
    int32_t cnt = 0;

    if (esp_at_get_para_as_digit(cnt++, &enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((enable != 0) && (enable != 1)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit(cnt++, &tx_with_long) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((tx_with_long != 0) && (tx_with_long != 1)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    /**
     * TODO:
     * Explicit include corresponding header file and call esp_wifi_set_11b_tx_plcp() API,
     * However the SDK has not exposed this API so far.
    */
    void esp_wifi_set_11b_tx_plcp(bool enable, bool tx_with_long);
    esp_wifi_set_11b_tx_plcp(enable, tx_with_long);

    return ESP_AT_RESULT_CODE_OK;
}

static const esp_at_cmd_struct at_fact_cmd[] = {
    {"+FACTPLCP", NULL, NULL, at_setupCmdFactPlcp, NULL},
};

bool esp_at_fact_cmd_regist(void)
{
    return esp_at_custom_cmd_array_regist(at_fact_cmd, sizeof(at_fact_cmd) / sizeof(at_fact_cmd[0]));
}

ESP_AT_CMD_SET_FIRST_INIT_FN(esp_at_fact_cmd_regist);

#endif
