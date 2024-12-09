// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#include <iostream>
int main()
{
   std::cout << "Exception and/or RTTI support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"

#include <string>
#include <utility>
#include <vector>

#include <tao/pegtl/contrib/nested_exceptions.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   const std::string s1 = "foo";
   const std::string s2 = "bar";

   struct visitor
   {
      void operator()( const std::string& s, const std::size_t l )
      {
         v.emplace_back( std::string( "string%" ) + s, l );
      }

      void operator()( const std::exception& e, const std::size_t l )
      {
         v.emplace_back( std::string( "exception%" ) + e.what(), l );
      }

      void operator()( const std::runtime_error& e, const std::size_t l )
      {
         v.emplace_back( std::string( "runtime%" ) + e.what(), l );
      }

      std::vector< std::pair< std::string, std::size_t > > v;
   };

   void test1()
   {
      try {
         throw std::string( s1 );
      }
      catch( ... ) {
         visitor v;
         inspect< std::string >( v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "string%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 0 );
      }
   }

   void test2()
   {
      try {
         throw std::string( s1 );
      }
      catch( const std::string& s ) {
         visitor v;
         inspect< std::string >( s, v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "string%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 0 );
      }
   }

   void test3()
   {
      try {
         try {
            throw std::string( s1 );
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( ... ) {
         visitor v;
         inspect< std::string, std::runtime_error >( v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "string%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].first == "runtime%bar" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].second == 0 );
      }
   }

   void test4()
   {
      try {
         try {
            throw std::logic_error( s1 );
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( ... ) {
         visitor v;
         inspect< std::exception, std::runtime_error >( v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "exception%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].first == "runtime%bar" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].second == 0 );
      }
   }

   void test5()
   {
      try {
         try {
            throw std::logic_error( s1 );
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( ... ) {
         visitor v;
         inspect< std::runtime_error, std::exception >( v );  // Testing WRONG ORDER of exception classes!
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "exception%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].first == "exception%bar" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].second == 0 );
      }
   }

   void test6()
   {
      try {
         try {
            try {
               throw std::string( "yo" );
            }
            catch( ... ) {
               std::throw_with_nested( std::logic_error( s1 ) );
            }
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( ... ) {
         visitor v;
         inspect< std::string, std::exception, std::runtime_error >( v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 3 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "string%yo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 2 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].first == "exception%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].second == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 2 ].first == "runtime%bar" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 2 ].second == 0 );
      }
   }

   void test7()
   {
      try {
         try {
            try {
               throw std::string( "yo" );
            }
            catch( ... ) {
               std::throw_with_nested( std::logic_error( s1 ) );
            }
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( const std::exception& e ) {
         visitor v;
         inspect< std::string, std::exception, std::runtime_error >( e, v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 3 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "string%yo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 2 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].first == "exception%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].second == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 2 ].first == "exception%bar" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 2 ].second == 0 );
      }
   }

   [[nodiscard]] inline bool operator==( const parse_error_base& l, const parse_error_base& r ) noexcept
   {
      return ( l.message() == r.message() ) && ( l.position_string() == r.position_string() );
   }

   void test8()
   {
      try {
         try {
            try {
               throw_parse_error( "first", count_position( 1 ) );
            }
            catch( ... ) {
               throw_parse_error_with_nested( "second", count_position( 2 ) );
            }
         }
         catch( ... ) {
            throw_parse_error_with_nested( "third", count_position( 3 ) );
         }
      }
      catch( const parse_error_base& e ) {
         const auto v1 = flatten();
         const auto v2 = flatten( e );
         TAO_PEGTL_TEST_ASSERT( v1 == v2 );
         TAO_PEGTL_TEST_ASSERT( v1.size() == 3 );
         TAO_PEGTL_TEST_ASSERT( v1[ 0 ].message() == "first" );
         TAO_PEGTL_TEST_ASSERT( v1[ 1 ].message() == "second" );
         TAO_PEGTL_TEST_ASSERT( v1[ 2 ].message() == "third" );
      }
   }

   void unit_test()
   {
      test1();
      test2();
      test3();
      test4();
      test5();
      test6();
      test7();
      test8();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
