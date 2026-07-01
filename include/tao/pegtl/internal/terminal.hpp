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
   template< typename Rule, typename Peek >
   struct terminal
   {
      using rule_t = Rule;
      using subs_t = empty_list;
      using peek_t = Peek;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Rule::test( t.data() ) ) {
               in.template consume< Rule >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Rule, typename Peek >
   inline constexpr bool enable_control< terminal< Rule, Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
