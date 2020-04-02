/* user_settings.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#include "sdkconfig.h"

#define WOLFSSL_ESPIDF
#define WOLFSSL_ESPWROOM32

#define BENCH_EMBEDDED
#define USE_CERT_BUFFERS_2048

#define HAVE_TLS_EXTENSIONS
#define WC_RSA_PSS
#define HAVE_HKDF
#define HAVE_AEAD
#define HAVE_SUPPORTED_CURVES
#define HAVE_SNI
/* ALPN in wolfSSL is enabled by default,can be disabled with menuconfig */
#define HAVE_ALPN
/* when you want to use SINGLE THREAD */
/* #define SINGLE_THREADED */
#define NO_FILESYSTEM
#define WOLFSSL_STATIC_PSK
#define HAVE_AESGCM
/* when you want to use SHA384 */
/* #define WOLFSSL_SHA384 */
#define WOLFSSL_SHA512
#define HAVE_ECC
#define HAVE_CURVE25519
#define CURVE25519_SMALL
#define HAVE_ED25519
/* do not use wolfssl defined app_main function used to test esp-wolfssl */
#define NO_MAIN_DRIVER
/* you can disable folowing cipher suits by uncommenting following lines */
//#define NO_DSA
//#define NO_DH

/* These Flags are defined to make wolfssl not use some insecure cipher suites */
#define NO_MD4
#define NO_DES3
#define NO_RC4
#define NO_RABBIT

#define OPENSSL_EXTRA_X509_SMALL /* Allows of x509 certs (for wolfssl_get_verify_result function)*/
#define WOLFSSL_ALT_CERT_CHAINS /* Allow to try alternate cert chain */

#define WOLFSSL_BASE64_ENCODE
//#define OPENSSL_EXTRA
#define OPENSSL_ALL
/* If you want to authenticate the server with Intermediate CA cert, enable following flag */
#define WOLFSSL_SMALL_CERT_VERIFY

/* Reduces the Cache used by wolfssl and thus reduces heap used */
#define WOLFSSL_SMALL_STACK
#define SMALL_SESSION_CACHE
/* esp32-wroom-32se specific definition */
#if defined(WOLFSSL_ESPWROOM32SE)
    #define WOLFSSL_ATECC508A
    #define HAVE_PK_CALLBACKS
    /* when you want to use a custom slot allocation for ATECC608A */
    /* unless your configuration is unusual, you can use default   */
    /* implementation.                                             */
    /* #define CUSTOM_SLOT_ALLOCATION                              */
#endif

/* rsa primitive specific definition */
#if defined(WOLFSSL_ESPWROOM32) || defined(WOLFSSL_ESPWROOM32SE)
    /* Define USE_FAST_MATH and SMALL_STACK                        */
    #define ESP32_USE_RSA_PRIMITIVE
    /* threshold for performance adjustment for hw primitive use   */
    /* X bits of G^X mod P greater than                            */ 
    #define EPS_RSA_EXPT_XBTIS           36
    /* X and Y of X * Y mod P greater than                         */
    #define ESP_RSA_MULM_BITS            2000
#endif

/* debug options */
/* #define DEBUG_WOLFSSL */
/* #define WOLFSSL_ESP32WROOM32_CRYPT_DEBUG */
/* #define WOLFSSL_ATECC508A_DEBUG          */

/* date/time                               */
/* if it cannot adjust time in the device, */
/* enable macro below                      */
#define NO_ASN_TIME
#define XTIME time
#define XGMTIME(c, t) gmtime((c))

/* when you want not to use HW acceleration */
#if !defined(CONFIG_IDF_TARGET_ESP32)
#define NO_ESP32WROOM32_CRYPT
#endif
/* Turn off the sha acceleration for esp32 */
#define NO_WOLFSSL_ESP32WROOM32_CRYPT_HASH
/* #define NO_WOLFSSL_ESP32WROOM32_CRYPT_AES */
/* #define NO_WOLFSSL_ESP32WROOM32_CRYPT_RSA_PRI */
