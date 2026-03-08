// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ONE_IMPL_HPP
#define TAO_PEGTL_INTERNAL_ONE_IMPL_HPP

#include "../config.hpp"

#include "dependent_false.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct one_impl
   {
      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         using data_t = typename Peek::data_t;
         static_assert( sizeof( Data ) <= sizeof( data_t ) );
         return ( ( data_t( c ) == Cs ) || ... );
      }
   };

   template< typename Peek >
   struct one_impl< Peek >
   {
      static_assert( dependent_false< Peek > );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
