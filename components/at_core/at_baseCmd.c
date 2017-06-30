/*
 * File : at_baseCmd.c
 * This file is part of Espressif's AT+ command set program.
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
#include "esp_types.h"
#include "string.h"
#include "esp_system.h"
#include "at.h"
#include "at_nvm.h"
/** @defgroup AT_BASECMD_Functions
 * @{
 */

static uint8_t *at_custom_info = NULL;

static bool at_echo_flag = TRUE;

/**
 * @brief  Execution commad of AT.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdNull(uint8_t *cmd_name)
{
    return ESP_AT_RESULT_CODE_OK;
}

void at_set_echo_flag(bool enable)
{
    at_echo_flag = (enable == FALSE) ? FALSE : TRUE;
}

bool at_get_echo_flag(void)
{
    return at_echo_flag;
}

/**
 * @brief  Enable or disable Echo.
 * @param  id: command id number
 * @param  pPara:
 * @retval None
 */
uint8_t at_exeCmdE0(uint8_t *cmd_name)
{
    uint8_t ret = ESP_AT_RESULT_CODE_OK;
    at_set_echo_flag(FALSE);
    return ret;
}

uint8_t at_exeCmdE1(uint8_t *cmd_name)
{
    uint8_t ret = ESP_AT_RESULT_CODE_OK;
    at_set_echo_flag(TRUE);
    return ret;
}

/**
 * @brief  Execution commad of restart.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdRst(uint8_t *cmd_name)
{
    at_handle_restart();

    // never reach here
    return ESP_AT_RESULT_CODE_OK;
}

void at_set_custom_version(const uint8_t *info)
{
    if (info == NULL) {
        return;
    }
    if (at_custom_info != NULL) {
        at_free(at_custom_info);
    }
    at_custom_info = (uint8_t *)at_malloc(at_strlen(info) + 1);
    at_strcpy(at_custom_info, info);
}
/**
 * @brief  Execution commad of version.
 * @param  id: commad id number
 * @retval None
 */
uint8_t at_exeCmdGmr(uint8_t *cmd_name)
{
    uint8_t temp[64];
    uint8_t ret = ESP_AT_RESULT_CODE_OK;

    if (AT_VERSION_test) {
        at_sprintf(temp, "AT version:%d.%d.b%d.%d(%s %s)\r\n",
                   AT_VERSION_main, AT_VERSION_sub, AT_VERSION_test, AT_VERSION_cust, __DATE__, __TIME__);
    } else {
        at_sprintf(temp, "AT version:%d.%d.%d.%d(%s %s)\r\n",
                   AT_VERSION_main, AT_VERSION_sub, AT_VERSION_test, AT_VERSION_cust, __DATE__, __TIME__);
    }
    at_port_print(temp);
    at_sprintf(temp, "SDK version:%s\r\n", esp_get_idf_version());
    at_port_print(temp);
    if (at_custom_info != NULL) {
        at_port_print(at_custom_info);
    }
    return ret;
}

uint8_t at_execCmdRestore(uint8_t *cmd_name)
{
    uint8_t ret = ESP_AT_RESULT_CODE_OK;

    at_nvm_restore();
    // esp_wifi_restore();
    at_handle_restart();
    return ret;
}

uint8_t at_setupCmdGslp(uint8_t para_num)
{
    int n_ms = 0;
    uint8_t cnt = 0;
    uint8_t ret = ESP_AT_RESULT_CODE_OK;

    if (para_num != 1) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1, para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (esp_at_get_para_as_digit(cnt++, &n_ms) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    esp_at_response_result (ret);

    esp_deep_sleep(n_ms * 1000);
    return ret;
}

#ifdef AT_SLEEP_SUPPORT
uint8_t at_setupCmdSleep(uint8_t para_num)
{
    int32_t mode = 0;

    if (para_num != 1) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_NUM(1, para_num));
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if (esp_at_get_para_as_digit(0, &mode) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    if ((mode >= WIFI_PS_NONE) && (mode <= WIFI_PS_MODEM)) {
        if (ESP_OK != esp_wifi_set_ps(mode)) {
            return ESP_AT_RESULT_CODE_ERROR;
        }
    } else {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmdSleep(uint8_t *cmd_name)
{
    wifi_ps_type_t mode = 0;
    uint8_t temp[32];

    if (ESP_OK != esp_wifi_get_ps(&mode)) {
        return ESP_AT_RESULT_CODE_FAIL;
    }

    at_sprintf(temp, "%s:%d\r\n", cmd_name, mode);
    at_port_print(temp);

    return ESP_AT_RESULT_CODE_OK;
}
#endif
/**
 * @}
 */

