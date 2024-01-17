// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_RULE_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_RULE_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"

#include "dependent_false.hpp"
#include "scan_utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename = void >
   struct text_rule_traits;

   template< typename Eol >
   struct text_rule_traits< Eol, void >
   {
      static_assert( dependent_false< Eol > );
   };

   template< typename Eol >
   struct text_rule_traits< Eol, eol_exclude_tag >
      : scan_columns_impl
   {};

   template< typename Eol >
   struct text_rule_traits< Eol, eol_matched_tag >
      : scan_line_impl
   {};

   template< typename Eol >
   struct text_rule_traits< Eol, eol_unknown_tag >
      : scan_columns_impl
   {};

   template< typename Eol, typename Rule, typename >
   struct text_rule_traits
      : scan_columns_impl
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
