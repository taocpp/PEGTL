// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_ANALYZE_TRAITS_HPP
#define TAO_PEGTL_BUFFER_ANALYZE_TRAITS_HPP

#include <cstddef>

#include "../analyze_traits.hpp"
#include "../config.hpp"

#include "discard.hpp"
#include "is_buffer.hpp"
#include "require.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Name >
   struct analyze_traits< Name, discard >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, is_buffer >
      : analyze_opt_traits<>
   {};

   template< typename Name, std::size_t Amount >
   struct analyze_traits< Name, require< Amount > >
      : analyze_opt_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
