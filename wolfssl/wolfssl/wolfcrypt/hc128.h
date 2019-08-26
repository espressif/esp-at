/* hc128.h
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
    \file wolfssl/wolfcrypt/hc128.h
*/


#ifndef WOLF_CRYPT_HC128_H
#define WOLF_CRYPT_HC128_H

#include <wolfssl/wolfcrypt/types.h>

#ifndef NO_HC128

#ifdef __cplusplus
    extern "C" {
#endif

enum {
	HC128_ENC_TYPE =  WC_CIPHER_HC128,     /* cipher unique type */
};

/* HC-128 stream cipher */
typedef struct HC128 {
    word32 T[1024];             /* P[i] = T[i];  Q[i] = T[1024 + i ]; */
    word32 X[16];
    word32 Y[16];
    word32 counter1024;         /* counter1024 = i mod 1024 at the ith step */
    word32 key[8];
    word32 iv[8];
#ifdef XSTREAM_ALIGN
    void*  heap;  /* heap hint, currently XMALLOC only used with aligning */
#endif
} HC128;


WOLFSSL_API int wc_Hc128_Process(HC128*, byte*, const byte*, word32);
WOLFSSL_API int wc_Hc128_SetKey(HC128*, const byte* key, const byte* iv);

WOLFSSL_LOCAL int wc_Hc128_SetHeap(HC128* ctx, void* heap);

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* HAVE_HC128 */
#endif /* WOLF_CRYPT_HC128_H */

