// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include "test.hpp"

#include <tao/pegtl/contrib/trace.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using grammar = seq< sor< try_catch< must< one< 'a' > > >, one< 'F' > >, eof >;

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
