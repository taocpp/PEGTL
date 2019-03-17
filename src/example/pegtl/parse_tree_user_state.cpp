// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

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
   template< typename Input >
   static void apply( const Input& /*unused*/, user_state& /*unused*/ )
   {}
};

int main()
{
   memory_input<> input( "", "dummy" );

   user_state state;
   auto root = parse_tree::parse< success, selector, work >( input, state );

   return 0;
}
