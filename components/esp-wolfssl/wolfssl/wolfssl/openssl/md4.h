/* md4.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_MD4_H_
#define WOLFSSL_MD4_H_

#include <wolfssl/wolfcrypt/settings.h>

#ifndef NO_MD4

#ifdef WOLFSSL_PREFIX
#include "prefix_md4.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif


typedef struct WOLFSSL_MD4_CTX {
    int buffer[32];      /* big enough to hold, check size in Init */
} WOLFSSL_MD4_CTX;


WOLFSSL_API void wolfSSL_MD4_Init(WOLFSSL_MD4_CTX*);
WOLFSSL_API void wolfSSL_MD4_Update(WOLFSSL_MD4_CTX*, const void*, unsigned long);
WOLFSSL_API void wolfSSL_MD4_Final(unsigned char*, WOLFSSL_MD4_CTX*);


typedef WOLFSSL_MD4_CTX MD4_CTX;

#define MD4_Init   wolfSSL_MD4_Init
#define MD4_Update wolfSSL_MD4_Update
#define MD4_Final  wolfSSL_MD4_Final

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* NO_MD4 */

#endif /* WOLFSSL_MD4_H_ */

