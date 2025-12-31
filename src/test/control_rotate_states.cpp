// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/control/rotate_states_left.hpp>
#include <tao/pegtl/control/rotate_states_right.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static_assert( internal::rotate_states_left< 1 >::value< 0, 3 > == 1 );
   static_assert( internal::rotate_states_left< 1 >::value< 1, 3 > == 2 );
   static_assert( internal::rotate_states_left< 1 >::value< 2, 3 > == 0 );

   static_assert( internal::rotate_states_right< 1 >::value< 0, 3 > == 2 );
   static_assert( internal::rotate_states_right< 1 >::value< 1, 3 > == 0 );
   static_assert( internal::rotate_states_right< 1 >::value< 2, 3 > == 1 );

   static_assert( internal::rotate_states_left< 2 >::value< 0, 5 > == 2 );
   static_assert( internal::rotate_states_left< 2 >::value< 1, 5 > == 3 );
   static_assert( internal::rotate_states_left< 2 >::value< 2, 5 > == 4 );
   static_assert( internal::rotate_states_left< 2 >::value< 3, 5 > == 0 );
   static_assert( internal::rotate_states_left< 2 >::value< 4, 5 > == 1 );

   static_assert( internal::rotate_states_right< 2 >::value< 0, 5 > == 3 );
   static_assert( internal::rotate_states_right< 2 >::value< 1, 5 > == 4 );
   static_assert( internal::rotate_states_right< 2 >::value< 2, 5 > == 0 );
   static_assert( internal::rotate_states_right< 2 >::value< 3, 5 > == 1 );
   static_assert( internal::rotate_states_right< 2 >::value< 4, 5 > == 2 );

}  // namespace TAO_PEGTL_NAMESPACE

int main()
{
   return 0;
}
