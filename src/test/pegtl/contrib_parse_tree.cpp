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
         : parse_tree::selector< Rule, parse_tree::apply_store_content::to< A, B, C, D > >
      {
      };

      void unit_test()
      {
         memory_input<> in( "ac", "input" );
         const auto r = parse_tree::parse< D, selector >( in );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( !r->has_content() );
         TAO_PEGTL_TEST_ASSERT( r->children.size() == 1 );

         const auto& d = r->children.front();
         TAO_PEGTL_TEST_ASSERT( !d->is_root() );
         TAO_PEGTL_TEST_ASSERT( d->id == &typeid( D ) );
         TAO_PEGTL_TEST_ASSERT( d->is< D >() );
#if !defined( _MSC_VER )
         TAO_PEGTL_TEST_ASSERT( d->name() == "tao::pegtl::D" );
#endif

         TAO_PEGTL_TEST_ASSERT( d->has_content() );
         TAO_PEGTL_TEST_ASSERT( d->begin().byte == 0 );
         TAO_PEGTL_TEST_ASSERT( d->end().byte == 2 );
         TAO_PEGTL_TEST_ASSERT( d->content() == "ac" );

         TAO_PEGTL_TEST_ASSERT( d->children.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( d->children.front()->is< A >() );
         TAO_PEGTL_TEST_ASSERT( d->children.back()->is< C >() );

         memory_input<> in2( "x", "input" );
         const auto r2 = parse_tree::parse< D, selector >( in2 );
         TAO_PEGTL_TEST_ASSERT( !r2 );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
