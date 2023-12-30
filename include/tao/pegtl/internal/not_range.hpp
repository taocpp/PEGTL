// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NOT_RANGE_HPP
#define TAO_PEGTL_INTERNAL_NOT_RANGE_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "not_one.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct not_range
      : single< not_range< Peek, Lo, Hi > >
   {
      static_assert( Lo < Hi );

      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         return ( ( Lo <= c ) && ( c <= Hi ) ) == false;
      }
   };

   template< typename Peek, typename Peek::data_t C >
   struct not_range< Peek, C, C >
      : not_one< Peek, C >
   {};

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   inline constexpr bool enable_control< not_range< Peek, Lo, Hi > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif