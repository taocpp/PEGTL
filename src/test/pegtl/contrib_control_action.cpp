// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <string>

#include "test.hpp"

#include <tao/pegtl/contrib/control_action.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   struct first_rule
      : sor< alpha, digit >
   {};

   struct second_rule
      : must< alnum >
   {};

   std::string story;

   template<>
   struct test_action< first_rule >
      : control_action
   {
      template< typename ParseInput >
      static void start( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'a';
      }

      template< typename ParseInput >
      static void success( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'b';
      }

      template< typename ParseInput >
      static void failure( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'c';
      }
   };

   template<>
   struct test_action< alpha >
      : control_action
   {
      template< typename ParseInput >
      static void start( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'd';
      }

      template< typename ParseInput >
      static void success( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'e';
      }
      template< typename ParseInput >
      static void failure( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'f';
      }
   };

   template<>
   struct test_action< digit >
      : control_action
   {
      template< typename ParseInput >
      static void start( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'g';
      }

      template< typename ParseInput >
      static void success( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'h';
      }
      template< typename ParseInput >
      static void failure( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'i';
      }
   };

   template<>
   struct test_action< second_rule >
      : control_action
   {
      template< typename ParseInput >
      static void start( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'j';
      }

      template< typename ParseInput >
      static void success( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'k';
      }
      template< typename ParseInput >
      static void failure( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'l';
      }

      template< typename ParseInput >
      static void unwind( const ParseInput& /*unused*/, int /*unused*/ )
      {
         story += 'm';
      }
   };

   void unit_test()
   {
      {
         memory_input in( "0", __FUNCTION__ );
         const auto b = parse< first_rule, test_action >( in, 42 );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( story == "adfghb" );
      }
      story.clear();
      {
         memory_input in( "*", __FUNCTION__ );
         TAO_PEGTL_TEST_THROWS( parse< second_rule, test_action >( in, 42 ) );
         TAO_PEGTL_TEST_ASSERT( story == "jm" );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
