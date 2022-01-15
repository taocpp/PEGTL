// Copyright (c) 2017-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_EOL_PAIR_HPP
#define TAO_PEGTL_INTERNAL_EOL_PAIR_HPP

#include <cstddef>
#include <utility>

namespace tao::pegtl::internal
{
   using eol_pair = std::pair< bool, std::size_t >;

}  // namespace tao::pegtl::internal

#endif
