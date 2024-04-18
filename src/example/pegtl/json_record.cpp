// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/record.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " JSON\n"
                << "Parse a JSON file.\n\n"
                << "Example: " << argv[ 0 ] << " filename.json" << std::endl;
      return 1;
   }
   using input_t = pegtl::text_file_input<>;
   input_t in( argv[ 1 ] );
   try {
      const auto v = pegtl::record< pegtl::json::number,
                                    pegtl::json::key_content,
                                    pegtl::json::string_content,
                                    pegtl::json::true_,
                                    pegtl::json::false_,
                                    pegtl::json::null >::parse< example::grammar >( in );
      std::cout << v;
   }
   catch( const pegtl::parse_error< input_t::error_position_t >& e ) {
      const auto& p = e.position_object();
      std::cerr << e.what() << '\n'
                << in.line_view_at( p ) << '\n'
                << std::setw( int( p.count ) ) << '^' << std::endl;
      return 1;
   }
   return 0;
}
