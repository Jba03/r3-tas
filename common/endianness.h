//
//  endianness.h
//  r3lib
//
//  Created by Jba03 on 2021-05-20.
//  Copyright © 2021 Jba03. All rights reserved.
//

#ifndef endianness_h
#define endianness_h

#if __has_include(<SDL2/SDL_endian.h>)
#
#   include <SDL2/SDL_endian.h>
#   if SDL_BYTEORDER == SDL_LIL_ENDIAN
#       define LSB_FIRST
#   elif SDL_BYTEORDER == SDL_BIG_ENDIAN
#       define MSB_FIRST
#   else
#       error invalid byte order
#   endif
#
#else
#
#   if defined(__i386__)       \
    || defined(__i486__)       \
    || defined(__i586__)       \
    || defined(__i686__)       \
    || defined(__x86_64__)     \
    || defined(__alpha__)      \
    || defined(__MIPSEL__)     \
    || defined(_M_IX86)        \
    || defined(_M_X64)         \
    || defined(_XBOX1)         \
    || defined(__arm__)        \
    || defined(ANDROID)        \
    || defined(__aarch64__)    \
    || (defined(__BYTE_ORDER__) && __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__)
#       define LSB_FIRST
#   else
#       define MSB_FIRST
#   endif
#
#endif

#endif /* endianness_h */
