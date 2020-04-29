// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/coverage.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/print_coverage.hpp>

#include "json_errors.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " FILE\n"
                << "Print coverage of parsing FILE as JSON." << std::endl;
      return 1;
   }

   pegtl::file_input in( argv[ 1 ] );
   try {
      const auto data = pegtl::coverage< example::grammar, pegtl::nothing, example::control >( in );
      std::cout << data;
   }
   catch( const pegtl::parse_error& e ) {
      const auto p = e.positions.front();
      std::cerr << e.what() << '\n'
                << in.line_at( p ) << '\n'
                << std::setw( p.byte_in_line ) << ' ' << '^' << std::endl;
      return 1;
   }

   return 0;
}
