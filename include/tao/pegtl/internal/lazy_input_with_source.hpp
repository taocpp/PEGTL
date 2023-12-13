// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_LAZY_INPUT_WITH_SOURCE_HPP

#include "../config.hpp"

#include "input_with_source.hpp"
#include "lazy_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Source, typename Input >
   using lazy_input_with_source = input_with_source< Source, lazy_input< Eol, Input > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
