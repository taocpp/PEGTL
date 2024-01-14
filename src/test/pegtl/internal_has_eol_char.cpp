// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/internal/has_eol_char.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace test
   {
      struct baz
      {};

      struct bar
      {
         static constexpr char eol_char = '\0';
      };

      struct foo
      {
         static constexpr char eol_char = '\n';
      };

   }  // namespace test

   void unit_test()
   {
      static_assert( internal::has_eol_char< test::foo > );
      static_assert( !internal::has_eol_char< test::bar > );
      static_assert( !internal::has_eol_char< test::baz > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
