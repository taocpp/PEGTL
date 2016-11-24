// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <iostream>

#include <pegtl.hh>

namespace csv1
{
   // Simple CSV-file format for an unknown-at-compile-time number of values per
   // line, the values are space/tab-padded integers, comment lines start with
   // a hash and are ignored; the grammar does not enforce the same number of
   // values per line, this would have to be done by the actions; last line can
   // end with an LF or CR+LF but doesn't have to.

   // Example file contents parsed by this grammar (excluding C++ comment intro):
   // # This is a comment
   // 123 , 124,41,1
   //  1,2,3,4    
   // 1
   //    1,2


   struct value : pegtl::plus< pegtl::digit > {};
   struct value_item : pegtl::pad< value, pegtl::blank > {};
   struct value_list : pegtl::list_must< value_item, pegtl::one< ',' > > {};
   struct value_line : pegtl::if_must< value_list, pegtl::eolf > {};
   struct comment_line : pegtl::seq< pegtl::one< '#' >, pegtl::until< pegtl::eolf > > {};
   struct line : pegtl::sor< comment_line, value_line > {};
   struct file : pegtl::until< pegtl::eof, line > {};

} // csv1

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      pegtl::file_parser fp( argv[ i ] );
      std::cout << argv[ i ] << " " << fp.parse< csv1::file >() << std::endl;
   }
   return 0;
}
