// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>

#include "json_errors.hpp"

using namespace TAO_PEGTL_NAMESPACE;
using grammar = must< json::text, eof >;

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      argv_input in( argv, i );
      parse< grammar, nothing, examples::control >( in );
   }
   return 0;
}
