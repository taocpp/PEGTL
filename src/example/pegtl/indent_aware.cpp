// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

/* Example file for this grammar:

a = 3
  b = 4
    d = 5
  c = 6
d = 1
foo
  bar
    baz = 42

*/

#include <iostream>
#include <numeric>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/integer.hpp>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace example
{
   // clang-format off

   struct s : pegtl::star< pegtl::blank > {};
   struct eq : pegtl::one< '=' > {};
   struct key : pegtl::plus< pegtl::alpha > {};
   struct value : pegtl::plus< pegtl::digit > {};
   struct indent : pegtl::star< pegtl::one< ' ' > > {};

   struct assignment : pegtl::if_must< eq, s, value > {};

   struct line : pegtl::must< indent, key, s, pegtl::opt< assignment >, pegtl::eolf > {};

   struct grammar : pegtl::until< pegtl::eof, line > {};

   // clang-format on

   struct entry
   {
      unsigned i;
      std::string k;
   };

   struct state
   {
      unsigned indent = 0;
      unsigned converted = 0;
      std::vector< entry > stack;
      std::map< std::string, unsigned > result;
   };

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {
   };

   template<>
   struct action< indent >
   {
      template< typename Input >
      static void apply( const Input& in, state& is )
      {
         is.indent = in.size();
      }
   };

   template<>
   struct action< key >
   {
      template< typename Input >
      static void apply( const Input& in, state& is )
      {
         while( ( !is.stack.empty() ) && ( is.stack.back().i >= is.indent ) ) {
            is.stack.pop_back();
         }
         is.stack.emplace_back( entry{ is.indent, in.string() } );
      }
   };

   template<>
   struct action< value >
      : public pegtl::integer::unsigned_action
   {
   };

   std::string join( const std::vector< entry >& v, const char s = '.' )
   {
      return std::accumulate( v.begin(), v.end(), entry{ 0, "" }, [=]( const entry& l, const entry& r ) { return entry{ 0, l.k + s + r.k }; } ).k.substr( 1 );
   }

   template<>
   struct action< assignment >
   {
      static void apply0( state& is )
      {
         is.result[ join( is.stack ) ] = is.converted;
      }
   };

}  // namespace example

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      pegtl::file_input<> in( argv[ i ] );
      example::state is;
      pegtl::parse< example::grammar, example::action >( in, is );
      for( const auto& j : is.result ) {
         std::cout << j.first << " = " << j.second << std::endl;
      }
   }
   return 0;
}
