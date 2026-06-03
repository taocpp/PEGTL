// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/change_action.hpp>

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

template< typename Rule >
struct other_action
   : pegtl::nothing< Rule >
{};

template<>
struct action< action_rule >
#if TAO_PEGTL_COMPILE_ACCEPT
   : pegtl::change_action< other_action >
#else
// include/tao/pegtl/action/change_action.hpp
// static_assert( !std::is_same_v< Action< void >, NewAction< void > >, "Old and new action class templates coincide!" );
   : pegtl::change_action< action >
#endif
{};

int main()
{
   pegtl::text_view_input in( "a" );
   return pegtl::parse< grammar, action >( in ) ? 0 : 1;
}
