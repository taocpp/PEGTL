// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FILE_INPUT_HPP
#define TAO_PEGTL_INTERNAL_FILE_INPUT_HPP

#include "../config.hpp"
#include "../system.hpp"

#if defined( TAO_PEGTL_MMAP_AVAILABLE )

#include "mmap_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   using file_input = mmap_input< char >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#else

#include "read_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   using file_input = read_input;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
// FCOV_EXCL_FILE
#endif
