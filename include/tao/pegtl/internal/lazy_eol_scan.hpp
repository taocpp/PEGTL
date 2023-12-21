// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_EOL_SCAN_HPP
#define TAO_PEGTL_INTERNAL_LAZY_EOL_SCAN_HPP

#include "../config.hpp"

#include "lazy_scan_input.hpp"
#include "lazy_scan_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Data, typename Position >
   void lazy_eol_scan( Position& pos, const Data* data, const Data* dend )
   {
      lazy_scan_input< Data > in( data, dend );
      lazy_scan_traits< Eol >::scan( in, pos );
      pos.count += dend - data;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
