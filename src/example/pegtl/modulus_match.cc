// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hh>

namespace modulus
{
   template< unsigned M, unsigned R = 0 >
   struct my_rule
   {
      using analyze_t = tao::pegtl::analysis::generic< tao::pegtl::analysis::rule_type::ANY >;

      static_assert( M > 1, "Modulus must be greater than 1" );
      static_assert( R < M, "Remainder must be less than modulus" );

      template< typename Input >
      static bool match( Input & in )
      {
         if ( ! in.empty() ) {
            if ( ( ( * in.begin() ) % M ) == R ) {
               in.bump( 1 );
               return true;
            }
         }
         return false;
      }
   };

   struct grammar
         : tao::pegtl::until< tao::pegtl::eolf, tao::pegtl::must< my_rule< 3 > > > {};

} // namespace modulus

int main( int argc, char ** argv )
{
   if ( argc > 1 ) {
      tao::pegtl::parse_arg< modulus::grammar >( 1, argv );
   }
   return 0;
}
