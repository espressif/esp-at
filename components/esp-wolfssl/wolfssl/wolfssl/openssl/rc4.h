/* rc4.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



/*  rc4.h defines mini des openssl compatibility layer
 *
 */

#ifndef WOLFSSL_RC4_COMPAT_H_
#define WOLFSSL_RC4_COMPAT_H_

#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/openssl/ssl.h> /* included for size_t */

#ifdef __cplusplus
    extern "C" {
#endif

/* applications including wolfssl/openssl/rc4.h are expecting to have access to
 * the size of RC4_KEY structures. */
typedef struct WOLFSSL_RC4_KEY {
    /* big enough for Arc4 from wolfssl/wolfcrypt/arc4.h */
    void* holder[(272 + WC_ASYNC_DEV_SIZE) / sizeof(void*)];
} WOLFSSL_RC4_KEY;
typedef WOLFSSL_RC4_KEY RC4_KEY;

WOLFSSL_API void wolfSSL_RC4_set_key(WOLFSSL_RC4_KEY* key, int len,
        const unsigned char* data);
WOLFSSL_API void wolfSSL_RC4(WOLFSSL_RC4_KEY* key, size_t len,
        const unsigned char* in, unsigned char* out);

#define RC4         wolfSSL_RC4
#define RC4_set_key wolfSSL_RC4_set_key

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* WOLFSSL_RC4_COMPAT_H_ */

