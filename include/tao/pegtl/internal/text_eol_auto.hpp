// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_EOL_AUTO_HPP
#define TAO_PEGTL_INTERNAL_TEXT_EOL_AUTO_HPP

#include "../config.hpp"

#include "has_eol_char.hpp"
#include "has_eol_list.hpp"
#include "has_eol_meta.hpp"
#include "text_eol_list.hpp"
#include "text_eol_meta.hpp"
#include "text_eol_rule.hpp"
#include "text_eol_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename Position, typename Data >
   void text_eol_auto( Position& pos, const Data* data, const std::size_t count )
   {
      if constexpr( has_eol_char< Eol > ) {
         text_eol_scan< Eol, Rule >( pos, data, count );
      }
      else if constexpr( has_eol_list< Eol > ) {
         text_eol_list< Eol, Rule >( pos, data, count );
      }
      else if constexpr( has_eol_meta< Eol > ) {
         text_eol_meta< Eol, Rule >( pos, data, count );
      }
      else {
         text_eol_rule< Eol, Rule >( pos, data, count );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
