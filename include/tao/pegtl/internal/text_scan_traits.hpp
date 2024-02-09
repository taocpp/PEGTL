// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_SCAN_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_SCAN_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"

#include "ascii_string.hpp"
#include "at.hpp"
#include "char_scan_traits.hpp"
#include "eol.hpp"
#include "eolf.hpp"
#include "get_eol_rule_char.hpp"
#include "scan_base_classes.hpp"
#include "tester.hpp"
#include "until.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename = void >
   struct text_scan_traits;

   template< typename Eol >
   struct text_scan_traits< Eol, eol_exclude_tag >
      : add_column_scan
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, eol_matched_tag >
      : nop_scan
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, eol_unknown_tag >
      : char_scan_traits< typename Eol::eol_char_rule >
   {};

   // template< typename Eol >
   // struct text_scan_traits< Eol, typename Eol::eol_char_rule::rule_t >
   //    : inc_line_scan
   // {};

   template< typename Eol, typename Rule >
   struct text_scan_traits< Eol, tester< Rule > >
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* data, const std::size_t count ) noexcept
      {
         if constexpr( std::is_same_v< typename Eol::eol_char_rule::rule_t, tester< Rule > > ) {
            inc_line_scan::scan( pos, data, count );
         }
         else if constexpr( Rule::test( get_eol_rule_char_v< typename Eol::eol_char_rule > ) ) {
            char_scan_traits< typename Eol::eol_char_rule >::scan( pos, data, count );
         }
         else {
            add_column_scan::scan( pos, data, count );
         }
      }
   };

   template< typename Eol, char... Cs >
   struct text_scan_traits< Eol, ascii_string< Cs... >, std::enable_if_t< ( ( get_eol_rule_char_v< typename Eol::eol_char_rule > != Cs ) && ... ) > >
      : add_column_scan
   {};

   template< typename Eol, char... Cs >
   struct text_scan_traits< Eol, ascii_istring< Cs... >, std::enable_if_t< ( ( get_eol_rule_char_v< typename Eol::eol_char_rule > != Cs ) && ... ) > >
      : add_column_scan
   {};

   template< typename Eol, typename Cond >
   struct text_scan_traits< Eol, until< Cond >, std::enable_if_t< !std::is_same_v< Cond, typename Cond::rule_t > > >
      : text_scan_traits< Eol, until< typename Cond::rule_t > >
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, until< eol< void > > >
      : add_column_scan
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, until< eolf< void > > >
      : add_column_scan
   {};

   template< typename Eol, typename Cond >
   struct text_scan_traits< Eol, until< at< Cond > > >
      : text_scan_traits< Eol, until< typename Cond::rule_t > >
   {};

   template< typename Eol, typename Peek >
   struct text_scan_traits< Eol, until< tester< one< Peek, get_eol_rule_char_v< typename Eol::eol_char_rule > > > > >
      : add_column_scan
   {};

   template< typename Eol, typename Rule, typename >
   struct text_scan_traits
      : char_scan_traits< typename Eol::eol_char_rule >
   {
      static_assert( std::is_same_v< Rule, typename Rule::rule_t > );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
