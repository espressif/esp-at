/* misc.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



#ifndef WOLF_CRYPT_MISC_H
#define WOLF_CRYPT_MISC_H


#include <wolfssl/wolfcrypt/types.h>


#ifdef __cplusplus
    extern "C" {
#endif


#ifdef NO_INLINE
WOLFSSL_LOCAL
word32 rotlFixed(word32, word32);
WOLFSSL_LOCAL
word32 rotrFixed(word32, word32);

WOLFSSL_LOCAL
word32 ByteReverseWord32(word32);
WOLFSSL_LOCAL
void   ByteReverseWords(word32*, const word32*, word32);

WOLFSSL_LOCAL
void XorWords(wolfssl_word*, const wolfssl_word*, word32);
WOLFSSL_LOCAL
void xorbuf(void*, const void*, word32);

WOLFSSL_LOCAL
void ForceZero(const void*, word32);

WOLFSSL_LOCAL
int ConstantCompare(const byte*, const byte*, int);

#ifdef WORD64_AVAILABLE
WOLFSSL_LOCAL
word64 rotlFixed64(word64, word64);
WOLFSSL_LOCAL
word64 rotrFixed64(word64, word64);

WOLFSSL_LOCAL
word64 ByteReverseWord64(word64);
WOLFSSL_LOCAL
void   ByteReverseWords64(word64*, const word64*, word32);
#endif /* WORD64_AVAILABLE */

#ifndef WOLFSSL_HAVE_MIN
    #if defined(HAVE_FIPS) && !defined(min) /* so ifdef check passes */
        #define min min
    #endif
    WOLFSSL_LOCAL word32 min(word32 a, word32 b);
#endif

#ifndef WOLFSSL_HAVE_MAX
    #if defined(HAVE_FIPS) && !defined(max) /* so ifdef check passes */
        #define max max
    #endif
    WOLFSSL_LOCAL word32 max(word32 a, word32 b);
#endif /* WOLFSSL_HAVE_MAX */


void c32to24(word32 in, word24 out);
void c16toa(word16 u16, byte* c);
void c32toa(word32 u32, byte* c);
void c24to32(const word24 u24, word32* u32);
void ato16(const byte* c, word16* u16);
void ato24(const byte* c, word32* u24);
void ato32(const byte* c, word32* u32);
word32 btoi(byte b);


WOLFSSL_LOCAL byte ctMaskGT(int a, int b);
WOLFSSL_LOCAL byte ctMaskGTE(int a, int b);
WOLFSSL_LOCAL int  ctMaskIntGTE(int a, int b);
WOLFSSL_LOCAL byte ctMaskLT(int a, int b);
WOLFSSL_LOCAL byte ctMaskLTE(int a, int b);
WOLFSSL_LOCAL byte ctMaskEq(int a, int b);
WOLFSSL_LOCAL word16 ctMask16Eq(int a, int b);
WOLFSSL_LOCAL byte ctMaskNotEq(int a, int b);
WOLFSSL_LOCAL byte ctMaskSel(byte m, byte a, byte b);
WOLFSSL_LOCAL int  ctMaskSelInt(byte m, int a, int b);
WOLFSSL_LOCAL byte ctSetLTE(int a, int b);

#endif /* NO_INLINE */


#ifdef __cplusplus
    }   /* extern "C" */
#endif


#endif /* WOLF_CRYPT_MISC_H */

