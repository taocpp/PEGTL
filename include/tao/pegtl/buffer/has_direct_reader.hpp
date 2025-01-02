// Copyright (c) 2022-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_HAS_DIRECT_READER_HPP
#define TAO_PEGTL_BUFFER_HAS_DIRECT_READER_HPP

#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename, typename = void >
   inline constexpr bool has_direct_reader = false;

   template< typename C >
   inline constexpr bool has_direct_reader< C, decltype( (void)std::declval< C >().direct_reader(), void() ) > = true;

   // The (void) is to shut up a warning from GCC 9 and 10 about the return value of the nodiscard-function direct_reader() being ignored.

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
