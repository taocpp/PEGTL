// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ANY_HPP
#define TAO_PEGTL_INTERNAL_ANY_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "integer_size.hpp"
#include "peek_integer.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek >
   struct any
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = any;
      using subs_t = empty_list;

      [[nodiscard]] static constexpr bool test( const data_t /*unused*/ ) noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if constexpr( Peek::allow_bulk ) {
            static_assert( Peek::fixed_size > 0 );
            constexpr std::size_t s = integer_input_size< data_t, ParseInput >();
            if( in.size( s ) >= s ) {
               in.template consume< any >( s );
               return true;
            }
            return false;
         }
         else {
            if( const auto t = Peek::peek( in ) ) {
               in.template consume< any >( t.size() );
               return true;
            }
            return false;
         }
      }
   };

   template< typename Peek >
   inline constexpr bool enable_control< any< Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
