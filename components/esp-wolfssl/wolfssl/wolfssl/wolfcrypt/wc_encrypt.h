/* wc_encrypt.h
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
    \file wolfssl/wolfcrypt/wc_encrypt.h
*/


#ifndef WOLF_CRYPT_ENCRYPT_H
#define WOLF_CRYPT_ENCRYPT_H

#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/chacha.h>
#include <wolfssl/wolfcrypt/des3.h>
#include <wolfssl/wolfcrypt/arc4.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* determine max cipher key size */
#ifndef NO_AES
    #define WC_MAX_SYM_KEY_SIZE     (AES_MAX_KEY_SIZE/8)
#elif defined(HAVE_CHACHA)
    #define WC_MAX_SYM_KEY_SIZE     CHACHA_MAX_KEY_SZ
#elif !defined(NO_DES3)
    #define WC_MAX_SYM_KEY_SIZE     DES3_KEY_SIZE
#elif !defined(NO_RC4)
    #define WC_MAX_SYM_KEY_SIZE     RC4_KEY_SIZE
#else
    #define WC_MAX_SYM_KEY_SIZE     32
#endif


#if !defined(NO_AES) && defined(HAVE_AES_CBC)
WOLFSSL_API int wc_AesCbcEncryptWithKey(byte* out, const byte* in, word32 inSz,
                                        const byte* key, word32 keySz,
                                        const byte* iv);
WOLFSSL_API int wc_AesCbcDecryptWithKey(byte* out, const byte* in, word32 inSz,
                                        const byte* key, word32 keySz,
                                        const byte* iv);
#endif /* !NO_AES */


#ifndef NO_DES3
WOLFSSL_API int wc_Des_CbcDecryptWithKey(byte* out,
                                         const byte* in, word32 sz,
                                         const byte* key, const byte* iv);
WOLFSSL_API int wc_Des_CbcEncryptWithKey(byte* out,
                                         const byte* in, word32 sz,
                                         const byte* key, const byte* iv);
WOLFSSL_API int wc_Des3_CbcEncryptWithKey(byte* out,
                                          const byte* in, word32 sz,
                                          const byte* key, const byte* iv);
WOLFSSL_API int wc_Des3_CbcDecryptWithKey(byte* out,
                                          const byte* in, word32 sz,
                                          const byte* key, const byte* iv);
#endif /* !NO_DES3 */




#ifdef WOLFSSL_ENCRYPTED_KEYS
    struct EncryptedInfo;
    WOLFSSL_API int wc_BufferKeyDecrypt(struct EncryptedInfo* info, byte* der, word32 derSz,
        const byte* password, int passwordSz, int hashType);
    WOLFSSL_API int wc_BufferKeyEncrypt(struct EncryptedInfo* info, byte* der, word32 derSz,
        const byte* password, int passwordSz, int hashType);
#endif /* WOLFSSL_ENCRYPTED_KEYS */

#ifndef NO_PWDBASED
    WOLFSSL_LOCAL int wc_CryptKey(const char* password, int passwordSz, 
        byte* salt, int saltSz, int iterations, int id, byte* input, int length,
        int version, byte* cbcIv, int enc);
#endif

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* WOLF_CRYPT_ENCRYPT_H */

