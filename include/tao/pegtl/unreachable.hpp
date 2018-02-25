// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_UNREACHABLE_HPP
#define TAO_PEGTL_UNREACHABLE_HPP

#ifndef TAO_PEGTL_UNREACHABLE
#if defined( _MSC_VER ) && !defined( __ICC )
#define TAO_PEGTL_UNREACHABLE \
   do {                       \
      __assume( false );      \
   } while( false )
#else
#define TAO_PEGTL_UNREACHABLE  \
   do {                        \
      __builtin_unreachable(); \
   } while( false )
#endif
#endif

#endif
