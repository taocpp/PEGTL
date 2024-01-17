// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_META_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_META_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"
#include "../type_list.hpp"

#include "dependent_false.hpp"
#include "one.hpp"
#include "scan_utility.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename = void >
   struct text_meta_traits;

   template< typename Eol >
   struct text_meta_traits< Eol, void >
   {
      static_assert( dependent_false< Eol > );
   };

   template< typename Eol >
   struct text_meta_traits< Eol, eol_exclude_tag >
      : scan_columns_impl
   {};

   template< typename Eol >
   struct text_meta_traits< Eol, eol_matched_tag >
      : scan_nop_impl
   {};

   template< typename Eol >
   struct text_meta_traits< Eol, eol_unknown_tag >
      : scan_columns_impl
   {};

   template< typename Eol, typename Rule >
   struct text_meta_traits< Eol, Rule, std::enable_if_t< std::is_same_v< Eol, Rule > > >
      : scan_line_impl
   {};

   template< typename Peek, typename Peek::data_t L1, typename Peek::data_t L2, typename Peek::data_t... Ls, typename Peek::data_t R1 >
   struct text_meta_traits< single< one< Peek, L1, L2, Ls... > >, single< one< Peek, R1 > >, std::enable_if_t< one< Peek, L1, L2, Ls... >::test( R1 ) > >
      : scan_line_impl
   {};

   template< typename Peek, typename Peek::data_t L1, typename Rule >
   struct text_meta_traits< single< one< Peek, L1 > >, single< Rule >, std::enable_if_t< !std::is_same_v< single< one< Peek, L1 > >, single< Rule > > && std::is_same_v< Peek, typename single< Rule >::peek_t > && Rule::test( L1 ) > >
      : scan_line_impl
   {};

   template< typename Eol, typename Rule, typename >
   struct text_meta_traits
      : scan_columns_impl
   {
      static_assert( std::is_same_v< Eol, typename Eol::rule_t > );
      static_assert( std::is_same_v< Rule, typename Rule::rule_t > );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
