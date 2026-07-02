// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_THROW_OR_TERMINATE_HPP
#define TAO_PEGTL_ACTION_INTERNAL_THROW_OR_TERMINATE_HPP

#include "../../config.hpp"

#if defined( __cpp_exceptions )

#include "../../parse_error.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   [[noreturn]] void throw_or_terminate( const char* msg, const Input& in )
   {
      throw parse_error( msg, in.current_position() );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#else

#include <cstdio>
#include <exception>

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   [[noreturn]] void throw_or_terminate( const char* msg, const Input& /*unused*/ )
   {
      std::fprintf( stderr, "%s\n", msg );
      std::terminate();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
