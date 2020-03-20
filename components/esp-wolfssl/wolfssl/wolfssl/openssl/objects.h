/* objects.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_OBJECTS_H_
#define WOLFSSL_OBJECTS_H_

#include <wolfssl/wolfcrypt/settings.h>
//#include <wolfssl/openssl/ssl.h>
#ifndef OPENSSL_EXTRA_SSL_GUARD
#define OPENSSL_EXTRA_SSL_GUARD
#include <wolfssl/ssl.h>
#endif /* OPENSSL_EXTRA_SSL_GUARD */

#ifdef __cplusplus
    extern "C" {
#endif

#define OBJ_nid2sn  wolfSSL_OBJ_nid2sn
#define OBJ_obj2nid wolfSSL_OBJ_obj2nid
#define OBJ_sn2nid  wolfSSL_OBJ_sn2nid
#define OBJ_nid2ln  wolfSSL_OBJ_nid2ln
#define OBJ_txt2nid wolfSSL_OBJ_txt2nid
#define OBJ_txt2obj wolfSSL_OBJ_txt2obj
#define OBJ_nid2obj wolfSSL_OBJ_nid2obj
#define OBJ_obj2txt wolfSSL_OBJ_obj2txt
#define OBJ_cleanup wolfSSL_OBJ_cleanup
#define OBJ_cmp     wolfSSL_OBJ_cmp
#define OBJ_create  wolfSSL_OBJ_create
#define ASN1_OBJECT_free wolfSSL_ASN1_OBJECT_free

/* not required for wolfSSL */
#define OPENSSL_load_builtin_modules()


#define NID_ad_OCSP                     178
#define NID_ad_ca_issuers               179


#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif /* WOLFSSL_OBJECTS_H_ */
