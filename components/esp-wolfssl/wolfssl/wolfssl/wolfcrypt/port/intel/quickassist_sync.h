/* quickassist_sync.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#ifndef _INTEL_QUICKASSIST_SYNC_H_
#define _INTEL_QUICKASSIST_SYNC_H_

#ifdef HAVE_INTEL_QA_SYNC

WOLFSSL_API int wc_CryptoCb_InitIntelQa(void);
WOLFSSL_API void wc_CryptoCb_CleanupIntelQa(int* id);

WOLFSSL_API void* wc_CryptoCb_IntelQaMalloc(size_t size,
    void* heap, int type
#ifdef WOLFSSL_DEBUG_MEMORY
    , const char* func, unsigned int line
#endif
);

WOLFSSL_API void wc_CryptoCb_IntelQaFree(void *ptr,
    void* heap, int type
#ifdef WOLFSSL_DEBUG_MEMORY
    , const char* func, unsigned int line
#endif
);

WOLFSSL_API void* wc_CryptoCb_IntelQaRealloc(void *ptr,
    size_t size, void* heap, int type
#ifdef WOLFSSL_DEBUG_MEMORY
    , const char* func, unsigned int line
#endif
);

#endif /* HAVE_INTEL_QA_SYNC */

#endif /* _INTEL_QUICKASSIST_SYNC_H_ */
