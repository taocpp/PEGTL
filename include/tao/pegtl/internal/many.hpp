// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MANY_HPP
#define TAO_PEGTL_INTERNAL_MANY_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "any.hpp"
#include "enable_control.hpp"
#include "integer_size.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< unsigned Count, typename Peek >
   struct many
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = many;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( Count ) ) )
      {
         if constexpr( Peek::allow_bulk ) {
            static_assert( Peek::fixed_size > 0 );
            constexpr std::size_t s = integer_input_size< data_t, ParseInput >();
            if( in.size( Count * s ) >= ( Count * s ) ) {
               in.template consume< many >( Count * s );
               return true;
            }
            return false;
         }
         else {
            std::size_t total = 0;
            for( unsigned i = 0; i < Count; ++i ) {
               if( const auto t = Peek::peek( in, total ) ) {
                  total += t.size();
                  continue;
               }
               return false;
            }
            in.template consume< many >( total );
            return true;
         }
      }
   };

   template< typename Peek >
   struct many< 1, Peek >
      : any< Peek >
   {};

   template< typename Peek >
   struct many< 0, Peek >
      : success
   {};

   template< unsigned Count, typename Peek >
   inline constexpr bool enable_control< many< Count, Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
