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
#include "dependent_false.hpp"
#include "eol.hpp"
#include "eolf.hpp"
#include "scan_utility.hpp"
#include "tester.hpp"
#include "until.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char Eol, typename Rule, typename = void >
   struct text_scan_traits;

   template< char Eol >
   struct text_scan_traits< Eol, void >
   {
      static_assert( char_dependent_false< Eol > );
   };

   template< char Eol >
   struct text_scan_traits< Eol, eol_exclude_tag >
      : scan_skip_base
   {};

   template< char Eol >
   struct text_scan_traits< Eol, eol_matched_tag >
      : scan_nop_base
   {};

   template< char Eol >
   struct text_scan_traits< Eol, eol_unknown_tag >
      : scan_eol_base< Eol >
   {};

   template< char Eol, typename Peek >
   struct text_scan_traits< Eol, tester< one< Peek, Eol > > >
      : scan_line_base
   {};

   template< char Eol, typename Rule >
   struct text_scan_traits< Eol, tester< Rule >, std::enable_if_t< !Rule::test( typename Rule::data_t( Eol ) ) > >
      : scan_skip_base
   {};

   template< char Eol, char... Cs >
   struct text_scan_traits< Eol, ascii_string< Cs... >, std::enable_if_t< ( ( Eol != Cs ) && ... ) > >
      : scan_skip_base
   {};

   template< char Eol, char... Cs >
   struct text_scan_traits< Eol, ascii_istring< Cs... >, std::enable_if_t< ( ( Eol != Cs ) && ... ) > >
      : scan_skip_base
   {};

   template< char Eol, typename Cond >
   struct text_scan_traits< Eol, until< Cond >, std::enable_if_t< !std::is_same_v< Cond, typename Cond::rule_t > > >
      : text_scan_traits< Eol, until< typename Cond::rule_t > >
   {};

   template< char Eol >
   struct text_scan_traits< Eol, until< eol< void > > >
      : scan_skip_base
   {};

   template< char Eol >
   struct text_scan_traits< Eol, until< eolf< void > > >
      : scan_skip_base
   {};

   template< char Eol, typename Cond >
   struct text_scan_traits< Eol, until< at< Cond > > >
      : text_scan_traits< Eol, until< typename Cond::rule_t > >
   {};

   template< char Eol, typename Peek >
   struct text_scan_traits< Eol, until< tester< one< Peek, Eol > > > >
      : scan_skip_base
   {};

   template< char Eol, typename Rule, typename >
   struct text_scan_traits
      : scan_eol_base< Eol >
   {
      static_assert( std::is_same_v< Rule, typename Rule::rule_t > );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
