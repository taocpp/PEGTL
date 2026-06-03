// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/add_state.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct stateful_rule
   : pegtl::one< 'a' >
{};

struct grammar
   : pegtl::must< stateful_rule, pegtl::eof >
{};

struct constructible_state
{};

struct unconstructible_state
{
   unconstructible_state() = delete;
};

template< typename Rule >
struct action
   : pegtl::nothing< Rule >
{};

template<>
struct action< stateful_rule >
#if TAO_PEGTL_COMPILE_ACCEPT
   : pegtl::add_state< constructible_state >
#else
// include/tao/pegtl/action/add_state.hpp
// static_assert( internal::dependent_false< AddState >, "Unable to instantiate new state!" );
   : pegtl::add_state< unconstructible_state >
#endif
{};

int main()
{
   pegtl::text_view_input in( "a" );
   return pegtl::parse< grammar, action >( in ) ? 0 : 1;
}
