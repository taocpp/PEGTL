// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cstdlib>
#include <string>
#include <iostream>

#include <tao/pegtl.hpp>

#include "double.hpp"

namespace sum
{
   struct padded_double
         : tao::TAOCPP_PEGTL_NAMESPACE::pad< double_::grammar, tao::TAOCPP_PEGTL_NAMESPACE::space > {};

   struct double_list
         : tao::TAOCPP_PEGTL_NAMESPACE::list< padded_double, tao::TAOCPP_PEGTL_NAMESPACE::one< ',' > > {};

   struct grammar
         : tao::TAOCPP_PEGTL_NAMESPACE::seq< double_list, tao::TAOCPP_PEGTL_NAMESPACE::eof > {};

   template< typename Rule >
   struct action
         : tao::TAOCPP_PEGTL_NAMESPACE::nothing< Rule > {};

   template<>
   struct action< double_::grammar >
   {
      template< typename Input >
      static void apply( const Input & in, double & sum )
      {
         // assume all values will fit into a C++ double
         char* ptr = const_cast< char* >( in.end() );
         sum += std::strtod( const_cast< char* >( in.begin() ), & ptr );
      }
   };

} // namespace sum

int main()
{
   std::cout << "Give me a comma separated list of numbers.\n";
   std::cout << "The numbers are added using the PEGTL.\n";
   std::cout << "Type [q or Q] to quit\n\n";

   std::string str;

   while ( std::getline( std::cin, str ) ) {
      if ( str.empty() || str[ 0 ] == 'q' || str[ 0 ] == 'Q' ) {
         break;
      }
      double d = 0.0;
      if ( tao::TAOCPP_PEGTL_NAMESPACE::parse_string< sum::grammar, sum::action >( str, "std::cin", d ) ) {
         std::cout << "parsing OK; sum = " << d << std::endl;
      }
      else {
         std::cout << "parsing failed" << std::endl;
      }
   }
}
