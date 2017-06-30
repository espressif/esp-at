/*
 * at_commom.c
 *
 *  Created on: Feb 3, 2015
 *      Author: xcg
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "string.h"
#include "at.h"
#include "at_cmd.h"

#define AT_PARA_MAX_NUM 16
#define AT_CMD_LINE_TERMINATOR  "\r\n"


enum {
    AT_OP_INVALID = 0,
    AT_OP_TEST,
    AT_OP_QUERY,
    AT_OP_SETUP,
    AT_OP_EXEC

};

typedef struct {
    uint8_t *cmd;
    uint8_t  cmd_len;
    uint8_t  op;
    uint8_t  para_num;
    uint8_t *para[AT_PARA_MAX_NUM];
} at_information_struct;

static at_information_struct at_cmd_info;
static esp_at_cmd_struct at_default_cmd[] = {
    {NULL,  NULL, NULL, NULL, at_exeCmdNull},
    {"E0",  NULL, NULL, NULL, at_exeCmdE0},
    {"E1",  NULL, NULL, NULL, at_exeCmdE1},
    {"+RST",  NULL, NULL, NULL, at_exeCmdRst},
    {"+GMR",  NULL, NULL, NULL, at_exeCmdGmr},
    {"+GSLP", NULL, NULL, at_setupCmdGslp, NULL},
#ifdef AT_SLEEP_SUPPORT
    {"+SLEEP", NULL, at_queryCmdSleep, at_setupCmdSleep, NULL},
#endif
    {"+RESTORE", NULL, NULL, NULL, at_execCmdRestore},
//        {"+RFPOWER", 8, NULL, NULL, at_setupCmdRFPower, NULL},
//        {"+RFVDD", 6, NULL, at_queryCmdRFVdd, at_setupCmdRFVdd, at_execCmdRFVdd},
#ifdef AT_CMD_NO_OPTIMIZE
    {"+CWMODE", at_testCmdCwmode, at_queryCmdCwmode, at_setupCmdCwmodeDef, NULL},
    {"+CWJAP", NULL, at_queryCmdCwjap, at_setupCmdCwjapDef, NULL},
#endif
#ifdef AT_CMD_OPTIMIZE
    {"+CWMODE_DEF", at_testCmdCwmode, at_queryCmdCwmodeDef, at_setupCmdCwmodeDef, NULL},
    {"+CWMODE_CUR", at_testCmdCwmode, at_queryCmdCwmode, at_setupCmdCwmode, NULL},
    {"+CWJAP_DEF", NULL, at_queryCmdCwjapDef, at_setupCmdCwjapDef, NULL},
    {"+CWJAP_CUR", NULL, at_queryCmdCwjap, at_setupCmdCwjap, NULL},
#endif
#ifdef AT_SCAN_SUPPORT
    {"+CWLAP", NULL, NULL, at_setupCmdCwlap, at_exeCmdCwlap},
#endif
    {"+CWLAPOPT", NULL, NULL, at_setupCmdCwlapOpt, NULL},
    {"+CWQAP", at_testCmdCwqap, NULL, NULL, at_exeCmdCwqap},

#ifdef AT_CMD_NO_OPTIMIZE
    {"+CWSAP", NULL, at_queryCmdCwsap, at_setupCmdCwsapDef, NULL},
#endif
#ifdef AT_CMD_OPTIMIZE
    {"+CWSAP_DEF", NULL, at_queryCmdCwsapDef, at_setupCmdCwsapDef, NULL},
    {"+CWSAP_CUR", NULL, at_queryCmdCwsap, at_setupCmdCwsap, NULL},
#endif
#ifdef AT_LIST_IF_SUPPORT
    {"+CWLIF", NULL, NULL, NULL, at_exeCmdCwlif},
#endif
#ifdef AT_DHCP_SUPPORT
#ifdef AT_CMD_NO_OPTIMIZE
    {"+CWDHCP", NULL, at_queryCmdCwdhcp, at_setupCmdCwdhcpDef, NULL},
    {"+CWDHCPS",  NULL, at_queryCmdCwdhcps, at_setupCmdCwdhcpsDef, NULL},
#endif
#ifdef AT_CMD_OPTIMIZE
    {"+CWDHCP_DEF", NULL, at_queryCmdCwdhcpDef, at_setupCmdCwdhcpDef, NULL},
    {"+CWDHCP_CUR",  NULL, at_queryCmdCwdhcp, at_setupCmdCwdhcp, NULL},
    {"+CWDHCPS_DEF",  NULL, at_queryCmdCwdhcpsDef, at_setupCmdCwdhcpsDef, NULL},
    {"+CWDHCPS_CUR",  NULL, at_queryCmdCwdhcps, at_setupCmdCwdhcps, NULL},
#endif
#endif
    {"+CWAUTOCONN", NULL, at_queryCmdCwAutoConn, at_setupCmdCwAutoConn, NULL},
#ifdef AT_SMARTCONFIG_SUPPORT
    {"+CWSTARTSMART", NULL,    NULL, at_setupCmdSmartStart, NULL},
    {"+CWSTOPSMART",  NULL,    NULL,                  NULL, at_exeCmdSmartStop},
#endif
    {"+CWHOSTNAME",  NULL,    at_queryCmdCwHostname, at_setupCmdCwHostname, NULL},
#ifdef AT_WPS_SUPPORT
    {"+WPS", NULL, NULL,  at_setupCmdWps, NULL},
#endif
    {"+CIFSR", at_testCmdCifsr, NULL, at_setupCmdCifsr, at_exeCmdCifsr},
#ifdef AT_CMD_NO_OPTIMIZE
    {"+CIPSTAMAC", NULL, at_queryCmdCipstamac, at_setupCmdCipstamacDef, NULL},
    {"+CIPAPMAC", NULL, at_queryCmdCipapmac, at_setupCmdCipapmacDef, NULL},
    {"+CIPSTA", NULL, at_queryCmdCipsta, at_setupCmdCipstaDef, NULL},
    {"+CIPAP", NULL, at_queryCmdCipap, at_setupCmdCipapDef, NULL},
    {"+CIPDNS", NULL, at_queryCmdCipDns, at_setupCmdCipDnsDef, NULL},
#endif
#ifdef AT_CMD_OPTIMIZE
    {"+CIPSTAMAC_DEF", NULL, at_queryCmdCipstamacDef, at_setupCmdCipstamacDef, NULL},
    {"+CIPSTAMAC_CUR", NULL, at_queryCmdCipstamac, at_setupCmdCipstamac, NULL},
    {"+CIPAPMAC_DEF", NULL, at_queryCmdCipapmacDef, at_setupCmdCipapmacDef, NULL},
    {"+CIPAPMAC_CUR", NULL, at_queryCmdCipapmac, at_setupCmdCipapmac, NULL},
    {"+CIPSTA_DEF", NULL, at_queryCmdCipstaDef, at_setupCmdCipstaDef, NULL},
    {"+CIPSTA_CUR", NULL, at_queryCmdCipsta, at_setupCmdCipsta, NULL},
    {"+CIPAP_DEF", NULL, at_queryCmdCipapDef, at_setupCmdCipapDef, NULL},
    {"+CIPAP_CUR", NULL, at_queryCmdCipap, at_setupCmdCipap, NULL},
    {"+CIPDNS_DEF", NULL, at_queryCmdCipDnsDef, at_setupCmdCipDnsDef, NULL},
    {"+CIPDNS_CUR", NULL, at_queryCmdCipDns, at_setupCmdCipDns, NULL},
#endif
    {"+CIPDOMAIN", NULL, NULL, at_setupCmdCipdomain, NULL},
    {"+CIPSTATUS", NULL, NULL, NULL, at_exeCmdCipstatus},
    {"+CIPSTART", NULL, NULL, at_setupCmdCipstart, NULL},
    {"+CIPCLOSE", NULL, NULL, at_setupCmdCipclose, at_exeCmdCipclose},
    {"+CIPSEND", at_testCmdCipsend, NULL, at_setupCmdCipsend, at_exeCmdCipsend},
    {"+CIPSENDEX", at_testCmdCipsend, NULL, at_setupCmdCipsendEx, NULL},
    {"+CIPDINFO",  NULL, at_queryCmdCipIpdInfo, at_setupCmdCipIpdInfo, NULL},
    {"+CIPMUX", NULL, at_queryCmdCipmux, at_setupCmdCipmux, NULL},
    {"+CIPSERVER", NULL, NULL, at_setupCmdCipserver, NULL},

    {"+CIPMODE", NULL, at_queryCmdCipmode, at_setupCmdCipmode, NULL},
    {"+CIPSTO", NULL, at_queryCmdCipsto, at_setupCmdCipsto, NULL},
//    {"+CIPBUFRESET", 12, NULL, NULL, at_setupCmdCipBufReset, at_execCmdCipBufReset},
//    {"+CIPSENDBUF", 11, NULL, NULL,at_setupCmdCipSendBuf, NULL},
//    {"+CIPCHECKSEQ", 12, NULL, NULL, at_setupCmdCipCheckQueue, NULL},
//    {"+CIPCHECKQUEUE", 14, NULL, NULL, at_setupCmdCipCheckQueue, NULL},
//    {"+CIPBUFSTATUS", 13, NULL, NULL, at_setupCmdCipBufStatus, at_execCmdCipBufStatus},
//    {"+PING", 5, NULL, NULL, at_setupCmdPing, NULL},
    {"+SAVETRANSLINK", NULL, NULL, at_setupCmdSaveTransLink, NULL},

    {"+CIPSNTPCFG", NULL, at_queryCmdCipsntpcfg, at_setupCmdCipsntpcfg, at_exeCmdCipsntpcfg},
    {"+CIPSNTPTIME", NULL, at_queryCmdCipsntptime, NULL, NULL},



    {"+SYSRAM", NULL, at_queryCmdSysRam,NULL, NULL}
};

static esp_at_cmd_struct *at_custom_cmd_ptr = NULL;
static uint32_t at_custom_cmd_num = 0;

void esp_at_printf_error_code(uint32_t err_code)
{
    uint8_t *err_str = (uint8_t *)at_malloc(32);

    snprintf((char *)err_str, 32, "ERR CODE:0x%08x\r\n", err_code);
    at_port_print(err_str);
    at_free(err_str);
}

static bool at_cmd_parse(uint8_t *pStr, at_information_struct *at_info_ptr)
{

    // first two letter must be "AT" or "at" or "At" or "aT"
    //tmp_ptr = pStr;
    if ((at_info_ptr == NULL) || (pStr == NULL) || (at_strlen((char *)pStr) < 2)) {
        return FALSE;
    }
    at_memset(at_info_ptr, 0x00, sizeof(at_information_struct));
    at_info_ptr->op = AT_OP_INVALID;

    if (((*pStr != 'A') && (*pStr != 'a'))
            || ((*(pStr + 1) != 'T') && (*(pStr + 1) != 't'))) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_NOT_FOUND_AT);
        return FALSE;
    }
    pStr += 2;
    if (at_memcmp(pStr, AT_CMD_LINE_TERMINATOR, at_strlen(AT_CMD_LINE_TERMINATOR)) == 0) {
        // just AT
        at_info_ptr->cmd = NULL;
        at_info_ptr->para_num = 0;
        at_info_ptr->op = AT_OP_EXEC;
        return TRUE;
    }
    at_info_ptr->cmd = pStr;
    // get cmd
    at_info_ptr->cmd_len = 0;
    if (*pStr == '+') {
        pStr++;
        at_info_ptr->cmd_len++;
    }

    while (*pStr != '\0') {
        if ((*pStr >= 'A') && (*pStr <= 'Z')) { // OK
            pStr++;
        } else if ((*pStr >= 'a') && (*pStr <= 'z')) { // upper letter
            *pStr = (*pStr) - 'a' + 'A';
            pStr++;
        } else if ((*pStr >= '0') && (*pStr <= '9')) {
            pStr++;
        } else if ((*pStr == '!') || (*pStr == '%') || (*pStr == '-') || (*pStr == '.')
                   || (*pStr == '/') || (*pStr == ':') || (*pStr == '_')) {
            pStr++;
        } else {
            break;
        }
        at_info_ptr->cmd_len++;
    }
    // get operator
    if (*pStr == '?') {
        *pStr++ = '\0';
        at_info_ptr->op = AT_OP_QUERY;
        goto PARSE_END;
    } else if (*pStr == '=') {
        *pStr++ = '\0';
        if (*pStr == '?') {
            *pStr++ = '\0';
            at_info_ptr->op = AT_OP_TEST;
            goto PARSE_END;
        }
        at_info_ptr->op = AT_OP_SETUP;
    } else {
        goto PARSE_END;
    }
    // get parameter,like AT+CMD='abc'
    at_info_ptr->para_num = 0;
    at_info_ptr->para[at_info_ptr->para_num++] = pStr;
    while ((*pStr != '\r') && (*pStr != '\n')) {
        if (*pStr == '\\') {
            pStr += 2;
            continue;
        }

        if (*pStr == ',') {
            *pStr++ = '\0';
            at_info_ptr->para[at_info_ptr->para_num++] = pStr;
            continue;
        }
        pStr++;
    }

PARSE_END:
    if (at_memcmp(pStr, AT_CMD_LINE_TERMINATOR, at_strlen(AT_CMD_LINE_TERMINATOR)) == 0) {
        if (at_info_ptr->op == AT_OP_INVALID) {
            at_info_ptr->op = AT_OP_EXEC;
            *pStr = '\0';
        } else if (at_info_ptr->op == AT_OP_SETUP) {
            *pStr = '\0';
        }
        return TRUE;
    } else {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_NON_FINISH);
    }
    // TODO:print error number
    return FALSE;
}

static esp_at_cmd_struct *at_cmd_search(uint8_t *cmd, uint32_t cmd_len)
{
    int32_t cmd_index = 0;
    int32_t at_cmd_num = sizeof(at_default_cmd) / sizeof(at_default_cmd[0]);
    esp_at_cmd_struct *at_cmd_ptr = NULL;

    (void)cmd_len;

    if (cmd == NULL) {
        return &at_default_cmd[0];
    }
    for (cmd_index = 1; cmd_index < at_cmd_num; cmd_index++) {
        at_cmd_ptr = &at_default_cmd[cmd_index];
        if (at_strcmp((char *)cmd, at_cmd_ptr->at_cmdName) == 0) {
            break;
        }
    }

    if (cmd_index == at_cmd_num) {
        return NULL;
    }
    return at_cmd_ptr;
}

static esp_at_cmd_struct *at_custom_cmd_search(uint8_t *cmd, uint32_t cmd_len)
{
    int32_t cmd_index = 0;
    esp_at_cmd_struct *at_cmd_ptr = NULL;

    (void)cmd_len;

    if (cmd == NULL) {
        return NULL;
    }
    for (cmd_index = 0; cmd_index < at_custom_cmd_num; cmd_index++) {
        at_cmd_ptr = &at_custom_cmd_ptr[cmd_index];
        if (at_strcmp((char *)cmd, at_cmd_ptr->at_cmdName) == 0) {
            break;
        }
    }

    if (cmd_index == at_custom_cmd_num) {
        return NULL;
    }

    return at_cmd_ptr;
}


esp_at_para_parse_result_type esp_at_get_para_as_str(int32_t para_index, uint8_t **result)
{
    uint8_t *pStr = NULL;
    uint8_t *tmp_ptr = NULL;
    uint32_t para_len = 0;

    if (para_index > at_cmd_info.para_num) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(para_index));
        return FALSE;
    }

    pStr = at_cmd_info.para[para_index];
    if (pStr == NULL) {
        return ESP_AT_PARA_PARSE_RESULT_OMITTED;
    }

    if (*pStr == '\0') {
        // *result = pStr;
        *result = NULL;
        return ESP_AT_PARA_PARSE_RESULT_OK;
    }

    para_len = at_strlen((char *)pStr);
    if ((pStr[0] != '"') || (pStr[para_len - 1] != '"')) {
        esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(para_index));
        return ESP_AT_PARA_PARSE_RESULT_FAIL;
    }

    if ((pStr[0] == '"') && (pStr[1] == '"') && (para_len == 2)) {
        *result = NULL;
        return ESP_AT_PARA_PARSE_RESULT_OK;
    }

    pStr[para_len - 1] = '\0';
    pStr++;
    *result = pStr;
    tmp_ptr = pStr;
    while (*pStr != '\0') {
        if (*pStr == '\\') {
            if (pStr[1] == '\0') {
                esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(para_index));
                return ESP_AT_PARA_PARSE_RESULT_FAIL;
            }
            pStr++;
        } else if (*pStr == '"') {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(para_index));
            return ESP_AT_PARA_PARSE_RESULT_FAIL;
        }
        // Now get character
        *tmp_ptr++ = *pStr++;
    }
    *tmp_ptr = '\0';
    return ESP_AT_PARA_PARSE_RESULT_OK;
}

esp_at_para_parse_result_type esp_at_get_para_as_digit(int32_t para_index, int32_t *result)
{
    int32_t ret = 0;
    uint8_t base = 10;
    uint8_t *para_ptr = NULL;
    uint8_t ch = 0;
    int8_t sign = 1;
    uint32_t cutoff = 0;
    int32_t cutlim = 0;

    if (para_index > at_cmd_info.para_num) {
        goto ERROR_RET;
    }

    para_ptr = at_cmd_info.para[para_index];
    if (*para_ptr == '\0') {
        return ESP_AT_PARA_PARSE_RESULT_OMITTED;
    }

    if (*para_ptr == '0') {
        if (at_strlen((char *)para_ptr) >= 2) {
            if ((para_ptr[1] == 'x') || (para_ptr[1] == 'X')) {
                base = 16;
                para_ptr += 2;
            }
        }
    } else if (*para_ptr == '-') {
        sign = -1;
        para_ptr++;
    }
    cutoff = (unsigned long)0xffffffff / (unsigned long)base;
    cutlim = (unsigned long)0xffffffff % (unsigned long)base;
    while (*para_ptr != '\0') {
        if ((*para_ptr >= '0') && (*para_ptr <= '9')) {
            ch = *para_ptr - '0';
        } else if ((*para_ptr >= 'a') && (*para_ptr <= 'f')) {
            ch = *para_ptr - 'a' + 10;
        } else if ((*para_ptr >= 'A') && (*para_ptr <= 'F')) {
            ch = *para_ptr - 'A' + 10;
        } else {
            goto ERROR_RET;
        }

        if (ch >= base) {
            goto ERROR_RET;
        }

        if ((ret > cutoff) || (ret == cutoff && ch > cutlim)) {
            return ESP_AT_PARA_PARSE_RESULT_FAIL;
        }
        ret = ret * base + ch;
        para_ptr++;
    }

    *result = ret * sign;
    return ESP_AT_PARA_PARSE_RESULT_OK;

ERROR_RET:
    esp_at_printf_error_code(ESP_AT_CMD_ERROR_PARA_PARSE_FAIL(para_index));
    return ESP_AT_PARA_PARSE_RESULT_FAIL;
}



uint32_t at_cmd_process(uint8_t *pStr, uint16_t cmd_len)
{
    esp_at_cmd_struct *at_cmd_ptr = NULL;
    uint8_t ret = ESP_AT_RESULT_CODE_ERROR;

    if (at_cmd_parse(pStr, &at_cmd_info)) {
        at_cmd_ptr = at_cmd_search(at_cmd_info.cmd, at_cmd_info.cmd_len); // search at cmd
        if (at_cmd_ptr == NULL) {
            at_cmd_ptr = at_custom_cmd_search(at_cmd_info.cmd, at_cmd_info.cmd_len); // search at custom cmd
        }


        if (at_cmd_ptr == NULL) {
            esp_at_printf_error_code(ESP_AT_CMD_ERROR_CMD_UNSUPPORT);
            return ret;
        }

        if (at_cmd_info.op == AT_OP_TEST) {
            if (at_cmd_ptr->at_testCmd) {
                ret = at_cmd_ptr->at_testCmd(at_cmd_info.cmd);
            }
        } else if (at_cmd_info.op == AT_OP_QUERY) {
            if (at_cmd_ptr->at_queryCmd) {
                ret = at_cmd_ptr->at_queryCmd(at_cmd_info.cmd);
            }
        } else if (at_cmd_info.op == AT_OP_SETUP) {
            if (at_cmd_ptr->at_setupCmd) {
                ret = at_cmd_ptr->at_setupCmd(at_cmd_info.para_num);
            }
        } else if (at_cmd_info.op == AT_OP_EXEC) {
            if (at_cmd_ptr->at_exeCmd) {
                ret = at_cmd_ptr->at_exeCmd(at_cmd_info.cmd);
            }
        }
    }
    return ret;
}

bool esp_at_port_recv_data_notify (int32_t len, uint32_t msec)
{
    at_task_msg e;
    bool ret = FALSE;

    if (at_recv_data_queue == NULL) {
        return ret;
    }
    at_form_msg_p((&e), AT_MSG_ID_AT_PORT_RECV, len, NULL);
    if (msec == portMAX_DELAY) {
        if (xQueueSend(at_recv_data_queue, (void *)&e, portMAX_DELAY) == pdPASS) {
            ret = TRUE;
        }
    } else {
        if ((msec > 0) && (msec < (1000/xPortGetTickRateHz()))) {
            msec = 1;
        } else {
            msec = msec / (1000/xPortGetTickRateHz());
        }
        if (xQueueSend(at_recv_data_queue, (void *)&e, msec) == pdPASS) {
            ret = TRUE;
        } else {
            ret = FALSE;
        }
    }
    return ret;
}

void IRAM_ATTR esp_at_port_recv_data_notify_from_isr (int32_t len)
{
    portBASE_TYPE xHigherPriorityTaskWoken;
    at_task_msg e;
    
    if (at_recv_data_queue == NULL) {
        return;
    }
    at_form_msg_p_from_isr((&e), AT_MSG_ID_AT_PORT_RECV, len, NULL);
    if (xQueueSendFromISR(at_recv_data_queue, (void *)&e, &xHigherPriorityTaskWoken) == pdTRUE) {
        if (xHigherPriorityTaskWoken == pdTRUE) {
            portYIELD_FROM_ISR();
        }
    }
}

static int32_t  esp_at_port_write_data_default(uint8_t *data, int32_t len)
{
    return -1;
}
static int32_t esp_at_port_read_data_default (uint8_t*buf,int32_t len)
{
    return -1;
}

static bool esp_at_port_wait_write_complete_default (int32_t timeout_msec)
{
    return true;
}

static int32_t  esp_at_port_get_data_length_default (void)
{
    return -1;
}

static esp_at_custom_ops_struct esp_at_custom_ops;

void esp_at_custom_status_callback (esp_at_status_type status)
{
    if (esp_at_custom_ops.status_callback) {
        esp_at_custom_ops.status_callback(status);
    }
}

void esp_at_custom_ops_regist(esp_at_custom_ops_struct* ops)
{
    if (ops) {
        esp_at_custom_ops.status_callback = ops->status_callback;
    } else {
        esp_at_custom_ops.status_callback = NULL;
    }
}

uint32_t esp_at_get_version(void)
{
    return ((AT_VERSION_main << 24) | (AT_VERSION_sub << 16) | (AT_VERSION_test << 8) | (AT_VERSION_cust << 0));
}


static esp_at_device_ops_struct esp_at_device_ops = {
    .read_data = esp_at_port_read_data_default,
    .write_data = esp_at_port_write_data_default,
    .get_data_length = esp_at_port_get_data_length_default,
    .wait_write_complete = esp_at_port_wait_write_complete_default,
};

int32_t  esp_at_port_write_data(uint8_t *data, int32_t len)
{
    return esp_at_device_ops.write_data(data,len);
}
int32_t esp_at_port_read_data (uint8_t*buf,int32_t len)
{
    return esp_at_device_ops.read_data(buf,len);
}

 bool esp_at_port_wait_write_complete (int32_t timeout_msec)
{
    return esp_at_device_ops.wait_write_complete(timeout_msec);
}

 int32_t  esp_at_port_get_data_length (void)
{
    return esp_at_device_ops.get_data_length();
}

void at_port_print(uint8_t *str)
{
    if (str == NULL) {
        return;
    }
    esp_at_port_write_data(str, at_strlen(str));
}

void esp_at_device_ops_regist (esp_at_device_ops_struct* ops)
{
    if (ops == NULL) {
        esp_at_device_ops.read_data = esp_at_port_read_data_default;
        esp_at_device_ops.write_data = esp_at_port_write_data_default;
        esp_at_device_ops.get_data_length = esp_at_port_get_data_length_default;
        esp_at_device_ops.wait_write_complete = esp_at_port_wait_write_complete_default;
    } else {
        if (ops->read_data) {
            esp_at_device_ops.read_data = ops->read_data;
        } else {
            esp_at_device_ops.read_data = esp_at_port_read_data_default;
        }

        if (ops->write_data) {
            esp_at_device_ops.write_data = ops->write_data;
        } else {
            esp_at_device_ops.write_data = esp_at_port_write_data_default;
        }

        if (ops->get_data_length) {
            esp_at_device_ops.get_data_length = ops->get_data_length;
        } else {
            esp_at_device_ops.get_data_length = esp_at_port_get_data_length_default;
        }

        if (ops->wait_write_complete) {
            esp_at_device_ops.wait_write_complete = ops->wait_write_complete;
        } else {
             esp_at_device_ops.wait_write_complete = esp_at_port_wait_write_complete_default;
        }

    }
}

void esp_at_custom_cmd_array_regist (esp_at_cmd_struct *custom_at_cmd_array, uint32_t cmd_num)
{
    at_custom_cmd_ptr = custom_at_cmd_array;
    at_custom_cmd_num = cmd_num;
}


