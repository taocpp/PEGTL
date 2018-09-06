// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/parse_tree.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      // clang-format off
      struct A : one< 'a' > {};
      struct B : one< 'b' > {};
      struct C : one< 'c' > {};

      struct D : sor< seq< A, B >, seq< A, C > > {};
      // clang-format on

      template< typename Rule >
      struct selector
         : parse_tree::selector< Rule, parse_tree::apply_remove_content::to< A, B, C, D > >
      {
      };

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

      void unit_test()
      {
         memory_input<> in( "ac", "input" );
         const auto r = parse_tree::parse< D, selector >( in );
         TAO_PEGTL_TEST_ASSERT( r->children.size() == 1 );
         const auto* d = r->children.begin()->get();
         TAO_PEGTL_TEST_ASSERT( d->id == &typeid( D ) );
         TAO_PEGTL_TEST_ASSERT( d->children.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( d->children.front()->id == &typeid( A ) );
         TAO_PEGTL_TEST_ASSERT( d->children.back()->id == &typeid( C ) );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
