// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/trace.hpp>

namespace test
{
   using namespace TAO_PEGTL_NAMESPACE;

   using grammar = seq< sor< one< 'a' >, one< 'F' > >, eof >;

}  // namespace test

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const std::string data = "F";
      memory_input in( data, "trace test -- please ignore" );
      // Just enough to see that it compiles and nothing explodes.
      include_trace< type_list< one< 'a' >, one< 'F' > >, test::grammar >( in );
      in.restart();
      exclude_trace< type_list< one< 'a' > >, test::grammar >( in );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
