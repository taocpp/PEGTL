// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/parse_tree.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct A : one< 'a' > {};
   struct B : one< 'b' > {};
   struct C : one< 'c' > {};

   struct D : sor< seq< A, B >, seq< A, C > > {};
   struct E : star< A, B > {};
   struct F : seq< E > {};

#if defined( __cpp_exceptions )
   struct D2 : sor< try_catch_return_false< if_must< A, B > >, seq< A, C > > {};
#else
   struct D2 : D {};
#endif
   // clang-format on

   template< typename Rule >
   using selector = parse_tree::selector< Rule,
                                          parse_tree::store_content::on< A, B >,
                                          parse_tree::remove_content::on< C >,
                                          parse_tree::fold_one::on< D > >;

   template< typename Rule >
   using selector2 = parse_tree::selector< Rule,
                                           parse_tree::store_content::on< A >,
                                           parse_tree::discard_empty::on< B >,
                                           parse_tree::discard_empty::on< F >,
                                           parse_tree::fold_one::on< E > >;

   void unit_test()
   {
      {
         text_view_input<> in( "ac" );
         const auto r = parse_tree::parse< D, selector >( in );
         TAO_PEGTL_TEST_ASSERT( r );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( r->data.empty() );
         TAO_PEGTL_TEST_ASSERT( r->children.size() == 1 );

         const auto& d = r->children.front();
         TAO_PEGTL_TEST_ASSERT( !d->is_root() );
         TAO_PEGTL_TEST_ASSERT( d->is_type< D >() );
         TAO_PEGTL_TEST_ASSERT( d->data.empty() );

         TAO_PEGTL_TEST_ASSERT( d->children.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( d->children.front()->is_type< A >() );
         TAO_PEGTL_TEST_ASSERT( d->children.back()->is_type< C >() );

         text_view_input<> in2( "x" );
         const auto r2 = parse_tree::parse< D, selector >( in2 );
         TAO_PEGTL_TEST_ASSERT( !r2 );
      }

      {
         text_view_input<> in( "aba" );
         const auto r = parse_tree::parse< E, selector >( in );
         TAO_PEGTL_TEST_ASSERT( r );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( r->data.empty() );

         TAO_PEGTL_TEST_ASSERT( r->children.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( r->children.front()->is_type< A >() );
         TAO_PEGTL_TEST_ASSERT( r->children.back()->is_type< B >() );
      }

      {
         text_view_input<> in( "ab" );
         const auto r = parse_tree::parse< E, selector2 >( in );
         TAO_PEGTL_TEST_ASSERT( r );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( r->data.empty() );

         TAO_PEGTL_TEST_ASSERT( r->children.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( r->children.front()->is_type< A >() );
      }

      {
         text_view_input<> in( "aba" );
         const auto r = parse_tree::parse< F, selector2 >( in );
         TAO_PEGTL_TEST_ASSERT( r );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( r->data.empty() );
         TAO_PEGTL_TEST_ASSERT( r->children.size() == 1 );

         const auto& f = r->children.front();
         TAO_PEGTL_TEST_ASSERT( !f->is_root() );
         TAO_PEGTL_TEST_ASSERT( f->is_type< F >() );
         TAO_PEGTL_TEST_ASSERT( f->data.empty() );
         TAO_PEGTL_TEST_ASSERT( f->children.size() == 1 );

         const auto& a = f->children.front();
         TAO_PEGTL_TEST_ASSERT( !a->is_root() );
         TAO_PEGTL_TEST_ASSERT( a->is_type< A >() );
         TAO_PEGTL_TEST_ASSERT( !a->data.empty() );
         TAO_PEGTL_TEST_ASSERT( a->children.empty() );
      }

      {
         text_view_input<> in( "ac" );
         const auto r = parse_tree::parse< D2, selector >( in );
         TAO_PEGTL_TEST_ASSERT( r );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( r->data.empty() );

         TAO_PEGTL_TEST_ASSERT( r->children.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( r->children.front()->is_type< A >() );
         TAO_PEGTL_TEST_ASSERT( r->children.back()->is_type< C >() );
      }

      {
         text_view_input<> in( "ac" );
         const auto r = parse_tree::parse< D2 >( in );
         TAO_PEGTL_TEST_ASSERT( r );
         TAO_PEGTL_TEST_ASSERT( r->is_root() );
         TAO_PEGTL_TEST_ASSERT( r->data.empty() );
         TAO_PEGTL_TEST_ASSERT( r->children.size() == 1 );

         const auto& d2 = r->children.front();
         TAO_PEGTL_TEST_ASSERT( !d2->is_root() );
         TAO_PEGTL_TEST_ASSERT( d2->is_type< D2 >() );

         TAO_PEGTL_TEST_ASSERT( !d2->data.empty() );
         TAO_PEGTL_TEST_ASSERT( d2->begin.count == 0 );
         TAO_PEGTL_TEST_ASSERT( d2->end.count == 2 );
         TAO_PEGTL_TEST_ASSERT( d2->string() == "ac" );

         const auto& internal = d2->children.front();
         TAO_PEGTL_TEST_ASSERT( !internal->is_root() );
         TAO_PEGTL_TEST_ASSERT( internal->is_type< seq< A, C > >() );

         TAO_PEGTL_TEST_ASSERT( !internal->data.empty() );
         TAO_PEGTL_TEST_ASSERT( internal->begin.count == 0 );
         TAO_PEGTL_TEST_ASSERT( internal->end.count == 2 );
         TAO_PEGTL_TEST_ASSERT( internal->data == "ac" );

         TAO_PEGTL_TEST_ASSERT( internal->children.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( internal->children.front()->is_type< A >() );
         TAO_PEGTL_TEST_ASSERT( internal->children.back()->is_type< C >() );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
