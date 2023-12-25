// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( internal::input_traits< base_input< ascii::lf > >::is_lazy == false );
      static_assert( internal::input_traits< base_input< ascii::lf > >::is_text == false );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
