// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <string>
#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

template< typename Rule >
using selector = pegtl::parse_tree::selector<
   Rule,
   pegtl::parse_tree::store_content::on<
      pegtl::json::null,
      pegtl::json::true_,
      pegtl::json::false_,
      pegtl::json::number,
      pegtl::json::string,
      pegtl::json::key,
      pegtl::json::array,
      pegtl::json::object,
      pegtl::json::member > >;

int main( int argc, char** argv )
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " JSON\n"
                << "Generate a 'dot' file from the JSON text.\n\n"
                << "Example: " << argv[ 0 ] << " \"{\"foo\":[42,null]}\" | dot -Tpng -o parse_tree.png\n";
      return 1;
   }
   pegtl::argv_input in( argv, 1 );
   try {
      const auto root = pegtl::parse_tree::parse< grammar, selector >( in );
      pegtl::parse_tree::print_dot( std::cout, *root );
      return 0;
   }
   catch( const pegtl::parse_error& e ) {
      const auto p = e.positions.front();
      std::cerr << e.what() << std::endl
                << in.line_at( p ) << std::endl
                << std::string( p.byte_in_line, ' ' ) << '^' << std::endl;
   }
   catch( const std::exception& e ) {
      std::cerr << e.what() << std::endl;
   }

   return 1;
}
