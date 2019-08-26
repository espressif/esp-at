/* cpuid.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



#ifndef WOLF_CRYPT_CPUID_H
#define WOLF_CRYPT_CPUID_H


#include <wolfssl/wolfcrypt/types.h>


#ifdef __cplusplus
    extern "C" {
#endif

#if defined(WOLFSSL_X86_64_BUILD) || defined(USE_INTEL_SPEEDUP) || \
    defined(WOLFSSL_AESNI)
    #define CPUID_AVX1   0x0001
    #define CPUID_AVX2   0x0002
    #define CPUID_RDRAND 0x0004
    #define CPUID_RDSEED 0x0008
    #define CPUID_BMI2   0x0010   /* MULX, RORX */
    #define CPUID_AESNI  0x0020
    #define CPUID_ADX    0x0040   /* ADCX, ADOX */

    #define IS_INTEL_AVX1(f)    ((f) & CPUID_AVX1)
    #define IS_INTEL_AVX2(f)    ((f) & CPUID_AVX2)
    #define IS_INTEL_RDRAND(f)  ((f) & CPUID_RDRAND)
    #define IS_INTEL_RDSEED(f)  ((f) & CPUID_RDSEED)
    #define IS_INTEL_BMI2(f)    ((f) & CPUID_BMI2)
    #define IS_INTEL_AESNI(f)   ((f) & CPUID_AESNI)
    #define IS_INTEL_ADX(f)     ((f) & CPUID_ADX)

    void cpuid_set_flags(void);
    word32 cpuid_get_flags(void);
#endif

#ifdef __cplusplus
    }   /* extern "C" */
#endif


#endif /* WOLF_CRYPT_CPUID_H */
