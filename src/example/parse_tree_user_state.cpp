// Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/extra/parse_tree.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   template< typename >
   using selector = std::true_type;

   struct user_state
   {};

   template< typename Rule >
   struct work
   {};

   template<>
   struct work< pegtl::success >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, user_state& /*unused*/ )
      {}
   };

}  // namespace

int main()
{
   example::user_state state;
   pegtl::text_view_input input( "" );
   auto root = pegtl::parse_tree::parse< pegtl::success, example::selector, example::work >( input, state );
   (void)root;
   return 0;
}
