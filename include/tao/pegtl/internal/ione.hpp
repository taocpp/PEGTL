// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_IONE_HPP
#define TAO_PEGTL_INTERNAL_IONE_HPP

#include "../config.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "failure.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct ione
      : single< ione< Peek, Cs... > >
   {
      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         return ( ascii_ichar_equal< Cs >( c ) || ... ) == true;
      }
   };

   template< typename Peek >
   struct ione< Peek >
      : failure
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< ione< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
