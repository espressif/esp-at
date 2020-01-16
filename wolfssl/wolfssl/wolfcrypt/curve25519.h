/* curve25519.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/*!
    \file wolfssl/wolfcrypt/curve25519.h
*/


#ifndef WOLF_CRYPT_CURVE25519_H
#define WOLF_CRYPT_CURVE25519_H

#include <wolfssl/wolfcrypt/types.h>

#ifdef HAVE_CURVE25519

#include <wolfssl/wolfcrypt/fe_operations.h>
#include <wolfssl/wolfcrypt/random.h>

#ifdef WOLFSSL_ASYNC_CRYPT
    #include <wolfssl/wolfcrypt/async.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#define CURVE25519_KEYSIZE 32

/* curve25519 set type */
typedef struct {
    int size;       /* The size of the curve in octets */
    const char* name;     /* name of this curve */
} curve25519_set_type;


/* ECC point, the internal structure is Little endian
 * the mathematical functions used the endianess */
typedef struct {
    byte point[CURVE25519_KEYSIZE];
    #ifdef FREESCALE_LTC_ECC
        byte pointY[CURVE25519_KEYSIZE];
    #endif
} ECPoint;

/* A CURVE25519 Key */
typedef struct curve25519_key {
    int idx;            /* Index into the ecc_sets[] for the parameters of
                           this curve if -1, this key is using user supplied
                           curve in dp */
    const curve25519_set_type* dp;   /* domain parameters, either points to
                                   curves (idx >= 0) or user supplied */
    ECPoint   p;        /* public key  */
    ECPoint   k;        /* private key */

#ifdef WOLFSSL_ASYNC_CRYPT
    WC_ASYNC_DEV asyncDev;
#endif
} curve25519_key;

enum {
    EC25519_LITTLE_ENDIAN=0,
    EC25519_BIG_ENDIAN=1
};

WOLFSSL_API
int wc_curve25519_make_key(WC_RNG* rng, int keysize, curve25519_key* key);

WOLFSSL_API
int wc_curve25519_shared_secret(curve25519_key* private_key,
                                curve25519_key* public_key,
                                byte* out, word32* outlen);

WOLFSSL_API
int wc_curve25519_shared_secret_ex(curve25519_key* private_key,
                                   curve25519_key* public_key,
                                   byte* out, word32* outlen, int endian);

WOLFSSL_API
int wc_curve25519_init(curve25519_key* key);

WOLFSSL_API
void wc_curve25519_free(curve25519_key* key);


/* raw key helpers */
WOLFSSL_API
int wc_curve25519_import_private(const byte* priv, word32 privSz,
                                 curve25519_key* key);
WOLFSSL_API
int wc_curve25519_import_private_ex(const byte* priv, word32 privSz,
                                    curve25519_key* key, int endian);

WOLFSSL_API
int wc_curve25519_import_private_raw(const byte* priv, word32 privSz,
                            const byte* pub, word32 pubSz, curve25519_key* key);
WOLFSSL_API
int wc_curve25519_import_private_raw_ex(const byte* priv, word32 privSz,
                                        const byte* pub, word32 pubSz,
                                        curve25519_key* key, int endian);
WOLFSSL_API
int wc_curve25519_export_private_raw(curve25519_key* key, byte* out,
                                     word32* outLen);
WOLFSSL_API
int wc_curve25519_export_private_raw_ex(curve25519_key* key, byte* out,
                                        word32* outLen, int endian);

WOLFSSL_API
int wc_curve25519_import_public(const byte* in, word32 inLen,
                                curve25519_key* key);
WOLFSSL_API
int wc_curve25519_import_public_ex(const byte* in, word32 inLen,
                                   curve25519_key* key, int endian);
WOLFSSL_API
int wc_curve25519_check_public(const byte* pub, word32 pubSz, int endian);

WOLFSSL_API
int wc_curve25519_export_public(curve25519_key* key, byte* out, word32* outLen);
WOLFSSL_API
int wc_curve25519_export_public_ex(curve25519_key* key, byte* out,
                                   word32* outLen, int endian);

WOLFSSL_API
int wc_curve25519_export_key_raw(curve25519_key* key,
                                 byte* priv, word32 *privSz,
                                 byte* pub, word32 *pubSz);
WOLFSSL_API
int wc_curve25519_export_key_raw_ex(curve25519_key* key,
                                    byte* priv, word32 *privSz,
                                    byte* pub, word32 *pubSz,
                                    int endian);
/* size helper */
WOLFSSL_API
int wc_curve25519_size(curve25519_key* key);

#ifdef __cplusplus
    }    /* extern "C" */
#endif

#endif /* HAVE_CURVE25519 */
#endif /* WOLF_CRYPT_CURVE25519_H */

