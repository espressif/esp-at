/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_at.h"

#ifdef CONFIG_AT_SIGNALING_COMMAND_SUPPORT

static uint8_t at_setupCmdFactPlcp(uint8_t para_num)
{
    int32_t enable = 0;
    int32_t tx_with_long = 0;
    int32_t cnt = 0;

    if (esp_at_get_para_as_digit (cnt++, &enable) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((enable != 0) && (enable != 1)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit (cnt++, &tx_with_long) != ESP_AT_PARA_PARSE_RESULT_OK) {
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
    return esp_at_custom_cmd_array_regist (at_fact_cmd, sizeof(at_fact_cmd)/sizeof(at_fact_cmd[0]));
}
#endif