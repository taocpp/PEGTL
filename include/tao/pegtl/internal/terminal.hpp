// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TERMINAL_HPP
#define TAO_PEGTL_INTERNAL_TERMINAL_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "invert_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< invert_mode, typename >
   struct terminal;

   template< invert_mode I, template< typename Peek, typename Peek::data_t... Cs > class Impl, typename Peek, typename Peek::data_t... Cs >
   struct terminal< I, Impl< Peek, Cs... > >
   {
      using rule_t = terminal;
      using subs_t = empty_list;

      template< typename Data >
      [[nodiscard]] static constexpr auto test( const Data c ) noexcept
      {
         return Impl< Peek, Cs... >::test( c ) != bool( I );
      }

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( test( t.data() ) ) {
               in.template consume< terminal >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< invert_mode I, typename Impl >
   inline constexpr bool enable_control< terminal< I, Impl > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
