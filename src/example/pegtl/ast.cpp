// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include <tao/pegtl.hpp>

using namespace tao::TAOCPP_PEGTL_NAMESPACE;

namespace ast
{
   template< typename >
   struct marker : std::false_type
   {
   };

   struct node
   {
      std::vector< std::unique_ptr< node > > children;
      const std::type_info* id = nullptr;
      const char* begin = nullptr;
      const char* end = nullptr;
   };

   struct state
   {
      std::vector< std::unique_ptr< node > > stack;

      state()
      {
         std::unique_ptr< node > r( new node );
         r->id = &typeid( void );
         r->begin = "root";
         r->end = r->begin + 4;
         stack.emplace_back( std::move( r ) );
      }

      const node& root() const
      {
         return *stack.front();
      }
   };

   template< typename Rule, bool = marker< Rule >::value >
   struct builder_impl;

   template< typename Rule >
   struct builder : builder_impl< Rule >
   {
   };

   template< typename Rule >
   struct builder_impl< Rule, false >
      : normal< Rule >
   {
   };

   template< typename Rule >
   struct builder_impl< Rule, true >
      : normal< Rule >
   {
      template< typename Input, typename = typename std::enable_if< marker< Rule >::value >::type >
      static void start( const Input& in, state& s )
      {
         s.stack.emplace_back( new ast::node );
         s.stack.back()->begin = in.current();
      }

      template< typename Input, typename = typename std::enable_if< marker< Rule >::value >::type >
      static void success( const Input& in, state& s )
      {
         auto n = std::move( s.stack.back() );
         n->id = &typeid( Rule );
         n->end = in.current();
         s.stack.pop_back();
         s.stack.back()->children.emplace_back( std::move( n ) );
      }

      template< typename Input, typename = typename std::enable_if< marker< Rule >::value >::type >
      static void failure( const Input&, state& s )
      {
         s.stack.pop_back();
      }
   };

   void print_node( const node& n, const std::string& s = "" )
   {
      std::cout << s << internal::demangle( n.id->name() ) << " \"" << std::string( n.begin, n.end ) << '"' << std::endl;
      for( auto& up : n.children ) {
         print_node( *up, s + "  " );
      }
   }

   // clang-format off
   struct integer : plus< digit > {};
   struct variable : identifier {};

   struct plus : pad< one< '+' >, space > {};
   struct minus : pad< one< '-' >, space > {};
   struct multiply : pad< one< '*' >, space > {};
   struct divide : pad< one< '/' >, space > {};

   struct open_bracket : seq< one< '(' >, star< space > > {};
   struct close_bracket : seq< star< space >, one< ')' > > {};

   struct expression;
   struct term : seq< open_bracket, expression, close_bracket > {};
   struct value : sor< integer, variable, term > {};
   struct product : list< value, sor< multiply, divide > > {};
   struct sum : list< product, sor< plus, minus > > {};
   struct expression : seq< sum > {};

   struct grammar : must< expression, eof > {};

   template<> struct marker< integer > : std::true_type {};
   template<> struct marker< variable > : std::true_type {};
   template<> struct marker< plus > : std::true_type {};
   template<> struct marker< minus > : std::true_type {};
   template<> struct marker< term > : std::true_type {};
   template<> struct marker< multiply > : std::true_type {};
   template<> struct marker< divide > : std::true_type {};
   template<> struct marker< product > : std::true_type {};
   template<> struct marker< sum > : std::true_type {};
   template<> struct marker< expression > : std::true_type {};
   // clang-format on

}  // namespace ast

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      argv_input<> in( argv, i );

      ast::state s;
      parse< ast::grammar, nothing, ast::builder >( in, s );
      print_node( s.root() );
   }
   return 0;
}
