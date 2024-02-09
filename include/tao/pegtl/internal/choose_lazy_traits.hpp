// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHOOSE_LAZY_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_CHOOSE_LAZY_TRAITS_HPP

#include <cstddef>

#include "../config.hpp"

#include "dependent_false.hpp"
#include "get_eol_rule_char.hpp"
#include "has_eol_lazy_peek.hpp"
#include "has_eol_char_rule.hpp"
#include "lazy_scan_traits.hpp"
#include "char_scan_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Position, typename Data >
   void choose_lazy_traits( Position& pos, const Data* data, const std::size_t count )
   {
      static_assert( !std::is_same_v< Eol, void > );
      static_assert( !std::is_same_v< Eol, typename Eol::rule_t > );

      if constexpr( get_eol_rule_char_v< Eol > ) {
         char_scan_traits< Eol >::scan( pos, data, count );
      }
      else if constexpr( has_eol_char_rule< Eol > ) {
         char_scan_traits< typename Eol::eol_char_rule >::scan( pos, data, count );
      }
      else if constexpr( has_eol_lazy_peek< Eol > ) {
         lazy_scan_traits< typename Eol::rule_t, typename Eol::eol_lazy_peek >::scan( pos, data, count );
      }
      else {
         static_assert( dependent_false< Eol, Position, Data > );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
