// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/debug/trace.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using grammar = seq< sor< one< 'a' >, one< 'F' > >, eof >;

   void unit_test()
   {
      const std::string data = "F";
      view_input in( data );
      include_trace< type_list< one< 'a' >, one< 'F' > >, grammar >( in );
      in.restart();
      exclude_trace< type_list< one< 'a' > >, grammar >( in );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
