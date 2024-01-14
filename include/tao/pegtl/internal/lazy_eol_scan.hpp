// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_EOL_SCAN_HPP
#define TAO_PEGTL_INTERNAL_LAZY_EOL_SCAN_HPP

#include "../config.hpp"

#include "lazy_scan_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Position, typename Data >
   void lazy_eol_scan( Position& pos, const Data* data, const Data* dend )
   {
      using eol_rule = typename Eol::rule_t;
      lazy_scan_traits< eol_rule >::scan( pos, data, dend );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
