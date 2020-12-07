// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

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
      : parse_tree::selector< Rule, parse_tree::store_content::on< A, B, C, D > >
   {};

   void unit_test()
   {
      memory_input in( "ac", "input" );
      const auto root = parse_tree::parse< D, selector >( in );
      parse_tree::print_dot( std::cout, *root );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
