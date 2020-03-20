/* signature.h
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
    \file wolfssl/wolfcrypt/signature.h
*/


#ifndef WOLF_CRYPT_SIGNATURE_H
#define WOLF_CRYPT_SIGNATURE_H

#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/random.h>

#ifdef __cplusplus
    extern "C" {
#endif

enum wc_SignatureType {
    WC_SIGNATURE_TYPE_NONE = 0,
    WC_SIGNATURE_TYPE_ECC = 1,
    WC_SIGNATURE_TYPE_RSA = 2,
    WC_SIGNATURE_TYPE_RSA_W_ENC = 3, /* Adds DER header via wc_EncodeSignature */
};

WOLFSSL_API int wc_SignatureGetSize(enum wc_SignatureType sig_type,
    const void* key, word32 key_len);

WOLFSSL_API int wc_SignatureVerifyHash(
    enum wc_HashType hash_type, enum wc_SignatureType sig_type,
    const byte* hash_data, word32 hash_len,
    const byte* sig, word32 sig_len,
    const void* key, word32 key_len);

WOLFSSL_API int wc_SignatureVerify(
    enum wc_HashType hash_type, enum wc_SignatureType sig_type,
    const byte* data, word32 data_len,
    const byte* sig, word32 sig_len,
    const void* key, word32 key_len);

WOLFSSL_API int wc_SignatureGenerateHash(
    enum wc_HashType hash_type, enum wc_SignatureType sig_type,
    const byte* hash_data, word32 hash_len,
    byte* sig, word32 *sig_len,
    const void* key, word32 key_len, WC_RNG* rng);
WOLFSSL_API int wc_SignatureGenerateHash_ex(
    enum wc_HashType hash_type, enum wc_SignatureType sig_type,
    const byte* hash_data, word32 hash_len,
    byte* sig, word32 *sig_len,
    const void* key, word32 key_len, WC_RNG* rng, int verify);
WOLFSSL_API int wc_SignatureGenerate(
    enum wc_HashType hash_type, enum wc_SignatureType sig_type,
    const byte* data, word32 data_len,
    byte* sig, word32 *sig_len,
    const void* key, word32 key_len,
    WC_RNG* rng);
WOLFSSL_API int wc_SignatureGenerate_ex(
    enum wc_HashType hash_type, enum wc_SignatureType sig_type,
    const byte* data, word32 data_len,
    byte* sig, word32 *sig_len,
    const void* key, word32 key_len,
    WC_RNG* rng, int verify);

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* WOLF_CRYPT_SIGNATURE_H */
