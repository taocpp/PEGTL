// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"
#include <tao/pegtl/apply_mode.hpp>
#include <tao/pegtl/internal/enum_invert_bool.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< std::underlying_type_t< apply_mode >, bool > );

      static_assert( apply_mode::action != apply_mode::nothing );

      static_assert( internal::enum_invert_bool( apply_mode::action ) == apply_mode::nothing );
      static_assert( internal::enum_invert_bool( apply_mode::nothing ) == apply_mode::action );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
