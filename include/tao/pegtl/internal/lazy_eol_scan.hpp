// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_EOL_SCAN_HPP
#define TAO_PEGTL_INTERNAL_LAZY_EOL_SCAN_HPP

#include <cstddef>

#include "../config.hpp"

#include "lazy_scan_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Position, typename Data >
   void lazy_eol_scan( Position& pos, const Data* data, const std::size_t count )
   {
      lazy_scan_traits< typename Eol::rule_t >::scan( pos, data, count );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
