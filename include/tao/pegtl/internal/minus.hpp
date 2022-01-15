// Copyright (c) 2020-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MINUS_HPP
#define TAO_PEGTL_INTERNAL_MINUS_HPP

#include "eof.hpp"
#include "not_at.hpp"
#include "rematch.hpp"
#include "seq.hpp"

namespace tao::pegtl::internal
{
   template< typename M, typename S >
   using minus = rematch< M, not_at< S, eof > >;

}  // namespace tao::pegtl::internal

#endif
