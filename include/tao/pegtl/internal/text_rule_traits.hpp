// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_RULE_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_RULE_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"

#include "dependent_false.hpp"

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
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         pos.column += count;
         pos.count += count;
      }
   };

   template< typename Eol >
   struct text_rule_traits< Eol, eol_matched_tag >
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         ++pos.line;
         pos.column = 1;
         pos.count += count;
      }
   };

   template< typename Eol >
   struct text_rule_traits< Eol, eol_unknown_tag >
      : text_rule_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol, typename Rule, typename >
   struct text_rule_traits
      : text_rule_traits< Eol, eol_exclude_tag >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
