// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_MUST_IF_HPP
#define TAO_PEGTL_CONTROL_MUST_IF_HPP

#include <type_traits>

#if !defined( __cpp_exceptions )
#error "Exception support required for tao/pegtl/control/must_if.hpp"
#else

#include "../config.hpp"
#include "../normal.hpp"

#include "internal/must_if.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // must_if_b is not implemented

   template< typename Errors, bool RequireMessage, template< typename... > class Control, typename Rule >
   using must_if_r = internal::must_if< Errors, RequireMessage, Control, Rule >;

   template< typename Errors, bool RequireMessage = true, template< typename... > class Control = normal >
   struct must_if_n
   {
      template< typename Rule >
      using type = internal::must_if< Errors, RequireMessage, Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
#endif
