/* opensslv.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/* opensslv.h compatibility */

#ifndef WOLFSSL_OPENSSLV_H_
#define WOLFSSL_OPENSSLV_H_


/* api version compatibility */
#if defined(WOLFSSL_APACHE_HTTPD)
    /* For Apache httpd, Use 1.1.0 compatibility */
     #define OPENSSL_VERSION_NUMBER 0x10100000L
#elif defined(OPENSSL_ALL) || defined(HAVE_STUNNEL) || defined(HAVE_LIGHTY) || \
    defined(WOLFSSL_NGINX) || defined(WOLFSSL_HAPROXY)
     /* version number can be increased for Lighty after compatibility for ECDH
        is added */
     #define OPENSSL_VERSION_NUMBER 0x10001000L
#else
     #define OPENSSL_VERSION_NUMBER 0x0090810fL
#endif

#define OPENSSL_VERSION_TEXT             LIBWOLFSSL_VERSION_STRING
#define OPENSSL_VERSION                  0

#endif /* header */
