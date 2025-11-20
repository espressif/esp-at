/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "esp_at.h"
#include "esp_flash.h"
#include "esp_random.h"
#include "mbedtls/aes.h"
#include "esp_log.h"
#include "at_custom_cmd.h"

#define AT_AES_PK_LEN           16         /* 128 bits. Optional: 128, 192, 256 bits. */
#define AT_AES_IV_LEN           16         /* IV length for AES-CTR mode */
#define LOWER(c)                (unsigned char)((c) | 0x20)
#define IS_NUM(c)               ((c) >= '0' && (c) <= '9')
#define IS_HEX_ALPHA(c)         ((LOWER(c) >= 'a') && (LOWER(c) <= 'f'))
#define MIN(a, b)               ((a) < (b) ? (a) : (b))

static uint8_t s_key[AT_AES_PK_LEN] = "ESP-AT-AES-KEY";
static const char *TAG = "at-storage-sec";

static void at_storage_security_get_key(uint8_t key[AT_AES_PK_LEN])
{
    memcpy(key, s_key, AT_AES_PK_LEN);
}

static void at_storage_security_set_key(uint8_t key[AT_AES_PK_LEN])
{
    memcpy(s_key, key, AT_AES_PK_LEN);
}

static bool at_nvs_key_encrypt(const char *key)
{
    if (!key) {
        return false;
    }

    // encrypt the key containing "key" keyword
    if (strstr(key, "key")) {
        return true;
    }

    // encrypt the key related to Wi-Fi authentication info
    if (strcmp(key, "sta.pswd") == 0 || strcmp(key, "sta.apinfo") == 0) {
        return true;
    }

    return false;
}

esp_err_t __wrap_nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length)
{
    if (!at_nvs_key_encrypt(key)) {
        return __real_nvs_set_blob(handle, key, value, length);
    }

    if (!value || length == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t *blob = (uint8_t *)malloc(length + AT_AES_IV_LEN);
    if (!blob) {
        return ESP_ERR_NO_MEM;
    }

    // set up AES context
    esp_err_t ret = ESP_FAIL;
    uint8_t aes_key[AT_AES_PK_LEN];
    at_storage_security_get_key(aes_key);
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    if (mbedtls_aes_setkey_enc(&ctx, aes_key, AT_AES_PK_LEN * 8) != 0) {
        goto cleanup;
    }

    // generate random IV and store at the beginning
    uint8_t iv[AT_AES_IV_LEN];
    esp_fill_random(iv, AT_AES_IV_LEN);
    memcpy(blob, iv, AT_AES_IV_LEN);

    // encrypt data (ciphertext stored after IV)
    uint8_t *ciphertext = blob + AT_AES_IV_LEN;
    size_t nc_off = 0;
    uint8_t stream_block[16] = {0};
    if (mbedtls_aes_crypt_ctr(&ctx, length, &nc_off, iv, stream_block, value, ciphertext) != 0) {
        goto cleanup;
    }

    // store IV + ciphertext
    ret = __real_nvs_set_blob(handle, key, blob, length + AT_AES_IV_LEN);

    ESP_LOGI(TAG, "nvs_set_blob encrypted, key:%s, len:%d, ret:0x%x", key, length, ret);
    ESP_LOG_BUFFER_HEXDUMP("plaintext", value, length, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEXDUMP("iv", blob, AT_AES_IV_LEN, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEXDUMP("ciphertext", ciphertext, length, ESP_LOG_INFO);

cleanup:
    free(blob);
    mbedtls_aes_free(&ctx);
    return ret;
}

esp_err_t __wrap_nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length)
{
    if (!at_nvs_key_encrypt(key)) {
        return __real_nvs_get_blob(handle, key, out_value, length);
    }

    if (!length) {
        return ESP_ERR_INVALID_ARG;
    }

    // get length of IV + ciphertext
    size_t stored_length = 0;
    esp_err_t ret = __real_nvs_get_blob(handle, key, NULL, &stored_length);
    if (ret != ESP_OK) {
        return ret;
    }
    if (stored_length < AT_AES_IV_LEN) {
        ESP_LOGE(TAG, "Invalid encrypted blob size: %d for %s", stored_length, key);
        return ESP_ERR_INVALID_SIZE;
    }

    // calculate actual data length (excluding IV)
    size_t data_length = stored_length - AT_AES_IV_LEN;
    if (!out_value) {
        *length = data_length;
        return ESP_OK;
    }

    // get blob of IV + ciphertext
    uint8_t *blob = (uint8_t *)malloc(stored_length);
    if (!blob) {
        return ESP_ERR_NO_MEM;
    }
    ret = __real_nvs_get_blob(handle, key, blob, &stored_length);
    if (ret != ESP_OK) {
        free(blob);
        return ret;
    }

    // set up AES context
    uint8_t aes_key[AT_AES_PK_LEN];
    at_storage_security_get_key(aes_key);
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    if (mbedtls_aes_setkey_enc(&ctx, aes_key, AT_AES_PK_LEN * 8) != 0) {
        ret = ESP_FAIL;
        goto cleanup;
    }

    // decrypt data
    uint8_t iv[AT_AES_IV_LEN];
    memcpy(iv, blob, AT_AES_IV_LEN);
    uint8_t *ciphertext = blob + AT_AES_IV_LEN;
    size_t nc_off = 0;
    uint8_t stream_block[16] = {0};
    size_t decrypt_len = MIN(data_length, *length);
    if (mbedtls_aes_crypt_ctr(&ctx, decrypt_len, &nc_off, iv, stream_block, ciphertext, out_value) != 0) {
        ret = ESP_FAIL;
        goto cleanup;
    }

    *length = decrypt_len;

    ESP_LOGI(TAG, "nvs_get_blob decrypted, key:%s, len:%d, ret:0x%x", key, decrypt_len, ret);
    ESP_LOG_BUFFER_HEXDUMP("ciphertext", ciphertext, data_length, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEXDUMP("iv", blob, AT_AES_IV_LEN, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEXDUMP("plaintext", out_value, decrypt_len, ESP_LOG_INFO);

cleanup:
    free(blob);
    mbedtls_aes_free(&ctx);
    return ret;
}

esp_err_t __wrap_nvs_set_str(nvs_handle_t handle, const char *key, const char *value)
{
    if (!at_nvs_key_encrypt(key)) {
        return __real_nvs_set_str(handle, key, value);
    }

    if (!value) {
        return ESP_ERR_INVALID_ARG;
    }

    // after encryption, the string may contain '\0', so use blob interface to store it
    return __wrap_nvs_set_blob(handle, key, value, strlen(value));
}

esp_err_t __wrap_nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length)
{
    if (!at_nvs_key_encrypt(key)) {
        return __real_nvs_get_str(handle, key, out_value, length);
    }

    // after decryption, the string may contain '\0', so use blob interface to get it
    return __wrap_nvs_get_blob(handle, key, out_value, length);
}

static uint8_t hex_char_to_value(char c)
{
    if (IS_NUM(c)) {
        return c - '0';
    } else {
        return LOWER(c) - 'a' + 0x0A;
    }
}

static uint8_t at_setup_cmd_storage_nvs_key(uint8_t para_num)
{
    int32_t cnt = 0;
    uint8_t *key = NULL;

    // AES key parameter
    if (esp_at_get_para_as_str(cnt++, &key) != ESP_AT_PARA_PARSE_RESULT_OK) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    if (at_str_is_null(key)) {
        return ESP_AT_RESULT_CODE_ERROR;
    }
    int key_len = strlen((char *)key);
    if (key_len != AT_AES_PK_LEN * 2) {
        ESP_LOGE(TAG, "Invalid key length: %d, expected: %d", key_len, AT_AES_PK_LEN * 2);
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // validate hex string
    for (int i = 0; i < key_len; i++) {
        if (!IS_NUM(key[i]) && !IS_HEX_ALPHA(key[i])) {
            ESP_LOGE(TAG, "Invalid hex character at position %d", i);
            return ESP_AT_RESULT_CODE_ERROR;
        }
    }

    if (cnt != para_num) {
        return ESP_AT_RESULT_CODE_ERROR;
    }

    // convert hex string to binary key
    uint8_t aes_key[AT_AES_PK_LEN];
    for (int i = 0; i < AT_AES_PK_LEN; ++i) {
        uint8_t c1 = hex_char_to_value(key[i * 2]);
        uint8_t c2 = hex_char_to_value(key[i * 2 + 1]);
        aes_key[i] = (c1 << 4) | c2;
    }
    at_storage_security_set_key(aes_key);

    return ESP_AT_RESULT_CODE_OK;
}

static uint8_t at_query_cmd_storage_nvs_key(uint8_t *cmd_name)
{
    uint8_t buffer[AT_BUFFER_ON_STACK_SIZE] = {0};

    uint8_t aes_key[AT_AES_PK_LEN];
    at_storage_security_get_key(aes_key);

    int ret = 0;
    ret = snprintf((char *)buffer, AT_BUFFER_ON_STACK_SIZE, "%s:", cmd_name);
    for (int i = 0; i < AT_AES_PK_LEN; ++i) {
        ret += snprintf((char *)buffer + ret, AT_BUFFER_ON_STACK_SIZE - ret, "%02x", aes_key[i]);
    }
    ret += snprintf((char *)buffer + ret, AT_BUFFER_ON_STACK_SIZE - ret, "\r\n");

    esp_at_port_write_data(buffer, ret);

    return ESP_AT_RESULT_CODE_OK;
}

static const esp_at_cmd_struct at_custom_cmd[] = {
    {"+STORAGE_NVS_KEY", NULL, at_query_cmd_storage_nvs_key, at_setup_cmd_storage_nvs_key, NULL},
    /**
     * @brief You can define your own AT commands here.
     */
};

bool esp_at_storage_nvs_key_cmd_register(void)
{
    return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
}

ESP_AT_CMD_SET_INIT_FN(esp_at_storage_nvs_key_cmd_register, 1);
