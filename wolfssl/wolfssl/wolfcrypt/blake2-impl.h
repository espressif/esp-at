/*
   BLAKE2 reference source code package - reference C implementations

   Written in 2012 by Samuel Neves <sneves@dei.uc.pt>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along with
   this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
/* blake2-impl.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */



#ifndef WOLFCRYPT_BLAKE2_IMPL_H
#define WOLFCRYPT_BLAKE2_IMPL_H

#include <wolfssl/wolfcrypt/types.h>

static WC_INLINE word32 load32( const void *src )
{
#if defined(LITTLE_ENDIAN_ORDER)
  return *( word32 * )( src );
#else
  const byte *p = ( byte * )src;
  word32 w = *p++;
  w |= ( word32 )( *p++ ) <<  8;
  w |= ( word32 )( *p++ ) << 16;
  w |= ( word32 )( *p++ ) << 24;
  return w;
#endif
}

static WC_INLINE word64 load64( const void *src )
{
#if defined(LITTLE_ENDIAN_ORDER)
  return *( word64 * )( src );
#else
  const byte *p = ( byte * )src;
  word64 w = *p++;
  w |= ( word64 )( *p++ ) <<  8;
  w |= ( word64 )( *p++ ) << 16;
  w |= ( word64 )( *p++ ) << 24;
  w |= ( word64 )( *p++ ) << 32;
  w |= ( word64 )( *p++ ) << 40;
  w |= ( word64 )( *p++ ) << 48;
  w |= ( word64 )( *p++ ) << 56;
  return w;
#endif
}

static WC_INLINE void store32( void *dst, word32 w )
{
#if defined(LITTLE_ENDIAN_ORDER)
  *( word32 * )( dst ) = w;
#else
  byte *p = ( byte * )dst;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w;
#endif
}

static WC_INLINE void store64( void *dst, word64 w )
{
#if defined(LITTLE_ENDIAN_ORDER)
  *( word64 * )( dst ) = w;
#else
  byte *p = ( byte * )dst;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w;
#endif
}

static WC_INLINE word64 load48( const void *src )
{
  const byte *p = ( const byte * )src;
  word64 w = *p++;
  w |= ( word64 )( *p++ ) <<  8;
  w |= ( word64 )( *p++ ) << 16;
  w |= ( word64 )( *p++ ) << 24;
  w |= ( word64 )( *p++ ) << 32;
  w |= ( word64 )( *p++ ) << 40;
  return w;
}

static WC_INLINE void store48( void *dst, word64 w )
{
  byte *p = ( byte * )dst;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w; w >>= 8;
  *p++ = ( byte )w;
}

static WC_INLINE word32 rotl32( const word32 w, const unsigned c )
{
  return ( w << c ) | ( w >> ( 32 - c ) );
}

static WC_INLINE word64 rotl64( const word64 w, const unsigned c )
{
  return ( w << c ) | ( w >> ( 64 - c ) );
}

static WC_INLINE word32 rotr32( const word32 w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 32 - c ) );
}

static WC_INLINE word64 rotr64( const word64 w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
static WC_INLINE void secure_zero_memory( void *v, word64 n )
{
  volatile byte *p = ( volatile byte * )v;

  while( n-- ) *p++ = 0;
}

#endif  /* WOLFCRYPT_BLAKE2_IMPL_H */

