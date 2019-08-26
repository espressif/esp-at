/* wolfcaam.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#ifndef WOLF_CRYPT_CAAM_INIT_H
#define WOLF_CRYPT_CAAM_INIT_H

#include <wolfssl/wolfcrypt/settings.h>

#if defined(WOLFSSL_IMX6_CAAM) || defined(WOLFSSL_IMX6_CAAM_RNG)

#include <wolfssl/wolfcrypt/types.h>

#if defined(__INTEGRITY) || defined(INTEGRITY)
    #include <INTEGRITY.h>
#endif

WOLFSSL_LOCAL int wc_caamInit(void);
WOLFSSL_LOCAL int wc_caamFree(void);
WOLFSSL_LOCAL int wc_caamInitRng(void);
WOLFSSL_LOCAL int wc_caamFreeRng(void);

WOLFSSL_LOCAL word32 wc_caamReadRegister(word32 reg);
WOLFSSL_LOCAL void wc_caamWriteRegister(word32 reg, word32 value);
WOLFSSL_LOCAL int  wc_caamAddAndWait(Buffer* buf, word32 arg[4], word32 type);

WOLFSSL_API int wc_caamSetResource(IODevice ioDev);

WOLFSSL_API int wc_caamOpenBlob(byte* data, word32 dataSz, byte* out,
	word32* outSz);
WOLFSSL_API int wc_caamCreateBlob(byte* data, word32 dataSz, byte* out,
	word32* outSz);

/* additional size that is added by CAAM when creating a blob */
#define WC_CAAM_BLOB_SZ 48

#ifndef WC_CAAM_READ
    #define WC_CAAM_READ(reg)      wc_caamReadRegister((reg))
#endif
#ifndef WC_CAAM_WRITE
    #define WC_CAAM_WRITE(reg, x)  wc_caamWriteRegister((reg), (x))
#endif

#endif /* WOLFSSL_IMX6_CAAM */

#endif /* WOLF_CRYPT_CAAM_INIT_H */
