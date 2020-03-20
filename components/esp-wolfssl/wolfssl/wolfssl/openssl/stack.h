/* stack.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/* stack.h for openSSL */

#ifndef WOLFSSL_STACK_H_
#define WOLFSSL_STACK_H_

#ifdef __cplusplus
    extern "C" {
#endif

typedef void (*wolfSSL_sk_freefunc)(void *);

WOLFSSL_API void wolfSSL_sk_GENERIC_pop_free(WOLFSSL_STACK* sk, wolfSSL_sk_freefunc);
WOLFSSL_API void wolfSSL_sk_GENERIC_free(WOLFSSL_STACK *);
WOLFSSL_API int wolfSSL_sk_GENERIC_push(WOLFSSL_STACK *sk, void *data);
WOLFSSL_API void wolfSSL_sk_pop_free(WOLFSSL_STACK *st, void (*func) (void *));
WOLFSSL_API void wolfSSL_sk_CONF_VALUE_free(WOLF_STACK_OF(WOLFSSL_CONF_VALUE)* sk);
WOLFSSL_API WOLFSSL_STACK *wolfSSL_sk_new_null(void);

WOLFSSL_API int wolfSSL_sk_CIPHER_push(WOLFSSL_STACK *st,WOLFSSL_CIPHER *cipher);
WOLFSSL_API WOLFSSL_CIPHER* wolfSSL_sk_CIPHER_pop(WOLF_STACK_OF(WOLFSSL_CIPHER)* sk);
WOLFSSL_API WOLFSSL_STACK* wolfSSL_sk_new_cipher(void);

#define OPENSSL_sk_free       wolfSSL_sk_free
#define OPENSSL_sk_pop_free   wolfSSL_sk_pop_free
#define OPENSSL_sk_new_null   wolfSSL_sk_new_null
#define OPENSSL_sk_push       wolfSSL_sk_push

/* provides older OpenSSL API compatibility  */
#define sk_free         OPENSSL_sk_free
#define sk_pop_free     OPENSSL_sk_pop_free
#define sk_new_null     OPENSSL_sk_new_null
#define sk_push         OPENSSL_sk_push

#ifdef  __cplusplus
}
#endif

#endif
