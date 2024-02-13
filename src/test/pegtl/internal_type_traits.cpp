// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/internal/type_traits.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   enum class test_enum
   {};

   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( internal::is_simple_type_v< int > );
      TAO_PEGTL_TEST_ASSERT( internal::is_simple_type_v< long > );
      TAO_PEGTL_TEST_ASSERT( internal::is_simple_type_v< char32_t > );
      TAO_PEGTL_TEST_ASSERT( internal::is_simple_type_v< std::byte > );
      TAO_PEGTL_TEST_ASSERT( internal::is_simple_type_v< test_enum > );

      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< int& > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< long& > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< char32_t& > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< std::byte& > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< test_enum& > );

      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< int* > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< long* > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< char32_t* > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< std::byte* > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< test_enum* > );

      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< void > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< float > );
      TAO_PEGTL_TEST_ASSERT( !internal::is_simple_type_v< std::string > );

      TAO_PEGTL_TEST_ASSERT( internal::is_simple_type_v< bool > );  // TODO?
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
