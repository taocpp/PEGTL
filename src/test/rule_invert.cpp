// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< invert< one< 'a' > >, typename not_one< 'a' >::rule_t >();
      verify_meta< invert< not_one< 'a' > >, typename one< 'a' >::rule_t >();
      verify_meta< invert< range< 'a', 'z' > >, typename not_range< 'a', 'z' >::rule_t >();
      verify_meta< invert< not_range< 'a', 'z' > >, typename range< 'a', 'z' >::rule_t >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
