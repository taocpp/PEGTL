// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_IGNORE_ARGUMENTS_HPP
#define TAO_PEGTL_INTERNAL_IGNORE_ARGUMENTS_HPP

#include "../config.hpp"

#if defined( _MSC_VER )
#  define TAO_PEGTL_MSVC_IGNORE( ... )  TAO_PEGTL_NAMESPACE::internal::ignore_arguments( __VA_ARGS__ )
#else
#  define TAO_PEGTL_MSVC_IGNORE( ... )  while( false )
#endif

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Ts >
   void ignore_arguments( Ts&&... /*unused*/ )
   {}

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
