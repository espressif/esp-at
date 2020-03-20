/* esp32-crypt.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */
#ifndef __ESP32_CRYPT_H__

#define __ESP32_CRYPT_H__

#include "esp_idf_version.h"
#include "esp_types.h"
#include "esp_log.h"

#ifdef WOLFSSL_ESP32WROOM32_CRYPT_DEBUG
#undef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#else
#undef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#endif

#include <freertos/FreeRTOS.h>
#include "soc/dport_reg.h"
#include "soc/hwcrypto_reg.h"
#include "soc/cpu.h"
#include "driver/periph_ctrl.h"
#if ESP_IDF_VERSION_MAJOR >= 4 && ESP_IDF_VERSION_MINOR >= 1
#include <esp32/rom/ets_sys.h>
#else
#include <rom/ets_sys.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int esp_CryptHwMutexInit(wolfSSL_Mutex* mutex);
int esp_CryptHwMutexLock(wolfSSL_Mutex* mutex, TickType_t xBloxkTime);
int esp_CryptHwMutexUnLock(wolfSSL_Mutex* mutex);

#ifndef NO_AES

#if ESP_IDF_VERSION_MAJOR >= 4 && ESP_IDF_VERSION_MINOR >= 1
#include "esp32/rom/aes.h"
#else
#include "rom/aes.h"
#endif

typedef enum tagES32_AES_PROCESS {
    ESP32_AES_LOCKHW = 1,
    ESP32_AES_UPDATEKEY_ENCRYPT = 2,
    ESP32_AES_UPDATEKEY_DECRYPT = 3,
    ESP32_AES_UNLOCKHW          = 4
} ESP32_AESPROCESS;

struct Aes;
int wc_esp32AesCbcEncrypt(struct Aes* aes, byte* out, const byte* in, word32 sz);
int wc_esp32AesCbcDecrypt(struct Aes* aes, byte* out, const byte* in, word32 sz);
int wc_esp32AesEncrypt(struct Aes *aes, const byte* in, byte* out);
int wc_esp32AesDecrypt(struct Aes *aes, const byte* in, byte* out);

#endif

#ifdef WOLFSSL_ESP32WROOM32_CRYPT_DEBUG

void wc_esp32TimerStart();
uint64_t  wc_esp32elapsedTime();

#endif /* WOLFSSL_ESP32WROOM32_CRYPT_DEBUG */

#if (!defined(NO_SHA) || !defined(NO_SHA256) || defined(WOLFSSL_SHA384) || \
      defined(WOLFSSL_SHA512)) && \
    !defined(NO_WOLFSSL_ESP32WROOM32_CRYPT_HASH)

/* RAW hash function APIs are not implemented with esp32 hardware acceleration*/
#define WOLFSSL_NO_HASH_RAW
#if ESP_IDF_VERSION_MAJOR >= 4 && ESP_IDF_VERSION_MINOR >= 1
#include "esp32/rom/sha.h"
#else
#include "rom/sha.h"
#endif

typedef enum {
    ESP32_SHA_INIT = 0,
    ESP32_SHA_HW = 1,
    ESP32_SHA_SW = 2,
} ESP32_DOSHA;

typedef struct {
    byte isfirstblock;
    /* 0 , 1 hard, 2 soft */
    byte mode;
    /* sha_type */
    enum SHA_TYPE sha_type;
} WC_ESP32SHA;

int esp_sha_try_hw_lock(WC_ESP32SHA* ctx);
void esp_sha_hw_unlock( void );

struct wc_Sha;
int esp_sha_digest_process(struct wc_Sha* sha, byte bockprocess);
int esp_sha_process(struct wc_Sha* sha, const byte* data);

#ifndef NO_SHA256
    struct wc_Sha256;
    int esp_sha256_digest_process(struct wc_Sha256* sha, byte bockprocess);
    int esp_sha256_process(struct wc_Sha256* sha, const byte* data);
#endif

#if defined(WOLFSSL_SHA512) || defined(WOLFSSL_SHA384)
    struct wc_Sha512;
    int esp_sha512_process(struct wc_Sha512* sha);
    int esp_sha512_digest_process(struct wc_Sha512* sha, byte blockproc);
#endif

#endif /* NO_SHA && */

#if !defined(NO_RSA) || defined(HAVE_ECC)

#ifndef ESP_RSA_TIMEOUT
    #define ESP_RSA_TIMEOUT 0xFFFFF
#endif

struct fp_int;
int esp_mp_mul(struct fp_int* X, struct fp_int* Y, struct fp_int* Z);
int esp_mp_exptmod(struct fp_int* G, struct fp_int* X, word32 Xbits, struct fp_int* P,
                                     struct fp_int* Y);
int esp_mp_mulmod(struct fp_int* X, struct fp_int* Y, struct fp_int* M,
                                     struct fp_int* Z);

#endif /* NO_RSA || HAVE_ECC*/

#ifdef __cplusplus
}
#endif

#endif  /* __ESP32_CRYPT_H__ */
