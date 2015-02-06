// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <pegtl.hh>

namespace sexpr
{
   using namespace pegtl;

   struct line_comment
         : until< eolf > {};

   struct list;

   struct list_comment
         : if_must< at< one< '(' > >, disable< list > > {};

   struct comment
         : if_must< one< '#' >, sor< list_comment, line_comment > > {};

   struct nothing
         : sor< space, comment > {};

   struct number
         : plus< digit > {};

   struct symbol
         : identifier {};

   struct atom
         : sor< number, symbol > {};

   struct anything;

   struct list
         : if_must< one< '(' >, until< one< ')' >, anything > > {};

   struct something
         : sor< atom, list > {};

   struct anything
         : sor< nothing, something > {};

   struct file
         : until< eof, anything > {};

} // sexpr

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      pegtl::parse< sexpr::file >( i, argv );
   }
   return 0;
}
