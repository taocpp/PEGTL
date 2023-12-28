// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SINGLE_HPP
#define TAO_PEGTL_INTERNAL_SINGLE_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Test >
   struct single
   {
      using test_t = Test;

      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = single;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Test::test( t.data() ) ) {
               in.template consume< single >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek, typename Test >
   inline constexpr bool enable_control< single< Peek, Test > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
