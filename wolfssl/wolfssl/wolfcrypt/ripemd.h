/* ripemd.h
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
    \file wolfssl/wolfcrypt/ripemd.h
*/

#ifndef WOLF_CRYPT_RIPEMD_H
#define WOLF_CRYPT_RIPEMD_H

#include <wolfssl/wolfcrypt/types.h>

#ifdef WOLFSSL_RIPEMD

#ifdef __cplusplus
    extern "C" {
#endif


/* in bytes */
enum {
    RIPEMD             =  3,    /* hash type unique */
    RIPEMD_BLOCK_SIZE  = 64,
    RIPEMD_DIGEST_SIZE = 20,
    RIPEMD_PAD_SIZE    = 56
};


/* RipeMd 160 digest */
typedef struct RipeMd {
    word32  buffLen;   /* in bytes          */
    word32  loLen;     /* length in bytes   */
    word32  hiLen;     /* length in bytes   */
    word32  digest[RIPEMD_DIGEST_SIZE / sizeof(word32)];
    word32  buffer[RIPEMD_BLOCK_SIZE  / sizeof(word32)];
} RipeMd;


WOLFSSL_API int wc_InitRipeMd(RipeMd*);
WOLFSSL_API int wc_RipeMdUpdate(RipeMd*, const byte*, word32);
WOLFSSL_API int wc_RipeMdFinal(RipeMd*, byte*);


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* WOLFSSL_RIPEMD */
#endif /* WOLF_CRYPT_RIPEMD_H */
