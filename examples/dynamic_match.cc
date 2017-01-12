// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <cstring>

#include <string>
#include <iostream>

#include <pegtl.hh>

namespace dynamic
{
   struct long_literal_open
         : pegtl::seq< pegtl::one< '[' >, pegtl::plus< pegtl::not_one< '[' > >, pegtl::one< '[' > > {};

   struct long_literal_mark
   {
      template< pegtl::apply_mode, pegtl::rewind_mode, template< typename ... > class Action, template< typename ... > class Control, typename Input >
      static bool match( Input & in, const std::string & long_literal_mark, const std::string & )
      {
         if ( in.size( long_literal_mark.size() ) >= long_literal_mark.size() ) {
            if ( std::memcmp( in.begin(), long_literal_mark.data(), long_literal_mark.size() ) == 0 ) {
               in.bump( long_literal_mark.size() );
               return true;
            }
         }
         return false;
      }
   };

   struct long_literal_close
         : pegtl::seq< pegtl::one< ']' >, long_literal_mark, pegtl::one< ']' > > {};

   struct long_literal_body
         : pegtl::any {};

   struct grammar
         : pegtl::if_must< long_literal_open, pegtl::until< long_literal_close, long_literal_body >, pegtl::eof > {};

   template< typename Rule >
   struct action
         : pegtl::nothing< Rule > {};

   template<> struct action< pegtl::plus< pegtl::not_one< '[' > > >
   {
      template< typename Input >
      static void apply( const Input & in, std::string & long_literal_mark, const std::string & )
      {
         long_literal_mark = in.string();
      }
   };

   template<> struct action< long_literal_body >
   {
      template< typename Input >
      static void apply( const Input & in, const std::string &, std::string & long_literal_body )
      {
         long_literal_body += in.string();
      }
   };
} // namespace dynamic

int main( int argc, char ** argv )
{
   if ( argc > 1 ) {
      std::string long_literal_mark;
      std::string long_literal_body;
      pegtl::parse_arg< dynamic::grammar, dynamic::action >( 1, argv, long_literal_mark, long_literal_body );
      std::cout << "long literal mark was: " << long_literal_mark << std::endl;
      std::cout << "long literal body was: " << long_literal_body << std::endl;
   }
   return 0;
}
