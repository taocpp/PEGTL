// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <iostream>

#include <pegtl.hh>

namespace csv2
{
   // Simple CSV-file format for a known-at-compile-time number of values per
   // line, the values are strings that can use quotes when they contain commas,
   // if quotes are used they have to be the first character (of the line or
   // after the comma); quoted strings can't contain quotes, no string can have
   // LF or CR; last line has to end with an LF or CR+LF.

   // Example file contents parsed by this grammar (excluding C++ comment intro):
   // a,b,c
   // "foo","bar","baz"
   // ",,,",13,42
   // aha """,yes, this works

   template< int C > struct string_without : pegtl::star< pegtl::not_one< C, 10, 13 > > {};
   struct plain_value : string_without< ',' > {};
   struct quoted_value : pegtl::if_must< pegtl::one< '"' >, string_without< '"' >, pegtl::one< '"' > > {};
   struct value : pegtl::sor< quoted_value, plain_value > {};
   template< unsigned N > struct line : pegtl::seq< value, pegtl::rep< N - 1, pegtl::one< ',' >, value >, pegtl::eol > {};
   template< unsigned N > struct file : pegtl::until< pegtl::eof, line< N > > { static_assert( N, "N must be positive" ); };

} // csv2

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      pegtl::file_parser fp( argv[ i ] );
      std::cout << argv[ i ] << " " << fp.parse< csv2::file< 3 > >() << std::endl;
   }
   return 0;
}
