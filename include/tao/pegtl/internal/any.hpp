// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ANY_HPP
#define TAO_PEGTL_INTERNAL_ANY_HPP

#include <cstddef>

#include "../config.hpp"
#include "../tags.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

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
         return true;  // TODO: Is it a problem that this is only true for values that Peek can return?
      }

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if constexpr( Peek::template bulk< ParseInput >() ) {
            constexpr std::size_t s = Peek::template size< ParseInput >();
            static_assert( s > 0 );
            if( in.size( s ) >= s ) {
               in.template consume< eol_unknown_tag >( s );
               return true;
            }
            return false;
         }
         else {
            if( const auto t = Peek::peek( in ) ) {
               in.template consume< eol_unknown_tag >( t.size() );
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
