// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MANY_HPP
#define TAO_PEGTL_INTERNAL_MANY_HPP

#include <cstddef>

#include "dependent_false.hpp"
#include "enable_control.hpp"
#include "success.hpp"

#include "../config.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< unsigned Count, typename Peek >
   struct many
   {
      using rule_t = many;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( Count ) ) )
      {
         if constexpr( !Peek::allow_bulk ) {
            std::size_t done = 0;

            for( unsigned i = 0; i < Count; ++i ) {
               if( const auto t = Peek::peek( in, done ) ) {
                  done += t.size();
                  continue;
               }
               return false;
            }
            in.template consume< many >( done );
            return true;
         }
         else if constexpr( sizeof( *in.current() ) == 1 ) {
            static_assert( Peek::fixed_size > 0 );
            if( in.size( Count * Peek::fixed_size ) >= ( Count * Peek::fixed_size ) ) {
               in.template consume< many >( Count * Peek::fixed_size );
               return true;
            }
            return false;
         }
         else if constexpr( sizeof( *in.current() ) == Peek::fixed_size ) {
            static_assert( Peek::fixed_size > 0 );
            if( in.size( Count ) >= Count ) {
               in.template consume< many >( Count );
               return true;
            }
            return false;
         }
         else {
            static_assert( Peek::fixed_size > 0 );
            static_assert( dependent_false< Peek > );
         }
      }
   };

   template< typename Peek >
   struct many< 0, Peek >
      : success
   {};

   template< unsigned Count, typename Peek >
   inline constexpr bool enable_control< many< Count, Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
