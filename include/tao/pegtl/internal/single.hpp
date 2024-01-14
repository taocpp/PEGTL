// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SINGLE_HPP
#define TAO_PEGTL_INTERNAL_SINGLE_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename >
   struct single;

   template< template< typename Peek, typename Peek::data_t... Cs > class Rule, typename Peek, typename Peek::data_t... Cs >
   struct single< Rule< Peek, Cs... > >
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = single;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Rule< Peek, Cs... >::test( t.data() ) ) {
               in.template consume< single >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Rule >
   inline constexpr bool enable_control< single< Rule > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
