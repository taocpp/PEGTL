// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/internal/math_utility.hpp>

using namespace TAO_PEGTL_NAMESPACE;

int main()
{
   static_assert( internal::is_divisible( 4, 1 ) );
   static_assert( internal::is_divisible( 4, 2 ) );
   static_assert( internal::is_divisible( 40, 40 ) );
   static_assert( internal::is_divisible( 400, 20 ) );

   static_assert( !internal::is_divisible( 1, 2 ) );
   static_assert( !internal::is_divisible( 20, 400 ) );
   static_assert( !internal::is_divisible( 21, 14 ) );

   static_assert( internal::ranges_disjoint( 10, 20, 30, 40 ) );
   static_assert( internal::ranges_disjoint( 30, 40, 10, 20 ) );
   static_assert( internal::ranges_disjoint( 10, 20, 21, 40 ) );
   static_assert( internal::ranges_disjoint( 21, 40, 10, 20 ) );

   static_assert( !internal::ranges_overlap( 10, 20, 30, 40 ) );
   static_assert( !internal::ranges_overlap( 30, 40, 10, 20 ) );
   static_assert( !internal::ranges_overlap( 10, 20, 21, 40 ) );
   static_assert( !internal::ranges_overlap( 21, 40, 10, 20 ) );

   static_assert( internal::ranges_overlap( 10, 20, 10, 10 ) );
   static_assert( internal::ranges_overlap( 10, 20, 20, 20 ) );
   static_assert( internal::ranges_overlap( 10, 20, 15, 20 ) );
   static_assert( internal::ranges_overlap( 10, 20, 10, 15 ) );
   static_assert( internal::ranges_overlap( 10, 30, 20, 40 ) );
   static_assert( internal::ranges_overlap( 20, 40, 10, 30 ) );
   static_assert( internal::ranges_overlap( 10, 30, 30, 40 ) );
   static_assert( internal::ranges_overlap( 30, 40, 10, 30 ) );

   static_assert( !internal::ranges_disjoint( 10, 20, 10, 10 ) );
   static_assert( !internal::ranges_disjoint( 10, 20, 20, 20 ) );
   static_assert( !internal::ranges_disjoint( 10, 20, 15, 20 ) );
   static_assert( !internal::ranges_disjoint( 10, 20, 10, 15 ) );
   static_assert( !internal::ranges_disjoint( 10, 30, 20, 40 ) );
   static_assert( !internal::ranges_disjoint( 20, 40, 10, 30 ) );
   static_assert( !internal::ranges_disjoint( 10, 30, 30, 40 ) );
   static_assert( !internal::ranges_disjoint( 30, 40, 10, 30 ) );

   return 0;
}
