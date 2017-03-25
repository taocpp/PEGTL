// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <string>
#include <iostream>

#include <tao/pegtl.hh>

namespace hello
{
   struct prefix
         : tao::pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' > {};

   struct name
         : tao::pegtl::plus< tao::pegtl::alpha > {};

   struct grammar
         : tao::pegtl::must< prefix, name, tao::pegtl::one< '!' >, tao::pegtl::eof > {};

   template< typename Rule >
   struct action
         : tao::pegtl::nothing< Rule > {};

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
      tao::pegtl::parse_arg< hello::grammar, hello::action >( 1, argv, name );
      std::cout << "Good bye, " << name << "!" << std::endl;
   }
}
