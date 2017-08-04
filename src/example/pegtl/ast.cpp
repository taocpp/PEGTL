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
   struct store_simple : std::false_type
   {
   };

   template< typename >
   struct store_content : std::false_type
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
         emplace_back();
      }

      const node& root() const
      {
         return *stack.front();
      }

      void emplace_back()
      {
         std::unique_ptr< node > r( new node );
         stack.emplace_back( std::move( r ) );
      }
   };

   template< typename Rule, bool = store_simple< Rule >::value, bool = store_content< Rule >::value >
   struct builder_impl;

   template< typename Rule >
   struct builder : builder_impl< Rule >
   {
   };

   template< typename Rule >
   struct builder_impl< Rule, false, false >
      : normal< Rule >
   {
   };

   template< typename Rule >
   struct builder_impl< Rule, true, true >
      : normal< Rule >
   {
      static_assert( sizeof( Rule ) == 0, "error: both store_simple and store_content are set" );
   };

   template< typename Rule >
   struct builder_impl< Rule, true, false >
      : normal< Rule >
   {
      template< typename Input >
      static void start( const Input&, state& s )
      {
         s.emplace_back();
      }

      template< typename Input >
      static void success( const Input&, state& s )
      {
         auto n = std::move( s.stack.back() );
         n->id = &typeid( Rule );
         s.stack.pop_back();
         s.stack.back()->children.emplace_back( std::move( n ) );
      }

      template< typename Input >
      static void failure( const Input&, state& s )
      {
         s.stack.pop_back();
      }
   };

   template< typename Rule >
   struct builder_impl< Rule, false, true >
      : normal< Rule >
   {
      template< typename Input >
      static void start( const Input& in, state& s )
      {
         s.emplace_back();
         s.stack.back()->begin = in.current();
      }

      template< typename Input >
      static void success( const Input& in, state& s )
      {
         auto n = std::move( s.stack.back() );
         n->id = &typeid( Rule );
         n->end = in.current();
         s.stack.pop_back();
         s.stack.back()->children.emplace_back( std::move( n ) );
      }

      template< typename Input >
      static void failure( const Input&, state& s )
      {
         s.stack.pop_back();
      }
   };

   void print_node( const node& n, const std::string& s = "" )
   {
      if( n.id ) {
         if( n.begin ) {
            std::cout << s << internal::demangle( n.id->name() ) << " \"" << std::string( n.begin, n.end ) << '"' << std::endl;
         }
         else {
            std::cout << s << internal::demangle( n.id->name() ) << std::endl;
         }
      }
      else {
         std::cout << "ROOT" << std::endl;
      }
      if( !n.children.empty() ) {
         const auto s2 = s + "  ";
         for( auto& up : n.children ) {
            print_node( *up, s2 );
         }
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
   struct bracketed : seq< open_bracket, expression, close_bracket > {};
   struct value : sor< integer, variable, bracketed > {};
   struct product : list< value, sor< multiply, divide > > {};
   struct expression : list< product, sor< plus, minus > > {};

   struct grammar : must< expression, eof > {};

   template<> struct store_content< integer > : std::true_type {};
   template<> struct store_content< variable > : std::true_type {};
   template<> struct store_simple< plus > : std::true_type {};
   template<> struct store_simple< minus > : std::true_type {};
   template<> struct store_simple< multiply > : std::true_type {};
   template<> struct store_simple< divide > : std::true_type {};
   template<> struct store_simple< bracketed > : std::true_type {};
   template<> struct store_simple< product > : std::true_type {};
   template<> struct store_simple< expression > : std::true_type {};
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
