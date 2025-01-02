// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/check_depth.hpp>
#include <tao/pegtl/contrib/input_with_depth.hpp>
#include <tao/pegtl/contrib/json.hpp>

#include "json_errors.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

   template< typename >
   struct action
   {};

   template<>
   struct action< pegtl::json::value >
      : pegtl::check_depth< 42 >
   {};

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " JSON\n"
                << "Parse a JSON text.\n\n"
                << "Example: " << argv[ 0 ] << " '{\"foo\":[42,null]}'" << std::endl;
      return 1;
   }
   using input_t = pegtl::input_with_depth< pegtl::argv_input< pegtl::scan::lf_crlf > >;
   input_t in( argv, 1 );
#if defined( __cpp_exceptions )
   try {
      pegtl::parse< example::grammar, example::action, example::control >( in );
   }
   catch( const decltype( in )::parse_error_t& e ) {
      const auto& p = e.position_object();
      std::cerr << e.what() << '\n'
                << in.line_view_at( p ) << '\n'
                << std::setw( int( p.count ) ) << '^' << std::endl;
      return 1;
   }
#else
   if( !pegtl::parse< example::grammar, example::action, example::control >( in ) ) {
      std::cerr << "error occurred" << std::endl;
      return 1;
   }
#endif

   return 0;
}
