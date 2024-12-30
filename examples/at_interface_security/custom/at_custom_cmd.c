/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "esp_at.h"
#include "mbedtls/aes.h"
#include "esp_log.h"
#include "esp_at_interface.h"

#ifdef CONFIG_AT_INTF_SECURITY_SUPPORT
#define AT_AES_PK_LEN    16         /* 128 bits. Optional: 128, 192, 256 bits. */
#define AT_TX_DATA_LEN_MAX  4096    /* The maximum length of data that can be sent in one go */
#define AT_RX_DATA_LEN_MAX  8192    /* The maximum length of data that can be received in one go */

typedef struct {
    mbedtls_aes_context ctx;        /* mbedtls context data for AES */
    uint8_t iv[16];                 /* The 128-bit nonce and counter */
    uint8_t stream_block[16];       /* The saved stream block for resuming */
    size_t offset;                  /* The offset in the current \p stream_block, for resuming within the current cipher stream. */
    uint8_t *buffer;                /* The buffer holding the data */
} at_security_t;

// two security contexts for interface tx (index:0) and rx (index:1)
static at_security_t *s_ctx;

static const char *TAG = "at-intf-sec";

// You MUST absolutely modify its implement in your real product, according to <AES Key and IV Management> section in the example README.md
static void at_port_security_get_key(uint8_t key[AT_AES_PK_LEN])
{
    memset(key, 'A', AT_AES_PK_LEN);
}

// You MUST absolutely modify its implement in your real product, according to <AES Key and IV Management> section in the example README.md
static void at_port_security_get_iv(uint8_t iv[16])
{
    memset(iv, 'T', 16);
}

static void at_port_security_close(void)
{
    if (s_ctx) {
        mbedtls_aes_free(&s_ctx[0].ctx);
        if (s_ctx[0].buffer) {
            free(s_ctx[0].buffer);
        }
        mbedtls_aes_free(&s_ctx[1].ctx);
        if (s_ctx[1].buffer) {
            free(s_ctx[1].buffer);
        }
        free(s_ctx);
        s_ctx = NULL;
    }
    ESP_LOGI(TAG, "AT port security closed");
}

static int at_port_security_open(void)
{
    s_ctx = (at_security_t *)calloc(2, sizeof(at_security_t));
    if (!s_ctx) {
        ESP_LOGE(TAG, "calloc failed");
        return -1;
    }
    s_ctx[0].buffer = (uint8_t *)malloc(AT_TX_DATA_LEN_MAX);
    s_ctx[1].buffer = (uint8_t *)malloc(AT_RX_DATA_LEN_MAX);
    if (!s_ctx[0].buffer || !s_ctx[1].buffer) {
        ESP_LOGE(TAG, "malloc failed");
        at_port_security_close();
        return -1;
    }

    // set key
    uint8_t key[AT_AES_PK_LEN];
    at_port_security_get_key(key);
    mbedtls_aes_init(&s_ctx[0].ctx);
    if (mbedtls_aes_setkey_enc(&s_ctx[0].ctx, key, AT_AES_PK_LEN * 8) != 0) {
        ESP_LOGE(TAG, "setkey failed");
        at_port_security_close();
        return -1;
    }
    mbedtls_aes_init(&s_ctx[1].ctx);
    if (mbedtls_aes_setkey_enc(&s_ctx[1].ctx, key, AT_AES_PK_LEN * 8) != 0) {
        ESP_LOGE(TAG, "setkey failed");
        at_port_security_close();
        return -1;
    }

    // set iv
    uint8_t nonce[16];
    at_port_security_get_iv(nonce);
    memcpy(s_ctx[0].iv, nonce, 16);
    memcpy(s_ctx[1].iv, nonce, 16);

    ESP_LOGI(TAG, "AT port security opened");
    return 0;
}

static int32_t at_port_security_read(uint8_t *data, int32_t size)
{
    if (!s_ctx) {
        ESP_LOGE(TAG, "Security context not initialized");
        return -1;
    }
    if (size > AT_RX_DATA_LEN_MAX) {
        ESP_LOGE(TAG, "rx size overlength");
        return -1;
    }

    at_read_data_fn_t read_fn = at_interface_get_read_fn();
    int buffered_len = read_fn(s_ctx[1].buffer, size);
    if (buffered_len <= 0) {
        return buffered_len;
    }

    ESP_LOG_BUFFER_HEXDUMP("intf-sec-rx", data, buffered_len, ESP_LOG_INFO);

    for (int idx = 0; idx < buffered_len; idx = idx + buffered_len) {
        mbedtls_aes_crypt_ctr(&s_ctx[1].ctx, buffered_len - idx, &s_ctx[1].offset, s_ctx[1].iv, s_ctx[1].stream_block, s_ctx[1].buffer + idx, data + idx);
    }

    return buffered_len;
}

static int32_t at_port_security_write(uint8_t *data, int32_t size)
{
    if (!s_ctx) {
        ESP_LOGE(TAG, "Security context not initialized");
        return -1;
    }
    if (size > AT_TX_DATA_LEN_MAX) {
        ESP_LOGE(TAG, "tx size overlength");
        return -1;
    }

    for (int idx = 0; idx < size; idx = idx + size) {
        mbedtls_aes_crypt_ctr(&s_ctx[0].ctx, size - idx, &s_ctx[0].offset, s_ctx[0].iv, s_ctx[0].stream_block, data + idx, s_ctx[0].buffer + idx);
    }

    ESP_LOG_BUFFER_HEXDUMP("intf-sec-tx", s_ctx[0].buffer, size, ESP_LOG_INFO);

    at_write_data_fn_t write_fn = at_interface_get_write_fn();
    return write_fn(s_ctx[0].buffer, size);
}

void esp_at_ready_before(void)
{
    at_intf_security_ops_t ops = {
        .open = &at_port_security_open,
        .read = &at_port_security_read,
        .write = &at_port_security_write,
        .close = &at_port_security_close,
    };

    // switch to the security channel over the interface
    at_interface_security_set(&ops);
}
#endif
