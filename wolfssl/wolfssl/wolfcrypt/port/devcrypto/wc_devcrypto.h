/* wc_devcrypto.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_DEVCRYPTO_H
#define WOLFSSL_DEVCRYPTO_H

#include <wolfssl/wolfcrypt/types.h>

#ifdef WOLFSSL_DEVCRYPTO

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <crypto/cryptodev.h>

typedef struct WC_CRYPTODEV {
    int cfd;
    struct session_op sess;
} WC_CRYPTODEV;

WOLFSSL_LOCAL int wc_DevCryptoCreate(WC_CRYPTODEV* ctx, int type, byte* key, word32 keySz);
WOLFSSL_LOCAL void wc_DevCryptoFree(WC_CRYPTODEV* ctx);
WOLFSSL_LOCAL void wc_SetupCrypt(struct crypt_op* crt, WC_CRYPTODEV* dev,
        byte* src, int srcSz, byte* dst, byte* dig, int flag);
WOLFSSL_LOCAL void wc_SetupCryptSym(struct crypt_op* crt, WC_CRYPTODEV* dev,
        byte* src, word32 srcSz, byte* dst, byte* iv, int flag);
WOLFSSL_LOCAL void wc_SetupCryptAead(struct crypt_auth_op* crt, WC_CRYPTODEV* dev,
         byte* src, word32 srcSz, byte* dst, byte* iv, word32 ivSz, int flag,
         byte* authIn, word32 authInSz, byte* authTag, word32 authTagSz);

#endif /* WOLFSSL_DEVCRYPTO */
#endif /* WOLFSSL_DEVCRYPTO_H */


