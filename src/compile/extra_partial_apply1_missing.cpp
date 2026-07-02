// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

#include <tao/pegtl/extra/partial_apply1.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct first_rule
   : pegtl::one< 'a' >
{};

struct second_rule
   : pegtl::one< 'b' >
{};

struct grammar
   : pegtl::must< pegtl::partial_apply1< first_rule, second_rule >, pegtl::eof >
{};

template< typename Rule >
struct action
   : pegtl::nothing< Rule >
{};

template<>
struct action< first_rule >
#if TAO_PEGTL_COMPILE_REJECT
   : pegtl::maybe_nothing
#endif
{
#if TAO_PEGTL_COMPILE_ACCEPT
   template< typename ActionInput >
   static void apply( const ActionInput& /*unused*/ )
   {}
#else
   // include/tao/pegtl/extra/internal/partial_apply1.hpp
   // static_assert( is_exact_nothing, "partial_apply1 requires apply() or apply0() for the selected rule!" );
#endif
};

template<>
struct action< second_rule >
{
   template< typename ActionInput >
   static void apply( const ActionInput& /*unused*/ )
   {}
};

int main()
{
   pegtl::text_view_input in( "a" );
   return pegtl::parse< grammar, action >( in ) ? 0 : 1;
}
