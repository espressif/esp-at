/* err.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#ifndef WOLFSSL_OPENSSL_ERR_
#define WOLFSSL_OPENSSL_ERR_

#include <wolfssl/openssl/ssl.h>

/* err.h for openssl */
#define ERR_load_crypto_strings          wolfSSL_ERR_load_crypto_strings
#define ERR_peek_last_error              wolfSSL_ERR_peek_last_error

/* fatal error */
#define ERR_R_MALLOC_FAILURE                    MEMORY_E
#define ERR_R_PASSED_NULL_PARAMETER             BAD_FUNC_ARG
#define ERR_R_DISABLED                          NOT_COMPILED_IN
#define ERR_R_PASSED_INVALID_ARGUMENT           BAD_FUNC_ARG
#define RSA_R_UNKNOWN_PADDING_TYPE              RSA_PAD_E

/* SSL function codes */
#define RSA_F_RSA_OSSL_PRIVATE_ENCRYPT          1
#define SSL_F_SSL_CTX_USE_CERTIFICATE_FILE      2
#define SSL_F_SSL_USE_PRIVATEKEY                3

/* reasons */
#define ERR_R_SYS_LIB                           1
#define PKCS12_R_MAC_VERIFY_FAILURE             2

#define RSAerr(f,r)  ERR_put_error(0,(f),(r),__FILE__,__LINE__)
#define SSLerr(f,r)  ERR_put_error(0,(f),(r),__FILE__,__LINE__)

#endif /* WOLFSSL_OPENSSL_ERR_ */

