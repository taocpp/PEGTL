// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SCAN_INPUT_HPP
#define TAO_PEGTL_INTERNAL_SCAN_INPUT_HPP

#include "../config.hpp"

#include "input_with_fakes.hpp"
#include "input_with_peeks.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   using scan_input = input_with_peeks< input_with_fakes< view_input< Data > > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
