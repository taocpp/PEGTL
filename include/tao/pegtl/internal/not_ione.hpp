// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NOT_IONE_HPP
#define TAO_PEGTL_INTERNAL_NOT_IONE_HPP

#include "../config.hpp"

#include "any.hpp"
#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct not_ione
      : single< not_ione< Peek, Cs... > >
   {
      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         return ( ascii_ichar_equal< Cs >( c ) || ... ) == false;
      }
   };

   template< typename Peek >
   struct not_ione< Peek >
      : any< Peek >
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< not_ione< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
