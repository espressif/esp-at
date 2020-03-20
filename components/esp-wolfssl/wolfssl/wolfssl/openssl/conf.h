/* conf.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/* conf.h for openSSL */

#ifndef WOLFSSL_conf_H_
#define WOLFSSL_conf_H_

#ifdef __cplusplus
    extern "C" {
#endif

struct WOLFSSL_CONF_VALUE {
    char *section;
    char *name;
    char *value;
};

struct WOLFSSL_INIT_SETTINGS {
    char* appname;
};

typedef struct WOLFSSL_CONF_VALUE CONF_VALUE;
typedef struct WOLFSSL_INIT_SETTINGS OPENSSL_INIT_SETTINGS;

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* WOLFSSL_conf_H_ */
