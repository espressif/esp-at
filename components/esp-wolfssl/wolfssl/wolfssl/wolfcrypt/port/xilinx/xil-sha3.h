/* xil-sha3.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLF_XIL_CRYPT_SHA3_H
#define WOLF_XIL_CRYPT_SHA3_H

#ifdef WOLFSSL_SHA3
#include "xsecure_sha.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Sha3 digest */
typedef struct Sha3 {
    XSecure_Sha3 hw;
    XCsuDma      dma;
} wc_Sha3;

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* WOLFSSL_SHA3 */
#endif /* WOLF_XIL_CRYPT_SHA3_H */

