// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_RULE_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_RULE_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"

#include "scan_base_classes.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule >
   struct text_rule_traits
      : add_column_scan
   {};

   template< typename Eol >
   struct text_rule_traits< Eol, eol_exclude_tag >
      : add_column_scan
   {};

   template< typename Eol >
   struct text_rule_traits< Eol, eol_matched_tag >
      : inc_line_scan
   {};

   template< typename Eol >
   struct text_rule_traits< Eol, eol_unknown_tag >
      : add_column_scan
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
