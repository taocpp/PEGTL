// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <string>
#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

namespace example
{
   // the grammar

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
   struct bracketed : if_must< open_bracket, expression, close_bracket > {};
   struct value : sor< integer, variable, bracketed >{};
   struct product : list_must< value, sor< multiply, divide > > {};
   struct expression : list_must< product, sor< plus, minus > > {};

   struct grammar : seq< expression, eof > {};

   // after a node is stored successfully, you can add an optional transformer like this:
   struct rearrange : std::true_type
   {
      // recursively rearrange nodes. the basic principle is:
      //
      // from:          PROD/EXPR
      //                /   |   \          (LHS... may be one or more children, followed by OP,)
      //             LHS... OP   RHS       (which is one operator, and RHS, which is a single child)
      //
      // to:               OP
      //                  /  \             (OP now has two children, the original PROD/EXPR and RHS)
      //         PROD/EXPR    RHS          (Note that PROD/EXPR has two fewer children now)
      //             |
      //            LHS...
      //
      // if only one child is left for LHS..., replace the PROD/EXPR with the child directly.
      // otherwise, perform the above transformation, then apply it recursively until LHS...
      // becomes a single child, which then replaces the parent node and the recursion ends.
      static void transform( std::unique_ptr< parse_tree::node >& n )
      {
         if( n->children.size() == 1 ) {
            n = std::move( n->children.back() );
         }
         else {
            n->remove_content();
            auto& c = n->children;
            auto r = std::move( c.back() );
            c.pop_back();
            auto o = std::move( c.back() );
            c.pop_back();
            o->children.emplace_back( std::move( n ) );
            o->children.emplace_back( std::move( r ) );
            n = std::move( o );
            transform( n->children.front() );
         }
      }
   };

   // select which rules in the grammar will produce parse tree nodes:

   template< typename Rule >
   using selector = parse_tree::selector<
      Rule,
      parse_tree::apply_store_content::to<
         integer,
         variable >,
      parse_tree::apply_remove_content::to<
         plus,
         minus,
         multiply,
         divide >,
      parse_tree::apply< rearrange >::to<
         product,
         expression > >;

   // debugging/show result:

   void print_node( const parse_tree::node& n, const std::string& s = "" )
   {
      // detect the root node:
      if( n.is_root() ) {
         std::cout << "ROOT" << std::endl;
      }
      else {
         if( n.has_content() ) {
            std::cout << s << n.name() << " \"" << n.content() << "\" at " << n.begin() << " to " << n.end() << std::endl;
         }
         else {
            std::cout << s << n.name() << " at " << n.begin() << std::endl;
         }
      }
      // print all child nodes
      if( !n.children.empty() ) {
         const auto s2 = s + "  ";
         for( auto& up : n.children ) {
            print_node( *up, s2 );
         }
      }
   }

}  // namespace example

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      try {
         argv_input<> in( argv, i );
         if( const auto root = parse_tree::parse< example::grammar, example::selector >( in ) ) {
            example::print_node( *root );
         }
         else {
            std::cout << "PARSE FAILED" << std::endl;
         }
      }
      catch( const std::exception& e ) {
         std::cout << "PARSE FAILED WITH EXCEPTION: " << e.what() << std::endl;
      }
   }
   return 0;
}
