// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/has_eol_rule.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( internal::has_eol_rule< internal::input_with_lines< ascii::any7, internal::view_input< char > > > );
      static_assert( !internal::has_eol_rule< internal::input_with_lines< void, internal::view_input< char > > > );
      static_assert( !internal::has_eol_rule< internal::view_input< char > > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
