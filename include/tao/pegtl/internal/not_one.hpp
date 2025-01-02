// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NOT_ONE_HPP
#define TAO_PEGTL_INTERNAL_NOT_ONE_HPP

#include "../config.hpp"

#include "any.hpp"
#include "enable_control.hpp"
#include "tester.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct not_one
      : tester< not_one< Peek, Cs... > >
   {
      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         using data_t = typename Peek::data_t;
         static_assert( sizeof( Data ) <= sizeof( data_t ) );
         return !( ( data_t( c ) == Cs ) || ... );
      }
   };

   template< typename Peek >
   struct not_one< Peek >
      : any< Peek >
   {};

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< not_one< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
