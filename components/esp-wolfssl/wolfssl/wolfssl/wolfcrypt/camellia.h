/* camellia.h ver 1.2.0
 *
 * Copyright (c) 2006,2007
 * NTT (Nippon Telegraph and Telephone Corporation) . All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer as
 *   the first lines of this file unmodified.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NTT ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NTT BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* camellia.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/*!
    \file wolfssl/wolfcrypt/camellia.h
*/


#ifndef WOLF_CRYPT_CAMELLIA_H
#define WOLF_CRYPT_CAMELLIA_H

#include <wolfssl/wolfcrypt/types.h>

#ifdef HAVE_CAMELLIA

#ifdef __cplusplus
    extern "C" {
#endif

enum {
    CAMELLIA_BLOCK_SIZE = 16
};

#define CAMELLIA_TABLE_BYTE_LEN 272
#define CAMELLIA_TABLE_WORD_LEN (CAMELLIA_TABLE_BYTE_LEN / sizeof(word32))

typedef word32 KEY_TABLE_TYPE[CAMELLIA_TABLE_WORD_LEN];

typedef struct Camellia {
    word32 keySz;
    KEY_TABLE_TYPE key;
    word32 reg[CAMELLIA_BLOCK_SIZE / sizeof(word32)]; /* for CBC mode */
    word32 tmp[CAMELLIA_BLOCK_SIZE / sizeof(word32)]; /* for CBC mode */
} Camellia;


WOLFSSL_API int  wc_CamelliaSetKey(Camellia* cam,
                                   const byte* key, word32 len, const byte* iv);
WOLFSSL_API int  wc_CamelliaSetIV(Camellia* cam, const byte* iv);
WOLFSSL_API int  wc_CamelliaEncryptDirect(Camellia* cam, byte* out,
                                                                const byte* in);
WOLFSSL_API int  wc_CamelliaDecryptDirect(Camellia* cam, byte* out,
                                                                const byte* in);
WOLFSSL_API int wc_CamelliaCbcEncrypt(Camellia* cam,
                                          byte* out, const byte* in, word32 sz);
WOLFSSL_API int wc_CamelliaCbcDecrypt(Camellia* cam,
                                          byte* out, const byte* in, word32 sz);


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* HAVE_CAMELLIA */
#endif /* WOLF_CRYPT_CAMELLIA_H */

