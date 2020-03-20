/* des.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



/*  des.h defines mini des openssl compatibility layer
 *
 */


#ifndef WOLFSSL_DES_H_
#define WOLFSSL_DES_H_

#include <wolfssl/wolfcrypt/settings.h>

#ifndef NO_DES3

#ifdef WOLFSSL_PREFIX
#include "prefix_des.h"
#endif


#ifdef __cplusplus
    extern "C" {
#endif

typedef unsigned char WOLFSSL_DES_cblock[8];
typedef /* const */ WOLFSSL_DES_cblock WOLFSSL_const_DES_cblock;
typedef WOLFSSL_DES_cblock WOLFSSL_DES_key_schedule;
typedef unsigned int WOLFSSL_DES_LONG;


enum {
    DES_ENCRYPT = 1,
    DES_DECRYPT = 0
};


WOLFSSL_API int wolfSSL_DES_is_weak_key(WOLFSSL_const_DES_cblock* key);
WOLFSSL_API WOLFSSL_DES_LONG wolfSSL_DES_cbc_cksum(const unsigned char* in,
            WOLFSSL_DES_cblock* out, long length, WOLFSSL_DES_key_schedule* sc,
            WOLFSSL_const_DES_cblock* iv);
WOLFSSL_API int wolfSSL_DES_set_key(WOLFSSL_const_DES_cblock* myDes,
                                               WOLFSSL_DES_key_schedule* key);
WOLFSSL_API int wolfSSL_DES_set_key_checked(WOLFSSL_const_DES_cblock* myDes,
                                               WOLFSSL_DES_key_schedule* key);
WOLFSSL_API void wolfSSL_DES_set_key_unchecked(WOLFSSL_const_DES_cblock*,
                                             WOLFSSL_DES_key_schedule*);
WOLFSSL_API int  wolfSSL_DES_key_sched(WOLFSSL_const_DES_cblock* key,
                                     WOLFSSL_DES_key_schedule* schedule);
WOLFSSL_API void wolfSSL_DES_cbc_encrypt(const unsigned char* input,
                     unsigned char* output, long length,
                     WOLFSSL_DES_key_schedule* schedule, WOLFSSL_DES_cblock* ivec,
                     int enc);
WOLFSSL_API void wolfSSL_DES_ede3_cbc_encrypt(const unsigned char* input,
                                      unsigned char* output, long sz,
                                      WOLFSSL_DES_key_schedule* ks1,
                                      WOLFSSL_DES_key_schedule* ks2,
                                      WOLFSSL_DES_key_schedule* ks3,
                                      WOLFSSL_DES_cblock* ivec, int enc);
WOLFSSL_API void wolfSSL_DES_ncbc_encrypt(const unsigned char* input,
                      unsigned char* output, long length,
                      WOLFSSL_DES_key_schedule* schedule,
                      WOLFSSL_DES_cblock* ivec, int enc);

WOLFSSL_API void wolfSSL_DES_set_odd_parity(WOLFSSL_DES_cblock*);
WOLFSSL_API void wolfSSL_DES_ecb_encrypt(WOLFSSL_DES_cblock*, WOLFSSL_DES_cblock*,
                                       WOLFSSL_DES_key_schedule*, int);
WOLFSSL_API int wolfSSL_DES_check_key_parity(WOLFSSL_DES_cblock*);


typedef WOLFSSL_DES_cblock DES_cblock;
typedef WOLFSSL_const_DES_cblock const_DES_cblock;
typedef WOLFSSL_DES_key_schedule DES_key_schedule;
typedef WOLFSSL_DES_LONG DES_LONG;

#define DES_check_key(x) /* Define WOLFSSL_CHECK_DESKEY to check key */
#define DES_is_weak_key       wolfSSL_DES_is_weak_key
#define DES_set_key           wolfSSL_DES_set_key
#define DES_set_key_checked   wolfSSL_DES_set_key_checked
#define DES_set_key_unchecked wolfSSL_DES_set_key_unchecked
#define DES_key_sched         wolfSSL_DES_key_sched
#define DES_cbc_encrypt       wolfSSL_DES_cbc_encrypt
#define DES_ncbc_encrypt      wolfSSL_DES_ncbc_encrypt
#define DES_set_odd_parity    wolfSSL_DES_set_odd_parity
#define DES_ecb_encrypt       wolfSSL_DES_ecb_encrypt
#define DES_ede3_cbc_encrypt  wolfSSL_DES_ede3_cbc_encrypt
#define DES_cbc_cksum         wolfSSL_DES_cbc_cksum
#define DES_check_key_parity  wolfSSL_DES_check_key_parity

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* NO_DES3 */

#endif /* WOLFSSL_DES_H_ */
