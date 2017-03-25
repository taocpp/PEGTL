// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include <tao/pegtl.hh>
#include <tao/pegtl/contrib/to_string.hh>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         TEST_ASSERT( to_string< string<> >() == "" );
         TEST_ASSERT(( to_string< string< 'a', 'b', 'c' > >() == "abc" ));

         TEST_ASSERT( to_string< istring<> >() == "" );
         TEST_ASSERT(( to_string< istring< 'a', 'b', 'c' > >() == "abc" ));

         TEST_ASSERT( to_string< TAOCPP_PEGTL_STRING( "" ) >() == "" );
         TEST_ASSERT( to_string< TAOCPP_PEGTL_STRING( "abc" ) >() == "abc" );
         TEST_ASSERT( to_string< TAOCPP_PEGTL_STRING( "AbC" ) >() == "AbC" );
         TEST_ASSERT( to_string< TAOCPP_PEGTL_STRING( "abc" ) >() != "AbC" );
         TEST_ASSERT( to_string< TAOCPP_PEGTL_ISTRING( "abc" ) >() == "abc" );
         TEST_ASSERT( to_string< TAOCPP_PEGTL_ISTRING( "AbC" ) >() == "AbC" );
         TEST_ASSERT( to_string< TAOCPP_PEGTL_ISTRING( "abc" ) >() != "AbC" );

         // to_string does *not* care about the outer class template
         TEST_ASSERT(( to_string< one< 'a', 'b', 'c' > >() == "abc" ));
      }

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#include "main.hh"
