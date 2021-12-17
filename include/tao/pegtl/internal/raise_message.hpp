// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RAISE_MESSAGE_HPP
#define TAO_PEGTL_INTERNAL_RAISE_MESSAGE_HPP

#if !defined( __cpp_exceptions )
#error "Exception support required for tao/pegtl/internal/raise.hpp"
#else

#include "../config.hpp"

#include "enable_control.hpp"
#include "raise.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char... Cs >
   struct raise_message
      : raise< raise_message< Cs... > >
   {
      static constexpr const char error_message[] = { Cs..., 0 };
   };

   template< char... Cs >
   inline constexpr bool enable_control< raise_message< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
