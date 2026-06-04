// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEMANGLE_MSVC_HPP
#define TAO_PEGTL_INTERNAL_DEMANGLE_MSVC_HPP

#include <string_view>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      // We can not add static_assert for additional safety,
      // see issues #296, #301 and #308.
      constexpr std::string_view sv = __FUNCSIG__;
      constexpr auto begin = sv.find( "demangle<" );
      constexpr auto tmp = sv.substr( begin + 9 );
      constexpr auto end = tmp.rfind( '>' );
      return tmp.substr( 0, end );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
