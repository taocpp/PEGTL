// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ENDIAN_SYSTEM_HPP
#define TAO_PEGTL_INTERNAL_ENDIAN_SYSTEM_HPP

#if defined( _WIN32 )

#if !defined( __ORDER_BIG_ENDIAN__ )
#define __ORDER_BIG_ENDIAN__ 4321
#endif

#if !defined( __ORDER_LITTLE_ENDIAN__ )
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif

#if !defined( __BYTE__ORDER__ )
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

#define TAO_PEGTL_BIG_ENDIAN_TYPE byteswap_endian
#define TAO_PEGTL_LITTLE_ENDIAN_TYPE identity_endian

#define TAO_PEGTL_ENDIAN_SUFFIXED( iDeNTiFieR ) iDeNTiFieR##le

// NOTE: I'm pretty sure that all Windows versions ever are little
// endian, including on processors like Alpha, Mips and Itanium.

#elif !defined( __BYTE_ORDER__ )

#error No byte order defined even though we are not on Windows!

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define TAO_PEGTL_BIG_ENDIAN_TYPE identity_endian
#define TAO_PEGTL_LITTLE_ENDIAN_TYPE byteswap_endian

#define TAO_PEGTL_ENDIAN_SUFFIXED( iDeNTiFieR ) iDeNTiFieR##be

#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define TAO_PEGTL_BIG_ENDIAN_TYPE byteswap_endian
#define TAO_PEGTL_LITTLE_ENDIAN_TYPE identity_endian

#define TAO_PEGTL_ENDIAN_SUFFIXED( iDeNTiFieR ) iDeNTiFieR##le

#else

#error Byte order is neither big nor little endian and we do NOT support the PDP-11!

#endif

#endif
