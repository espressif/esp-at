/* tls1.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_OPENSSL_TLS1_H_
#define WOLFSSL_OPENSSL_TLS1_H_

#ifndef TLS1_VERSION
#define TLS1_VERSION                    0x0301
#endif

#ifndef TLS1_1_VERSION
#define TLS1_1_VERSION                  0x0302
#endif

#ifndef TLS1_2_VERSION
#define TLS1_2_VERSION                  0x0303
#endif

#ifndef TLS1_3_VERSION
#define TLS1_3_VERSION                  0x0304
#endif

#ifndef TLS_MAX_VERSION
#define TLS_MAX_VERSION                 TLS1_3_VERSION
#endif

#endif /* WOLFSSL_OPENSSL_TLS1_H_ */
