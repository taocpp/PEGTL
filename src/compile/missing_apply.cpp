// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct action_rule : pegtl::one< 'a' >
{};

struct grammar : pegtl::must< action_rule, pegtl::eof >
{};

template< typename Rule >
struct action
   : pegtl::nothing< Rule >
{};

template<>
struct action< action_rule >
{
#if TAO_PEGTL_COMPILE_ACCEPT
   static void apply0()
   {}
#endif
};

int main()
{
   return pegtl::parse< grammar, action >( pegtl::text_view_input< pegtl::scan::lf >( "a" ) ) ? 0 : 1;
}
