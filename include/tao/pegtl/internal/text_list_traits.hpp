// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_LIST_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_LIST_TRAITS_HPP

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
   template< typename Eols, typename Rule, typename = void >
   struct text_list_traits;

   template< typename Eols >
   struct text_list_traits< Eols, void >
   {
      static_assert( dependent_false< Eols > );
   };

   template< typename Eols >
   struct text_list_traits< Eols, eol_exclude_tag >
      : scan_columns_impl
   {};

   template< typename Eols >
   struct text_list_traits< Eols, eol_matched_tag >
      : scan_nop_impl
   {};

   template< typename Eols >
   struct text_list_traits< Eols, eol_unknown_tag >
      : scan_columns_impl
   {};

   template< typename... Rules, typename Rule >
   struct text_list_traits< type_list< Rules... >, Rule, std::enable_if_t< type_list_contains_v< Rule, typename Rules::rule_t... > > >
      : scan_line_impl
   {};

   template< typename Eols, typename Rule, typename >
   struct text_list_traits
      : scan_columns_impl
   {
      static_assert( std::is_same_v< Rule, typename Rule::rule_t > );
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
