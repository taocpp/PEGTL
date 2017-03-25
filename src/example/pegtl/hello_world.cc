// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <string>
#include <iostream>

#include <tao/pegtl.hh>

namespace hello
{
   struct prefix
         : tao::TAOCPP_PEGTL_NAMESPACE::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' > {};

   struct name
         : tao::TAOCPP_PEGTL_NAMESPACE::plus< tao::TAOCPP_PEGTL_NAMESPACE::alpha > {};

   struct grammar
         : tao::TAOCPP_PEGTL_NAMESPACE::must< prefix, name, tao::TAOCPP_PEGTL_NAMESPACE::one< '!' >, tao::TAOCPP_PEGTL_NAMESPACE::eof > {};

   template< typename Rule >
   struct action
         : tao::TAOCPP_PEGTL_NAMESPACE::nothing< Rule > {};

   template<>
   struct action< name >
   {
      template< typename Input >
      static void apply( const Input & in, std::string & name )
      {
         name = in.string();
      }
   };

} // namespace hello

int main( int argc, char ** argv )
{
   if ( argc > 1 ) {
      std::string name;
      tao::TAOCPP_PEGTL_NAMESPACE::parse_arg< hello::grammar, hello::action >( 1, argv, name );
      std::cout << "Good bye, " << name << "!" << std::endl;
   }
}
