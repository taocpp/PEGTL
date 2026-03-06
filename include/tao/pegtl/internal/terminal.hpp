// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TERMINAL_HPP
#define TAO_PEGTL_INTERNAL_TERMINAL_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename >
   struct terminal;

   template< template< typename Peek, typename Peek::data_t... Cs > class Rule, typename Peek, typename Peek::data_t... Cs >
   struct terminal< Rule< Peek, Cs... > >
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = terminal;
      using subs_t = empty_list;

      using test_t = Rule< Peek, Cs... >;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Rule< Peek, Cs... >::test( t.data() ) ) {
               in.template consume< terminal >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Rule >
   inline constexpr bool enable_control< terminal< Rule > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
