// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/debug/coverage.hpp>
#include <tao/pegtl/debug/print_coverage.hpp>

#include "json_errors.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " FILE\n"
                << "Print coverage of parsing FILE as JSON." << std::endl;
      return 1;
   }
   using input_t = pegtl::text_file_input< pegtl::scan::lf >;
   input_t in( argv[ 1 ] );
   pegtl::coverage_result result;
#if defined( __cpp_exceptions )
   try {
      pegtl::coverage< example::grammar, pegtl::nothing, example::control >( in, result );
   }
   catch( const decltype( in )::parse_error_t& e ) {
      const auto& p = e.position_object();
      std::cerr << e.what() << '\n'
                << in.line_view_at( p ) << '\n'
                << std::setw( int( p.column ) ) << '^' << std::endl;
      return 1;
   }
#else
   if( !pegtl::coverage< example::grammar, pegtl::nothing, example::control >( in, result ) ) {
      std::cerr << "Error occurred -- exceptions disabled" << std::endl;
      return 1;
   }
#endif
   std::cout << result;
   return 0;
}
