// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_EOL_BUMP_HPP
#define TAO_PEGTL_INTERNAL_TEXT_EOL_BUMP_HPP

#include "../config.hpp"

#include "bump_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename Data, typename Position >
   void text_eol_bump( Position& pos, const Data* data, const std::size_t count )
   {
      using eol_rule = typename Eol::rule_t;
      using rule_rule = typename Rule::rule_t;
      bump_traits< eol_rule, rule_rule >::bump( pos, data, count );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
