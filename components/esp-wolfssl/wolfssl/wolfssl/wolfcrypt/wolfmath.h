/* wolfmath.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#ifndef __WOLFMATH_H__
#define __WOLFMATH_H__

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef WOLFSSL_PUBLIC_MP
    #define MP_API   WOLFSSL_API
#else
    #define MP_API   WOLFSSL_LOCAL
#endif

#ifndef MIN
   #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef MAX
   #define MAX(x,y) ((x)>(y)?(x):(y))
#endif

/* timing resistance array */
#if !defined(WC_NO_CACHE_RESISTANT) && \
    ((defined(HAVE_ECC) && defined(ECC_TIMING_RESISTANT)) || \
     (defined(USE_FAST_MATH) && defined(TFM_TIMING_RESISTANT)))

    extern const wolfssl_word wc_off_on_addr[2];
#endif


/* common math functions */
MP_API int get_digit_count(mp_int* a);
MP_API mp_digit get_digit(mp_int* a, int n);
MP_API int get_rand_digit(WC_RNG* rng, mp_digit* d);

WOLFSSL_API int mp_rand(mp_int* a, int digits, WC_RNG* rng);

enum {
    /* format type */
    WC_TYPE_HEX_STR = 1,
    WC_TYPE_UNSIGNED_BIN = 2,
};

WOLFSSL_API int wc_export_int(mp_int* mp, byte* buf, word32* len,
    word32 keySz, int encType);

#ifdef HAVE_WOLF_BIGINT
    #if !defined(WOLF_BIGINT_DEFINED)
        /* raw big integer */
        typedef struct WC_BIGINT {
            byte*   buf;
            word32  len;
            void*   heap;
        } WC_BIGINT;
        #define WOLF_BIGINT_DEFINED
    #endif

    WOLFSSL_LOCAL void wc_bigint_init(WC_BIGINT* a);
    WOLFSSL_LOCAL int wc_bigint_alloc(WC_BIGINT* a, word32 sz);
    WOLFSSL_LOCAL int wc_bigint_from_unsigned_bin(WC_BIGINT* a, const byte* in, word32 inlen);
    WOLFSSL_LOCAL int wc_bigint_to_unsigned_bin(WC_BIGINT* a, byte* out, word32* outlen);
    WOLFSSL_LOCAL void wc_bigint_zero(WC_BIGINT* a);
    WOLFSSL_LOCAL void wc_bigint_free(WC_BIGINT* a);

    WOLFSSL_LOCAL int wc_mp_to_bigint(mp_int* src, WC_BIGINT* dst);
    WOLFSSL_LOCAL int wc_mp_to_bigint_sz(mp_int* src, WC_BIGINT* dst, word32 sz);
    WOLFSSL_LOCAL int wc_bigint_to_mp(WC_BIGINT* src, mp_int* dst);
#endif /* HAVE_WOLF_BIGINT */


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* __WOLFMATH_H__ */
