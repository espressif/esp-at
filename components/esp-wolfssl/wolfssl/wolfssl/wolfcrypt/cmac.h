/* cmac.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLF_CRYPT_CMAC_H
#define WOLF_CRYPT_CMAC_H

#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/aes.h>

#if !defined(NO_AES) && defined(WOLFSSL_CMAC)

#if defined(HAVE_FIPS) && \
    defined(HAVE_FIPS_VERSION) && (HAVE_FIPS_VERSION >= 2)
    #include <wolfssl/wolfcrypt/fips.h>
#endif /* HAVE_FIPS_VERSION >= 2 */

#ifdef __cplusplus
    extern "C" {
#endif

/* avoid redefinition of structs */
#if !defined(HAVE_FIPS) || \
    (defined(HAVE_FIPS_VERSION) && (HAVE_FIPS_VERSION >= 2))

#ifndef WC_CMAC_TYPE_DEFINED
    typedef struct Cmac Cmac;
    #define WC_CMAC_TYPE_DEFINED
#endif
struct Cmac {
    Aes aes;
    byte buffer[AES_BLOCK_SIZE]; /* partially stored block */
    byte digest[AES_BLOCK_SIZE]; /* running digest */
    byte k1[AES_BLOCK_SIZE];
    byte k2[AES_BLOCK_SIZE];
    word32 bufferSz;
    word32 totalSz;
};



typedef enum CmacType {
    WC_CMAC_AES = 1
} CmacType;

#define WC_CMAC_TAG_MAX_SZ AES_BLOCK_SIZE
#define WC_CMAC_TAG_MIN_SZ (AES_BLOCK_SIZE/4)

#endif /* HAVE_FIPS */

WOLFSSL_API
int wc_InitCmac(Cmac* cmac,
                const byte* key, word32 keySz,
                int type, void* unused);
WOLFSSL_API
int wc_CmacUpdate(Cmac* cmac,
                  const byte* in, word32 inSz);
WOLFSSL_API
int wc_CmacFinal(Cmac* cmac,
                 byte* out, word32* outSz);

WOLFSSL_API
int wc_AesCmacGenerate(byte* out, word32* outSz,
                       const byte* in, word32 inSz,
                       const byte* key, word32 keySz);

WOLFSSL_API
int wc_AesCmacVerify(const byte* check, word32 checkSz,
                     const byte* in, word32 inSz,
                     const byte* key, word32 keySz);

#ifdef __cplusplus
    } /* extern "C" */
#endif


#endif /* NO_AES && WOLFSSL_CMAC */
#endif /* WOLF_CRYPT_CMAC_H */

