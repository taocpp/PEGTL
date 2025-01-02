// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/rewind_mode.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< std::underlying_type_t< rewind_mode >, bool > );

      static_assert( rewind_mode::optional != rewind_mode::required );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
