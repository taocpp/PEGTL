// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl/rules.hpp>

// clang-format off
struct A {};
struct B {};

struct S {};
struct P {};
// clang-format on

using namespace TAO_PEGTL_NAMESPACE;

static_assert( std::is_same_v< internal::success::rule_t, sep_pad< S, P >::rule_t > );
static_assert( std::is_same_v< internal::seq< A >::rule_t, sep_pad< S, P, A >::rule_t > );
static_assert( std::is_same_v< internal::seq< A, internal::seq< internal::star< P >, S, internal::star< P > >, B >::rule_t, sep_pad< S, P, A, B >::rule_t > );

int main()
{
   return 0;
}
