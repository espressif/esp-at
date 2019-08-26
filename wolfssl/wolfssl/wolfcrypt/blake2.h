/* blake2.h
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
    \file wolfssl/wolfcrypt/blake2.h
*/

#ifndef WOLF_CRYPT_BLAKE2_H
#define WOLF_CRYPT_BLAKE2_H

#include <wolfssl/wolfcrypt/settings.h>

#if defined(HAVE_BLAKE2) || defined(HAVE_BLAKE2S)

#include <wolfssl/wolfcrypt/blake2-int.h>

/* call old functions if using fips for the sake of hmac @wc_fips */
#ifdef HAVE_FIPS
    /* Since hmac can call blake functions provide original calls */
    #define wc_InitBlake2b   InitBlake2b
    #define wc_Blake2bUpdate Blake2bUpdate
    #define wc_Blake2bFinal  Blake2bFinal
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/* in bytes, variable digest size up to 512 bits (64 bytes) */
enum {
#ifdef HAVE_BLAKE2B
    BLAKE2B_ID  = WC_HASH_TYPE_BLAKE2B,
    BLAKE2B_256 = 32,  /* 256 bit type, SSL default */
#endif
#ifdef HAVE_BLAKE2S
    BLAKE2S_ID  = WC_HASH_TYPE_BLAKE2S,
    BLAKE2S_256 = 32   /* 256 bit type */
#endif
};


#ifdef HAVE_BLAKE2B
/* BLAKE2b digest */
typedef struct Blake2b {
    blake2b_state S[1];         /* our state */
    word32        digestSz;     /* digest size used on init */
} Blake2b;
#endif

#ifdef HAVE_BLAKE2S
/* BLAKE2s digest */
typedef struct Blake2s {
    blake2s_state S[1];         /* our state */
    word32        digestSz;     /* digest size used on init */
} Blake2s;
#endif


#ifdef HAVE_BLAKE2B
WOLFSSL_API int wc_InitBlake2b(Blake2b*, word32);
WOLFSSL_API int wc_Blake2bUpdate(Blake2b*, const byte*, word32);
WOLFSSL_API int wc_Blake2bFinal(Blake2b*, byte*, word32);
#endif

#ifdef HAVE_BLAKE2S
WOLFSSL_API int wc_InitBlake2s(Blake2s*, word32);
WOLFSSL_API int wc_Blake2sUpdate(Blake2s*, const byte*, word32);
WOLFSSL_API int wc_Blake2sFinal(Blake2s*, byte*, word32);
#endif


#ifdef __cplusplus
    }
#endif

#endif  /* HAVE_BLAKE2 || HAVE_BLAKE2S */
#endif  /* WOLF_CRYPT_BLAKE2_H */

