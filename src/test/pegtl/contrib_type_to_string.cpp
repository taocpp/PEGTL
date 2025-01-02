// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/type_to_string.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( type_to_string<>().empty() );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string< 'a', 'b', 'c' >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string< string<> >().empty() );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string< string< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string< istring<> >().empty() );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string< istring< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_STRING( "" ) >().empty() );
      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_STRING( "abc" ) >() == "abc" );
      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_STRING( "AbC" ) >() == "AbC" );
      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_STRING( "abc" ) >() != "AbC" );
      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_ISTRING( "abc" ) >() == "abc" );
      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_ISTRING( "AbC" ) >() == "AbC" );
      TAO_PEGTL_TEST_ASSERT( type_to_string< TAO_PEGTL_ISTRING( "abc" ) >() != "AbC" );

      TAO_PEGTL_TEST_ASSERT( ( type_to_string< one< 'a', 'b', 'c' > >() == "abc" ) );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string< not_one7< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string_view<>().empty() );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string_view< 'a', 'b', 'c' >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string_view< string<> >().empty() );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string_view< string< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string_view< istring<> >().empty() );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string_view< istring< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_STRING( "" ) >().empty() );
      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_STRING( "abc" ) >() == "abc" );
      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_STRING( "AbC" ) >() == "AbC" );
      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_STRING( "abc" ) >() != "AbC" );
      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_ISTRING( "abc" ) >() == "abc" );
      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_ISTRING( "AbC" ) >() == "AbC" );
      TAO_PEGTL_TEST_ASSERT( type_to_string_view< TAO_PEGTL_ISTRING( "abc" ) >() != "AbC" );

      TAO_PEGTL_TEST_ASSERT( ( type_to_string_view< one< 'a', 'b', 'c' > >() == "abc" ) );
      TAO_PEGTL_TEST_ASSERT( ( type_to_string_view< not_one7< 'a', 'b', 'c' > >() == "abc" ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
