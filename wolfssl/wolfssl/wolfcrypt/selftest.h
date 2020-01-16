/* selftest.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



#ifndef WOLFCRYPT_SELF_TEST_H
#define WOLFCRYPT_SELF_TEST_H

#include <wolfssl/wolfcrypt/types.h>


#ifdef __cplusplus
    extern "C" {
#endif

#ifdef HAVE_SELFTEST
    /* Get wolfCrypt CAVP version */
    WOLFSSL_API const char* wolfCrypt_GetVersion_CAVP_selftest(void);

    /* wolfCrypt self test, runs CAVP KATs */
    WOLFSSL_API int wolfCrypt_SelfTest(void);
#endif

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* WOLFCRYPT_SELF_TEST_H */


