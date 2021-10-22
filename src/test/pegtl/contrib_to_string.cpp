// Copyright (c) 2017-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/to_string.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( to_string< string<> >().empty() );
      TAO_PEGTL_TEST_ASSERT( ( to_string< string< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( to_string< istring<> >().empty() );
      TAO_PEGTL_TEST_ASSERT( ( to_string< istring< 'a', 'b', 'c' > >() == "abc" ) );

      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_STRING( "" ) >().empty() );
      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_STRING( "abc" ) >() == "abc" );
      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_STRING( "AbC" ) >() == "AbC" );
      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_STRING( "abc" ) >() != "AbC" );
      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_ISTRING( "abc" ) >() == "abc" );
      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_ISTRING( "AbC" ) >() == "AbC" );
      TAO_PEGTL_TEST_ASSERT( to_string< TAO_PEGTL_ISTRING( "abc" ) >() != "AbC" );

      // to_string does *not* care about the outer class template
      TAO_PEGTL_TEST_ASSERT( ( to_string< one< 'a', 'b', 'c' > >() == "abc" ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
