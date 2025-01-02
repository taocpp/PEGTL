// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHOOSE_TEXT_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_CHOOSE_TEXT_TRAITS_HPP

#include <type_traits>

#include "../config.hpp"

#include "scan_input.hpp"
#include "text_rule_traits.hpp"
#include "text_scan_traits.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename Position, typename Data >
   void choose_text_traits( Position& pos, scan_input< Data >& in )
   {
      static_assert( !std::is_same_v< Eol, void > );

      if constexpr( has_eol_char_rule< Eol > ) {
         text_scan_traits< Eol, typename Rule::rule_t >::scan( pos, in );
      }
      else {
         text_rule_traits< Eol, typename Rule::rule_t >::scan( pos, in );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
