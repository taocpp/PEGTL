// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl/contrib/separated_seq.hpp>

// clang-format off
struct A {};
struct B {};
struct C {};
struct D {};

struct S {};
// clang-format on

using namespace TAO_PEGTL_NAMESPACE;

static_assert( std::is_base_of_v< internal::seq<>, separated_seq< S > > );
static_assert( std::is_base_of_v< internal::seq< A >, separated_seq< S, A > > );
static_assert( std::is_base_of_v< internal::seq< A, S, B >, separated_seq< S, A, B > > );
static_assert( std::is_base_of_v< internal::seq< A, S, B, S, C, S, D >, separated_seq< S, A, B, C, D > > );

int main()
{
   return 0;
}
