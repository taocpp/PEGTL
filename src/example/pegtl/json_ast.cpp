// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include "json_errors.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

   template< typename Rule >
   using selector = pegtl::parse_tree::selector<
      Rule,
      pegtl::parse_tree::remove_content::on<
         pegtl::json::null,
         pegtl::json::true_,
         pegtl::json::false_,
         pegtl::json::array,
         pegtl::json::object,
         pegtl::json::member >,
      pegtl::parse_tree::store_content::on<
         pegtl::json::number,
         pegtl::json::string,
         pegtl::json::key > >;

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " JSON\n"
                << "Generate a 'dot' file from the JSON text.\n\n"
                << "Example: " << argv[ 0 ] << " '{\"foo\":[42,null]}' | dot -Tpng -o parse_tree.png\n";
      return 1;
   }

   pegtl::argv_input<> in( argv, 1 );
#if defined( __cpp_exceptions )
   try {
      const auto root = pegtl::parse_tree::parse< example::grammar, example::selector, pegtl::nothing, example::control >( in );
      pegtl::parse_tree::print_dot( std::cout, *root );
   }
   catch( const decltype( in )::parse_error_t& e ) {
      const auto& p = e.position_object();
      std::cerr << e.what() << std::endl
                << in.line_view_at( p ) << std::endl
                << std::setw( int( p.count ) ) << '^' << std::endl;
      return 1;
   }
#else
   if( const auto root = pegtl::parse_tree::parse< example::grammar, example::selector, pegtl::nothing, example::control >( in ) ) {
      pegtl::parse_tree::print_dot( std::cout, *root );
   }
   else {
      std::cerr << "error occurred" << std::endl;
      return 1;
   }
#endif

   return 0;
}
