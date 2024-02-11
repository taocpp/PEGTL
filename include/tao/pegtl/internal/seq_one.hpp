// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SEQ_ONE_HPP
#define TAO_PEGTL_INTERNAL_SEQ_ONE_HPP

#include "../config.hpp"

#include "one.hpp"
#include "seq.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   using seq_one = seq< one< Peek, Cs >... >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
