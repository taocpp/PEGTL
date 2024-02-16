// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/internal/value_list_concat.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< int... >
   class int_list
   {};

   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::value_list_concat_t< int_list<> >, int_list<> > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::value_list_concat_t< int_list< 42 > >, int_list< 42 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::value_list_concat_t< int_list< 1, 2, 3 > >, int_list< 1, 2, 3 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::value_list_concat_t< int_list<>, int_list< 42 > >, int_list< 42 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::value_list_concat_t< int_list< 42 >, int_list<> >, int_list< 42 > > );
      TAO_PEGTL_TEST_ASSERT( std::is_same_v< internal::value_list_concat_t< int_list< 1, 2, 3 >, int_list<>, int_list< 4, 5, 6 >, int_list< 7 > >, int_list< 1, 2, 3, 4, 5, 6, 7 > > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
