/* ge_operations.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


 /* Based On Daniel J Bernstein's ed25519 Public Domain ref10 work. */

#ifndef WOLF_CRYPT_GE_OPERATIONS_H
#define WOLF_CRYPT_GE_OPERATIONS_H

#include <wolfssl/wolfcrypt/settings.h>

#ifdef HAVE_ED25519

#include <wolfssl/wolfcrypt/fe_operations.h>

/*
ge means group element.

Here the group is the set of pairs (x,y) of field elements (see fe.h)
satisfying -x^2 + y^2 = 1 + d x^2y^2
where d = -121665/121666.

Representations:
  ge_p2 (projective): (X:Y:Z) satisfying x=X/Z, y=Y/Z
  ge_p3 (extended): (X:Y:Z:T) satisfying x=X/Z, y=Y/Z, XY=ZT
  ge_p1p1 (completed): ((X:Z),(Y:T)) satisfying x=X/Z, y=Y/T
  ge_precomp (Duif): (y+x,y-x,2dxy)
*/

#ifdef ED25519_SMALL
  typedef byte     ge[F25519_SIZE];
#elif defined(CURVED25519_ASM_64BIT)
  typedef int64_t  ge[4];
#elif defined(CURVED25519_ASM_32BIT)
  typedef int32_t  ge[8];
#elif defined(CURVED25519_128BIT)
  typedef int64_t  ge[5];
#else
  typedef int32_t  ge[10];
#endif

typedef struct {
  ge X;
  ge Y;
  ge Z;
} ge_p2;

typedef struct {
  ge X;
  ge Y;
  ge Z;
  ge T;
} ge_p3;


WOLFSSL_LOCAL int  ge_compress_key(byte* out, const byte* xIn, const byte* yIn,
                                                                word32 keySz);
WOLFSSL_LOCAL int  ge_frombytes_negate_vartime(ge_p3 *,const unsigned char *);

WOLFSSL_LOCAL int  ge_double_scalarmult_vartime(ge_p2 *,const unsigned char *,
                                         const ge_p3 *,const unsigned char *);
WOLFSSL_LOCAL void ge_scalarmult_base(ge_p3 *,const unsigned char *);
WOLFSSL_LOCAL void sc_reduce(byte* s);
WOLFSSL_LOCAL void sc_muladd(byte* s, const byte* a, const byte* b,
                             const byte* c);
WOLFSSL_LOCAL void ge_tobytes(unsigned char *,const ge_p2 *);
WOLFSSL_LOCAL void ge_p3_tobytes(unsigned char *,const ge_p3 *);


#ifndef ED25519_SMALL
typedef struct {
  ge X;
  ge Y;
  ge Z;
  ge T;
} ge_p1p1;

typedef struct {
  ge yplusx;
  ge yminusx;
  ge xy2d;
} ge_precomp;

typedef struct {
  ge YplusX;
  ge YminusX;
  ge Z;
  ge T2d;
} ge_cached;

#endif /* !ED25519_SMALL */

#endif /* HAVE_ED25519 */

#endif /* WOLF_CRYPT_GE_OPERATIONS_H */
