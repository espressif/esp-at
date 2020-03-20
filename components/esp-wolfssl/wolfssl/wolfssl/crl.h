/* crl.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



#ifndef WOLFSSL_CRL_H
#define WOLFSSL_CRL_H


#ifdef HAVE_CRL

#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/asn.h>

#ifdef __cplusplus
    extern "C" {
#endif

WOLFSSL_LOCAL int  InitCRL(WOLFSSL_CRL*, WOLFSSL_CERT_MANAGER*);
WOLFSSL_LOCAL void FreeCRL(WOLFSSL_CRL*, int dynamic);

WOLFSSL_LOCAL int  LoadCRL(WOLFSSL_CRL* crl, const char* path, int type, int mon);
WOLFSSL_LOCAL int  BufferLoadCRL(WOLFSSL_CRL*, const byte*, long, int, int);
WOLFSSL_LOCAL int  CheckCertCRL(WOLFSSL_CRL*, DecodedCert*);


#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* HAVE_CRL */
#endif /* WOLFSSL_CRL_H */
