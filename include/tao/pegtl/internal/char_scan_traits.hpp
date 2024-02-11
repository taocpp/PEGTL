// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHAR_SCAN_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_CHAR_SCAN_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

#include "scan_input.hpp"
#include "one.hpp"
#include "tester.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename One, typename = void >
   struct char_scan_traits;

   template< auto Eol, typename Peek >
   struct char_scan_traits< tester< one< Peek, Eol > > >
   {
      template< typename Position, typename Data >
      static void scan( Position& pos, scan_input< Data >& in )
      {
         using Char = decltype( Eol );

         static_assert( is_simple_type_v< Data > );

         pos.count += in.size();

         // if constexpr( std::is_same_v< Peek, peek_char > ) {
         if constexpr( ( sizeof( Char ) == 1 ) && ( sizeof( Data ) == 1 ) ) {
            while( !in.empty() ) {
               if( in.template peek_as< Char >() == Eol ) {
                  pos.line++;
                  pos.column = 1;
               }
               else {
                  pos.column++;
               }
               in.template consume< void >( 1 );
            }
         }
         else {
            while( !in.empty() ) {
               if( const auto pair = Peek::peek( in ) ) {
                  if( one< Peek, Eol >::test( pair.data() ) ) {
                     pos.line++;
                     pos.column = 1;
                  }
                  else {
                     pos.column += pair.size();
                  }
                  in.template consume< void >( pair.size() );
                  continue;
               }
               break;
            }
         }
      }
   };

   template< typename Rule >
   struct char_scan_traits< Rule, std::enable_if_t< !std::is_same_v< Rule, typename Rule::rule_t > > >
      : char_scan_traits< typename Rule::rule_t >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
