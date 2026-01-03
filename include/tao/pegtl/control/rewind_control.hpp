// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REWIND_CONTROL_HPP
#define TAO_PEGTL_CONTROL_REWIND_CONTROL_HPP

#include "../config.hpp"

#include "internal/rewind_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< template< typename... > class Control, typename Rule >
   using rewind_control_r = internal::rewind_control< Control, Rule >;

   template< template< typename... > class Control >
   struct rewind_control_n
   {
      template< typename Rule >
      using type = rewind_control_r< Control, Rule >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
