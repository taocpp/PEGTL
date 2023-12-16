// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ANY_HPP
#define TAO_PEGTL_INTERNAL_ANY_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "peek_integer.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename = void >
   struct any
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = any;
      using subs_t = empty_list;

      [[nodiscard]] static bool test( const data_t /*unused*/ ) noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if( const auto t = Peek::peek( in ) ) {
            in.template consume< any >( t.size() );
            return true;
         }
         return false;
      }
   };

   template< typename Peek >
   struct any< Peek, std::enable_if_t< Peek::allow_bulk > >
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = any;
      using subs_t = empty_list;

      [[nodiscard]] static bool test( const char /*unused*/ ) noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         if( in.size( Peek::fixed_size ) >= Peek::fixed_size ) {
            in.template consume< any >( Peek::fixed_size );
            return true;
         }
         return false;
      }
   };

   template< typename Peek, typename Z >
   inline constexpr bool enable_control< any< Peek, Z > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
