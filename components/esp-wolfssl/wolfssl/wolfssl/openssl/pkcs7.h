/* pkcs7.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/* pkcs7.h for openSSL */


#ifndef WOLFSSL_PKCS7_H_
#define WOLFSSL_PKCS7_H_

#include <wolfssl/openssl/ssl.h>
#include <wolfssl/wolfcrypt/pkcs7.h>

#ifdef __cplusplus
    extern "C" {
#endif

#if defined(OPENSSL_ALL) && defined(HAVE_PKCS7)

#define PKCS7_NOINTERN         0x0010
#define PKCS7_NOVERIFY         0x0020


typedef struct WOLFSSL_PKCS7
{
    PKCS7 pkcs7;
    unsigned char* data;
    int len;
} WOLFSSL_PKCS7;


WOLFSSL_API PKCS7* wolfSSL_PKCS7_new(void);
WOLFSSL_API PKCS7_SIGNED* wolfSSL_PKCS7_SIGNED_new(void);
WOLFSSL_API void wolfSSL_PKCS7_free(PKCS7* p7);
WOLFSSL_API void wolfSSL_PKCS7_SIGNED_free(PKCS7_SIGNED* p7);
WOLFSSL_API PKCS7* wolfSSL_d2i_PKCS7(PKCS7** p7, const unsigned char** in,
    int len);
WOLFSSL_API PKCS7* wolfSSL_d2i_PKCS7_bio(WOLFSSL_BIO* bio, PKCS7** p7);
WOLFSSL_API int wolfSSL_PKCS7_verify(PKCS7* p7, WOLFSSL_STACK* certs,
    WOLFSSL_X509_STORE* store, WOLFSSL_BIO* in, WOLFSSL_BIO* out, int flags);
WOLFSSL_API WOLFSSL_STACK* wolfSSL_PKCS7_get0_signers(PKCS7* p7,
    WOLFSSL_STACK* certs, int flags);
WOLFSSL_API int wolfSSL_PEM_write_bio_PKCS7(WOLFSSL_BIO* bio, PKCS7* p7);

#define PKCS7_new                      wolfSSL_PKCS7_new
#define PKCS7_SIGNED_new               wolfSSL_PKCS7_SIGNED_new
#define PKCS7_free                     wolfSSL_PKCS7_free
#define PKCS7_SIGNED_free              wolfSSL_PKCS7_SIGNED_free
#define d2i_PKCS7                      wolfSSL_d2i_PKCS7
#define d2i_PKCS7_bio                  wolfSSL_d2i_PKCS7_bio
#define PKCS7_verify                   wolfSSL_PKCS7_verify
#define PKCS7_get0_signers             wolfSSL_PKCS7_get0_signers
#define PEM_write_bio_PKCS7            wolfSSL_PEM_write_bio_PKCS7

#endif /* OPENSSL_ALL && HAVE_PKCS7 */

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* WOLFSSL_PKCS7_H_ */

