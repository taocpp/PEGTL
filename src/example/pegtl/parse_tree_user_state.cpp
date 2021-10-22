// Copyright (c) 2017-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

using namespace TAO_PEGTL_NAMESPACE;

template< typename >
using selector = std::true_type;

struct user_state
{};

template< typename Rule >
struct work
{};

template<>
struct work< success >
{
   template< typename ActionInput >
   static void apply( const ActionInput& /*unused*/, user_state& /*unused*/ )
   {}
};

int main()
{
   memory_input input( "", "dummy" );

   user_state state;
   auto root = parse_tree::parse< success, selector, work >( input, state );

   return 0;
}
