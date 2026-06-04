// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEMANGLE_CLANG_HPP
#define TAO_PEGTL_INTERNAL_DEMANGLE_CLANG_HPP

#if ( ( __GNUC__ == 9 ) && ( __GNUC_MINOR__ < 3 ) ) || ( __GNUC__ == 16 )

// GCC 9.1 and 9.2 can truncate __PRETTY_FUNCTION__.
// The bug was fixed in 9.3 but is back again in 16.1.
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91155

#include "demangle_rtti.hpp"

#else

#include <string_view>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::special
{
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      constexpr auto begin = sv.find( '=' );
      static_assert( begin != std::string_view::npos );
      constexpr auto tmp = sv.substr( begin + 2 );
      constexpr auto end = tmp.rfind( ';' );
      static_assert( end != std::string_view::npos );
      return tmp.substr( 0, end );
   }

}  // namespace TAO_PEGTL_NAMESPACE::special

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      return special::demangle< T >();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif

#endif
