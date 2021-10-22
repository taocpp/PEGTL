// Copyright (c) 2020-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include "test.hpp"

#include <tao/pegtl/contrib/trace.hpp>

namespace TAO_PEGTL_NAMESPACE
{
#if defined( __cpp_exceptions )
   using grammar = seq< sor< try_catch< must< one< 'a' > > >, one< 'F' > >, eof >;
#else
   using grammar = seq< sor< one< 'a' >, one< 'F' > >, eof >;
#endif

   void unit_test()
   {
      const std::string data = "F";
      memory_input in( data, __FILE__ );
      // Just enough to see that it compiles and nothing explodes;
      // the output format probabaly changes between compilers and
      // versions making a proper test difficult.
      standard_trace< grammar >( in );
      in.restart();
      complete_trace< grammar >( in );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
