// Copyright (c) 2021-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl/rules.hpp>

// clang-format off
struct A {};
struct B {};
struct C {};
struct D {};

struct S {};
// clang-format on

using namespace TAO_PEGTL_NAMESPACE;

static_assert( std::is_same_v< internal::success::rule_t, separated< S >::rule_t > );
static_assert( std::is_same_v< empty_list, separated< S >::subs_t > );
static_assert( std::is_same_v< internal::seq< A >::rule_t, separated< S, A >::rule_t > );
static_assert( std::is_same_v< type_list< A >, separated< S, A >::subs_t > );
static_assert( std::is_same_v< internal::seq< A, S, B >::rule_t, separated< S, A, B >::rule_t > );
static_assert( std::is_same_v< type_list< A, S, B >, separated< S, A, B >::subs_t > );
static_assert( std::is_same_v< internal::seq< A, S, B, S, C, S, D >::rule_t, separated< S, A, B, C, D >::rule_t > );
static_assert( std::is_same_v< type_list< A, S, B, S, C, S, D >, separated< S, A, B, C, D >::subs_t > );

int main()
{
   return 0;
}
