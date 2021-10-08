// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl/contrib/sep.hpp>

#include <type_traits>

// clang-format off
struct A {};
struct B {};
struct C {};
struct D {};

struct S {};
// clang-format on

using namespace TAO_PEGTL_NAMESPACE;
static_assert( std::is_base_of_v< internal::seq<>, sep< S >::seq<> > );
static_assert( std::is_base_of_v< internal::seq< A >, sep< S >::seq< A > > );
static_assert( std::is_base_of_v< internal::seq< A, S, B >, sep< S >::seq< A, B > > );
static_assert( std::is_base_of_v< internal::seq< A, S, B, S, C, S, D >, sep< S >::seq< A, B, C, D > > );

int main()
{}
