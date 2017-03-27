// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cstring>

#include <iostream>
#include <string>

#include <tao/pegtl.hpp>

namespace dynamic
{
   struct long_literal_open
      : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< '[' >, tao::TAOCPP_PEGTL_NAMESPACE::plus< tao::TAOCPP_PEGTL_NAMESPACE::not_one< '[' > >, tao::TAOCPP_PEGTL_NAMESPACE::one< '[' > >
   {
   };

   struct long_literal_mark
   {
      template< tao::TAOCPP_PEGTL_NAMESPACE::apply_mode, tao::TAOCPP_PEGTL_NAMESPACE::rewind_mode, template< typename... > class Action, template< typename... > class Control, typename Input >
      static bool match( Input& in, const std::string& long_literal_mark, const std::string& )
      {
         if( in.size( long_literal_mark.size() ) >= long_literal_mark.size() ) {
            if( std::memcmp( in.begin(), long_literal_mark.data(), long_literal_mark.size() ) == 0 ) {
               in.bump( long_literal_mark.size() );
               return true;
            }
         }
         return false;
      }
   };

   struct long_literal_close
      : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< ']' >, long_literal_mark, tao::TAOCPP_PEGTL_NAMESPACE::one< ']' > >
   {
   };

   struct long_literal_body
      : tao::TAOCPP_PEGTL_NAMESPACE::any
   {
   };

   struct grammar
      : tao::TAOCPP_PEGTL_NAMESPACE::if_must< long_literal_open, tao::TAOCPP_PEGTL_NAMESPACE::until< long_literal_close, long_literal_body >, tao::TAOCPP_PEGTL_NAMESPACE::eof >
   {
   };

   template< typename Rule >
   struct action
      : tao::TAOCPP_PEGTL_NAMESPACE::nothing< Rule >
   {
   };

   template<>
   struct action< tao::TAOCPP_PEGTL_NAMESPACE::plus< tao::TAOCPP_PEGTL_NAMESPACE::not_one< '[' > > >
   {
      template< typename Input >
      static void apply( const Input& in, std::string& long_literal_mark, const std::string& )
      {
         long_literal_mark = in.string();
      }
   };

   template<>
   struct action< long_literal_body >
   {
      template< typename Input >
      static void apply( const Input& in, const std::string&, std::string& long_literal_body )
      {
         long_literal_body += in.string();
      }
   };

}  // namespace dynamic

int main( int argc, char** argv )
{
   if( argc > 1 ) {
      std::string long_literal_mark;
      std::string long_literal_body;
      tao::TAOCPP_PEGTL_NAMESPACE::parse_arg< dynamic::grammar, dynamic::action >( 1, argv, long_literal_mark, long_literal_body );
      std::cout << "long literal mark was: " << long_literal_mark << std::endl;
      std::cout << "long literal body was: " << long_literal_body << std::endl;
   }
   return 0;
}
