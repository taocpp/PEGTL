// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/extra/dispatch.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct dispatched_rule
   : pegtl::one< 'a' >
{};

struct grammar
   : pegtl::must< dispatched_rule, pegtl::eof >
{};

struct first_action
{
   static void apply0()
   {}
};

struct second_action
{
   static void apply0()
   {}
};

using first_clause = pegtl::clause1< first_action, dispatched_rule >;
using second_clause = pegtl::clause1< second_action, dispatched_rule >;

int main()
{
   pegtl::text_view_input in( "a" );
#if TAO_PEGTL_COMPILE_ACCEPT
   return pegtl::dispatch< first_clause >::parse< grammar >( in ) ? 0 : 1;
#else
   // include/tao/pegtl/extra/dispatch.hpp
   // static_assert( dependent_false< Rule, Clauses... >, "More than one action clause applies!" );
   return pegtl::dispatch< first_clause, second_clause >::parse< grammar >( in ) ? 0 : 1;
#endif
}
