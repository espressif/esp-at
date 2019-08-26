/* nrf51.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_NRF51_PORT_H
#define WOLFSSL_NRF51_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wolfssl/wolfcrypt/types.h>

/* Public Functions */
int nrf51_random_generate(byte* output, word32 sz);

int nrf51_aes_set_key(const byte* key);
int nrf51_aes_encrypt(const byte* in, const byte* key, word32 rounds, byte* out);

double current_time(int reset);

#ifdef __cplusplus
}
#endif

#endif /* WOLFSSL_NRF51_PORT_H */
