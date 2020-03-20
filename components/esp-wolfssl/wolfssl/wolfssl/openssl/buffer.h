/* buffer.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_BUFFER_H_
#define WOLFSSL_BUFFER_H_

#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/openssl/ssl.h>

#ifdef __cplusplus
    extern "C" {
#endif


WOLFSSL_API WOLFSSL_BUF_MEM* wolfSSL_BUF_MEM_new(void);
WOLFSSL_API int wolfSSL_BUF_MEM_grow(WOLFSSL_BUF_MEM* buf, size_t len);
WOLFSSL_API void wolfSSL_BUF_MEM_free(WOLFSSL_BUF_MEM* buf);


#define BUF_MEM_new  wolfSSL_BUF_MEM_new
#define BUF_MEM_grow wolfSSL_BUF_MEM_grow
#define BUF_MEM_free wolfSSL_BUF_MEM_free

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* WOLFSSL_BUFFER_H_ */
