// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>

#include "json_errors.hpp"

using namespace TAO_PEGTL_NAMESPACE;
using grammar = must< json::text, eof >;

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      try {
         argv_input in( argv, i );
         try {
            parse< grammar, nothing, examples::control >( in );
         }
         catch( const parse_error& e ) {
            const auto p = e.positions.front();
            std::cerr << e.what() << '\n'
                      << in.line_at( p ) << '\n'
                      << std::setw( p.byte_in_line ) << ' ' << '^' << std::endl;
         }
      }
      catch( const std::exception& e ) {
         std::cerr << e.what() << std::endl;
      }
   }
   return 0;
}
