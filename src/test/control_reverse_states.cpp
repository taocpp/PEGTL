// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/control/reverse_states.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static_assert( internal::reverse_states::value< 0, 5 > == 4 );
   static_assert( internal::reverse_states::value< 1, 5 > == 3 );
   static_assert( internal::reverse_states::value< 2, 5 > == 2 );
   static_assert( internal::reverse_states::value< 3, 5 > == 1 );
   static_assert( internal::reverse_states::value< 4, 5 > == 0 );

}  // namespace TAO_PEGTL_NAMESPACE

int main()
{
   return 0;
}
