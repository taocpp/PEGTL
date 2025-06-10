// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/debug/analyze_traits_enum.hpp>

static_assert( TAO_PEGTL_NAMESPACE::analyze_traits_enum::any != TAO_PEGTL_NAMESPACE::analyze_traits_enum::opt );
static_assert( TAO_PEGTL_NAMESPACE::analyze_traits_enum::any != TAO_PEGTL_NAMESPACE::analyze_traits_enum::seq );
static_assert( TAO_PEGTL_NAMESPACE::analyze_traits_enum::any != TAO_PEGTL_NAMESPACE::analyze_traits_enum::sor );
static_assert( TAO_PEGTL_NAMESPACE::analyze_traits_enum::seq != TAO_PEGTL_NAMESPACE::analyze_traits_enum::opt );
static_assert( TAO_PEGTL_NAMESPACE::analyze_traits_enum::seq != TAO_PEGTL_NAMESPACE::analyze_traits_enum::sor );
static_assert( TAO_PEGTL_NAMESPACE::analyze_traits_enum::sor != TAO_PEGTL_NAMESPACE::analyze_traits_enum::opt );

int main()
{
   return 0;
}
