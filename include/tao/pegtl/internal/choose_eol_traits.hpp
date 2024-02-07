// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CHOOSE_EOL_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_CHOOSE_EOL_TRAITS_HPP

#include <cstddef>

#include "../config.hpp"

#include "has_eol_char.hpp"
#include "text_rule_traits.hpp"
#include "text_scan_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename Position, typename Data >
   void choose_eol_traits( Position& pos, const Data* data, const std::size_t count )
   {
      if constexpr( has_eol_char< Eol > ) {
         text_scan_traits< Eol::eol_char, typename Rule::rule_t >::scan( pos, data, count );
      }
      else {
         text_rule_traits< Eol, Rule >::scan( pos, data, count );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
