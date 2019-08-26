/* poly1305.h
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
    \file wolfssl/wolfcrypt/poly1305.h
*/

#ifndef WOLF_CRYPT_POLY1305_H
#define WOLF_CRYPT_POLY1305_H

#include <wolfssl/wolfcrypt/types.h>

#ifdef HAVE_POLY1305

#ifdef __cplusplus
    extern "C" {
#endif

/* auto detect between 32bit / 64bit */
#if defined(__SIZEOF_INT128__) && defined(__LP64__)
#define WC_HAS_SIZEOF_INT128_64BIT
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#define WC_HAS_MSVC_64BIT
#endif

#if (defined(__GNUC__) && defined(__LP64__) && \
        ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4))))
#define WC_HAS_GCC_4_4_64BIT
#endif

#ifdef USE_INTEL_SPEEDUP
#elif (defined(WC_HAS_SIZEOF_INT128_64BIT) || defined(WC_HAS_MSVC_64BIT) ||  \
       defined(WC_HAS_GCC_4_4_64BIT))
#define POLY130564
#else
#define POLY130532
#endif

enum {
    POLY1305 = 7,
    POLY1305_BLOCK_SIZE = 16,
    POLY1305_DIGEST_SIZE = 16,
};

#define WC_POLY1305_PAD_SZ 16
#define WC_POLY1305_MAC_SZ 16

/* Poly1305 state */
typedef struct Poly1305 {
#ifdef USE_INTEL_SPEEDUP
    word64 r[3];
    word64 h[3];
    word64 pad[2];
    word64 hh[20];
    word32 r1[8];
    word32 r2[8];
    word32 r3[8];
    word32 r4[8];
    word64 hm[16];
    unsigned char buffer[8*POLY1305_BLOCK_SIZE];
    size_t leftover;
    unsigned char finished;
    unsigned char started;
#else
#if defined(WOLFSSL_ARMASM) && defined(__aarch64__)
    ALIGN128 word32 r[5];
    ALIGN128 word32 r_2[5]; // r^2
    ALIGN128 word32 r_4[5]; // r^4
    ALIGN128 word32 h[5];
    word32 pad[4];
    word64 leftover;
#else
#if defined(POLY130564)
    word64 r[3];
    word64 h[3];
    word64 pad[2];
#else
    word32 r[5];
    word32 h[5];
    word32 pad[4];
#endif
    size_t leftover;
#endif /* WOLFSSL_ARMASM */
    unsigned char buffer[POLY1305_BLOCK_SIZE];
    unsigned char finished;
#endif
} Poly1305;

/* does init */

WOLFSSL_API int wc_Poly1305SetKey(Poly1305* poly1305, const byte* key,
                                  word32 kySz);
WOLFSSL_API int wc_Poly1305Update(Poly1305* poly1305, const byte*, word32);
WOLFSSL_API int wc_Poly1305Final(Poly1305* poly1305, byte* tag);
WOLFSSL_API int wc_Poly1305_MAC(Poly1305* ctx, byte* additional, word32 addSz,
                               byte* input, word32 sz, byte* tag, word32 tagSz);

void poly1305_block(Poly1305* ctx, const unsigned char *m);
void poly1305_blocks(Poly1305* ctx, const unsigned char *m,
                            size_t bytes);
#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* HAVE_POLY1305 */
#endif /* WOLF_CRYPT_POLY1305_H */

