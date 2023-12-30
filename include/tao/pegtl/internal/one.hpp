// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ONE_HPP
#define TAO_PEGTL_INTERNAL_ONE_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "failure.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct one
      : single< one< Peek, Cs... > >
   {
      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         return ( ( c == Cs ) || ... ) == true;
      }
   };

   template< typename Peek >
   struct one< Peek >
      : failure
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< one< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
