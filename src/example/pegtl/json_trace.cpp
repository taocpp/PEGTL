// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/trace.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( pegtl::analyze< grammar >() != 0 ) {
      std::cout << "cycles without progress detected!" << std::endl;
      return 1;
   }

   for( int i = 1; i < argc; ++i ) {
      std::cout << "Parsing " << argv[ i ] << std::endl;
      pegtl::argv_input in( argv, i );
      pegtl::standard_tracer tr( in );
      tr.parse< grammar >( in );
   }

   return 0;
}
