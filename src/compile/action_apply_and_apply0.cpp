// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct action_rule
   : pegtl::one< 'a' >
{};

struct grammar
   : pegtl::must< action_rule, pegtl::eof >
{};

template< typename Rule >
struct action
   : pegtl::nothing< Rule >
{};

template<>
struct action< action_rule >
{
   template< typename ActionInput >
   static void apply( const ActionInput& /*unused*/ )
   {}

#if TAO_PEGTL_COMPILE_REJECT
   // include/tao/pegtl/match.hpp
   // static_assert( !( has_apply && has_apply0 ), "Both apply() and apply0() detected in action!" );
   static void apply0()
   {}
#endif
};

int main()
{
   pegtl::text_view_input in( "a" );
   return pegtl::parse< grammar, action >( in ) ? 0 : 1;
}
