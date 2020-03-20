/* afalg_hash.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLF_CRYPT_AFALG_HASH_H
#define WOLF_CRYPT_AFALG_HASH_H

#include <wolfssl/wolfcrypt/types.h>

#undef  WOLFSSL_NO_HASH_RAW
#define WOLFSSL_NO_HASH_RAW

typedef struct {
    byte*  msg;
    void*  heap;
    word32 used;
    word32 len;
    int    alFd;
    int    rdFd;
} wolfssl_AFALG_Hash;



#if !defined(NO_SHA256) && defined(WOLFSSL_AFALG_HASH)
    typedef wolfssl_AFALG_Hash wc_Sha256;
#endif
#if defined(WOLFSSL_SHA3) && defined(WOLFSSL_AFALG_XILINX_SHA3)
    typedef wolfssl_AFALG_Hash wc_Sha3;
#endif

#endif /* WOLF_CRYPT_AFALG_HASH_H  */

