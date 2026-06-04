// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEMANGLE_RTTI_HPP
#define TAO_PEGTL_INTERNAL_DEMANGLE_RTTI_HPP

#if !defined( __cpp_rtti )
#error "RTTI support required for fallback demangle function.
#else

#include <string_view>
#include <typeinfo>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      // Fallback: RTTI without demangling.
      return typeid( T ).name();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif

#endif
