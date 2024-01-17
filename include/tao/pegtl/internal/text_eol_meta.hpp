// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_EOL_META_HPP
#define TAO_PEGTL_INTERNAL_TEXT_EOL_META_HPP

#include "../config.hpp"

#include "text_meta_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename Position, typename Data >
   void text_eol_meta( Position& pos, const Data* data, const std::size_t count )
   {
      text_meta_traits< typename Eol::eol_meta::rule_t, typename Rule::rule_t >::scan( pos, data, count );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
