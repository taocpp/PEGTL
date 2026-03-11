// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/example/fp.hpp>

using namespace TAO_PEGTL_NAMESPACE;

namespace sum
{
   struct padded_double
      : pad< fp::value, space >
   {};

   struct double_list
      : list< padded_double, one< ',' > >
   {};

   struct grammar
      : seq< double_list, eof >
   {};

   template< typename Rule >
   struct action
   {};

   template<>
   struct action< fp::value >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, double& sum )
      {
         // assume all values will fit into a C++ double
         std::stringstream ss( in.string() );
         double v;
         ss >> v;
         sum += v;
      }
   };

}  // namespace sum

int main()
{
   std::cout << "Give me a comma separated list of numbers.\n";
   std::cout << "The numbers are added using the PEGTL.\n";
   std::cout << "Type [q or Q] to quit\n\n";

   std::string str;

   while( !std::getline( std::cin, str ).fail() ) {
      if( str.empty() || str[ 0 ] == 'q' || str[ 0 ] == 'Q' ) {
         break;
      }
      double d = 0.0;
      view_input< void, char, std::string > in( "std::cin", str );
      if( parse< sum::grammar, sum::action >( in, d ) ) {
         std::cout << "parsing OK; sum = " << std::setprecision( 12 ) << d << std::endl;
      }
      else {
         std::cout << "parsing failed" << std::endl;
      }
   }
}
