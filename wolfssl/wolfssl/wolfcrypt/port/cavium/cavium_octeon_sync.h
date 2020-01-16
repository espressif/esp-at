/* cavium_octeon_sync.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#ifndef _CAVIUM_OCTEON_SYNC_H_
#define _CAVIUM_OCTEON_SYNC_H_

#ifdef HAVE_CAVIUM_OCTEON_SYNC

WOLFSSL_API int wc_CryptoCb_InitOcteon(void);
WOLFSSL_API void wc_CryptoCb_CleanupOcteon(int* id);

#endif /* HAVE_CAVIUM_OCTEON_SYNC */
#endif /* _CAVIUM_OCTEON_SYNC_H_ */
