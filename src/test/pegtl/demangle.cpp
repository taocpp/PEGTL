// Copyright (c) 2017-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <string>

#include <tao/pegtl/demangle.hpp>

#define TAO_PEGTL_STRINGIFY( a ) TAO_PEGTL_STRINGIFY_IMPL( a )
#define TAO_PEGTL_STRINGIFY_IMPL( a ) #a

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   void test( const std::string& s )
   {
      std::cerr << s << std::endl;
      std::cerr << demangle< T >() << std::endl;
      TAO_PEGTL_TEST_ASSERT( demangle< T >() == s );
   }

   void unit_test()
   {
      const std::string ns = TAO_PEGTL_STRINGIFY( TAO_PEGTL_NAMESPACE );
#if !defined( __clang__ ) && defined( __GNUC__ ) && ( __GNUC__ == 9 ) && ( __GNUC_MINOR__ <= 2 )
      // see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91155
      test< int >( "i" );
      test< double >( "d" );
      if( ns == "tao::pegtl" ) {
         test< seq< bytes< 42 >, eof > >( "N3tao5pegtl3seqIJNS0_5bytesILj42EEENS0_3eofEEEE" );
      }
#elif defined( _MSC_VER ) && !defined( __clang__ )
      test< int >( "int" );
      test< double >( "double" );
      // in the Microsoft world, class and struct are not the same!
      test< seq< bytes< 42 >, eof > >( "struct " + ns + "::seq<struct " + ns + "::bytes<42>,struct " + ns + "::eof>" );
#else
      test< int >( "int" );
      test< double >( "double" );
      test< seq< bytes< 42 >, eof > >( ns + "::seq<" + ns + "::bytes<42>, " + ns + "::eof>" );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
