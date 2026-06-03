// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/add_guard.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct guarded_rule
   : pegtl::one< 'a' >
{};

struct grammar
   : pegtl::must< guarded_rule, pegtl::eof >
{};

struct constructible_guard
{};

struct unconstructible_guard
{
   unconstructible_guard() = delete;
};

template< typename Rule >
struct action
   : pegtl::nothing< Rule >
{};

template<>
struct action< guarded_rule >
#if TAO_PEGTL_COMPILE_ACCEPT
   : pegtl::add_guard< constructible_guard >
#else
// include/tao/pegtl/action/add_guard.hpp
// static_assert( internal::dependent_false< AddGuard >, "Unable to instantiate guard!" );
   : pegtl::add_guard< unconstructible_guard >
#endif
{};

int main()
{
   pegtl::text_view_input in( "a" );
   return pegtl::parse< grammar, action >( in ) ? 0 : 1;
}
