// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl.hpp>

#include <tao/pegtl/extra/parse_tree.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct key : pegtl::plus< pegtl::ascii::alpha > {};
   struct value : pegtl::plus< pegtl::ascii::digit > {};
   struct assignment : pegtl::seq< key, pegtl::one< '=' >, value, pegtl::eof > {};

   template< typename Rule >
   using selector = pegtl::parse_tree::selector< Rule,
                                                  pegtl::parse_tree::store_content::on< key, value >,
                                                  pegtl::parse_tree::remove_content::on< assignment > >;

}  // namespace

int main()  // NOLINT(bugprone-exception-escape) -- Deliberately instantiates throwing parse paths.
{
   pegtl::text_view_input<> in( "answer=42" );
   const auto root = pegtl::parse_tree::parse< assignment, selector >( in );
   return ( root && ( root->children.size() == 1 ) ) ? 0 : 1;
}
