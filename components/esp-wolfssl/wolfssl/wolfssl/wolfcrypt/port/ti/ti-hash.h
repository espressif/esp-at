/* port/ti/ti-hash.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLF_CRYPT_TI_HASH_H
#define WOLF_CRYPT_TI_HASH_H

#include <wolfssl/wolfcrypt/types.h>

#ifndef WOLFSSL_TI_INITBUFF
    #define WOLFSSL_TI_INITBUFF    64
#endif

#ifndef WOLFSSL_MAX_HASH_SIZE
    #define WOLFSSL_MAX_HASH_SIZE  64
#endif

#define WOLFSSL_NO_HASH_RAW

typedef struct {
    byte   *msg;
    word32 used;
    word32 len;
    byte hash[WOLFSSL_MAX_HASH_SIZE];
} wolfssl_TI_Hash;


#ifndef TI_HASH_TEST

#if !defined(NO_MD5)
    typedef wolfssl_TI_Hash wc_Md5;
#endif
#if !defined(NO_SHA)
    typedef wolfssl_TI_Hash wc_Sha;
#endif
#if !defined(NO_SHA256)
    typedef wolfssl_TI_Hash wc_Sha256;
#endif
#if defined(WOLFSSL_SHA224)
    typedef wolfssl_TI_Hash wc_Sha224;
#endif

#endif /* !TI_HASH_TEST */

#endif /* WOLF_CRYPT_TI_HASH_H  */
