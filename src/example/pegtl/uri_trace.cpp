// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/trace.hpp>
#include <tao/pegtl/contrib/uri.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

using grammar = pegtl::must< pegtl::uri::URI >;

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      std::cout << "Parsing " << argv[ i ] << std::endl;
      pegtl::argv_input in( argv, i );
      pegtl::complete_tracer tr( in );
      tr.parse< grammar >( in );
   }
   return 0;
}
