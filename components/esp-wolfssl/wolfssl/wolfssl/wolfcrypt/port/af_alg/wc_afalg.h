/* wc_afalg.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLFSSL_AFALG_H
#define WOLFSSL_AFALG_H

#include <wolfssl/wolfcrypt/types.h>

#include <sys/socket.h>
#include <linux/if_alg.h>
#include <linux/socket.h>

#define WC_SOCK_NOTSET -1

/* In some cases these flags are not set in AF_ALG header files.
 * Documentation provided at kernel.org/doc/html/v4.16/crypto/userspace-if.html
 * suggests using these values if not set */
#ifndef AF_ALG
    #define AF_ALG 38
#endif
#ifndef SOL_ALG
    #define SOL_ALG 279
#endif

WOLFSSL_LOCAL void wc_Afalg_SockAddr(struct sockaddr_alg* in, const char* type, const char* name);
WOLFSSL_LOCAL int wc_Afalg_Accept(struct sockaddr_alg* in, int inSz, int sock);
WOLFSSL_LOCAL int wc_Afalg_Socket(void);
WOLFSSL_LOCAL int wc_Afalg_CreateRead(int sock, const char* type, const char* name);
WOLFSSL_LOCAL int wc_Afalg_SetIv(struct cmsghdr* cmsg, byte* iv, word32 ivSz);
WOLFSSL_LOCAL int wc_Afalg_SetOp(struct cmsghdr* cmsg, int dir);
WOLFSSL_LOCAL int wc_Afalg_SetAad(struct cmsghdr* cmsg, word32 sz);

#endif /* WOLFSSL_AFALG_H */

