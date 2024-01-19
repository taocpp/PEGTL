// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/internal/analyze_group.hpp>

int main()
{
   static_assert( TAO_PEGTL_NAMESPACE::internal::analyze_group::any != TAO_PEGTL_NAMESPACE::internal::analyze_group::opt );
   static_assert( TAO_PEGTL_NAMESPACE::internal::analyze_group::any != TAO_PEGTL_NAMESPACE::internal::analyze_group::seq );
   static_assert( TAO_PEGTL_NAMESPACE::internal::analyze_group::any != TAO_PEGTL_NAMESPACE::internal::analyze_group::sor );
   static_assert( TAO_PEGTL_NAMESPACE::internal::analyze_group::seq != TAO_PEGTL_NAMESPACE::internal::analyze_group::opt );
   static_assert( TAO_PEGTL_NAMESPACE::internal::analyze_group::seq != TAO_PEGTL_NAMESPACE::internal::analyze_group::sor );
   static_assert( TAO_PEGTL_NAMESPACE::internal::analyze_group::sor != TAO_PEGTL_NAMESPACE::internal::analyze_group::opt );
   return 0;
}
