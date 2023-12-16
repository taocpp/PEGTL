// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"
#include <tao/pegtl/internal/enum_invert_bool.hpp>
#include <tao/pegtl/internal/result_on_found.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< std::underlying_type_t< internal::result_on_found >, bool > );

      static_assert( internal::result_on_found::success != internal::result_on_found::failure );

      static_assert( internal::enum_invert_bool( internal::result_on_found::success ) == internal::result_on_found::failure );
      static_assert( internal::enum_invert_bool( internal::result_on_found::failure ) == internal::result_on_found::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
