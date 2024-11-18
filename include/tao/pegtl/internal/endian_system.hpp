// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ENDIAN_SYSTEM_HPP
#define TAO_PEGTL_INTERNAL_ENDIAN_SYSTEM_HPP

#include "../config.hpp"

#if defined( TAO_PEGTL_SYSTEM_WINDOWS )
#  if !defined( __ORDER_BIG_ENDIAN__ )
#    define __ORDER_BIG_ENDIAN__ 4321
#  endif
#  if !defined( __ORDER_LITTLE_ENDIAN__ )
#    define __ORDER_LITTLE_ENDIAN__ 1234
#  endif
#  if !defined( __BYTE_ORDER__ )
#    define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#  endif

// I'm pretty sure that all Windows versions ever released
// are little endian, including on Alpha, Mips and Itanium.

#  define TAO_PEGTL_BIG_ENDIAN_TYPE byteswap_endian
#  define TAO_PEGTL_LITTLE_ENDIAN_TYPE identity_endian
#  define TAO_PEGTL_ENDIAN_SUFFIXED( iDeNTiFieR ) iDeNTiFieR##le

#elif !defined( __BYTE_ORDER__ )
#  error "No byte order defined even though we are not on Windows!"
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#  define TAO_PEGTL_BIG_ENDIAN_TYPE identity_endian
#  define TAO_PEGTL_LITTLE_ENDIAN_TYPE byteswap_endian
#  define TAO_PEGTL_ENDIAN_SUFFIXED( iDeNTiFieR ) iDeNTiFieR##be

#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#  define TAO_PEGTL_BIG_ENDIAN_TYPE byteswap_endian
#  define TAO_PEGTL_LITTLE_ENDIAN_TYPE identity_endian
#  define TAO_PEGTL_ENDIAN_SUFFIXED( iDeNTiFieR ) iDeNTiFieR##le

#else
#  error "Byte order is neither big nor little endian -- are you using a PDP-11?"
#endif

#endif
