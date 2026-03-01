// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct my_action
      : nothing< Rule >
   {};

   struct a
      : one< 'a' >
   {};

   struct ar
      : one< 'r' >
   {};

   struct a0
      : one< '0' >
   {};

   struct a0r
      : one< '1' >
   {};

   struct ab
      : one< 'A' >
   {};

   struct arb
      : one< 'R' >
   {};

   struct a0b
      : one< '2' >
   {};

   struct a0rb
      : one< '3' >
   {};

   struct af
      : one< 'F' >
   {};

   struct arf
      : one< 'G' >
   {};

   struct a0f
      : one< '4' >
   {};

   struct a0rf
      : one< '5' >
   {};

   std::string accumulator;

   template<>
   struct my_action< a >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/ ) noexcept
      {
         accumulator += 'a';
      }
   };

   template<>
   struct my_action< ar >
   {
      template< typename Rule, typename ActionInput >
      static void apply( const ActionInput& /*unused*/ ) noexcept
      {
         accumulator += 'r';
      }
   };

   template<>
   struct my_action< a0 >
   {
      static void apply0() noexcept
      {
         accumulator += '0';
      }
   };

   template<>
   struct my_action< a0r >
   {
      template< typename >
      static void apply0() noexcept
      {
         accumulator += '1';
      }
   };

   template<>
   struct my_action< ab >
   {
      template< typename ActionInput >
      static bool apply( const ActionInput& /*unused*/ ) noexcept
      {
         accumulator += 'A';
         return true;
      }
   };

   template<>
   struct my_action< arb >
   {
      template< typename Rule, typename ActionInput >
      static bool apply( const ActionInput& /*unused*/ ) noexcept
      {
         accumulator += 'R';
         return true;
      }
   };

   template<>
   struct my_action< a0b >
   {
      static bool apply0() noexcept
      {
         accumulator += '2';
         return true;
      }
   };

   template<>
   struct my_action< a0rb >
   {
      template< typename >
      static bool apply0() noexcept
      {
         accumulator += '3';
         return true;
      }
   };

   template<>
   struct my_action< af >
   {
      template< typename ActionInput >
      static bool apply( const ActionInput& /*unused*/ ) noexcept
      {
         accumulator += 'F';
         return false;
      }
   };

   template<>
   struct my_action< arf >
   {
      template< typename Rule, typename ActionInput >
      static bool apply( const ActionInput& /*unused*/ ) noexcept
      {
         accumulator += 'G';
         return false;
      }
   };

   template<>
   struct my_action< a0f >
   {
      static bool apply0() noexcept
      {
         accumulator += '4';
         return false;
      }
   };

   template<>
   struct my_action< a0rf >
   {
      template< typename >
      static bool apply0() noexcept
      {
         accumulator += '5';
         return false;
      }
   };

   using my_grammar = seq< a, ar, a0, a0r, ab, arb, a0b, a0rb, sor< af, one< 'F' > >, sor< arf, one< 'G' > >, sor< a0f, one< '4' > >, sor< a0rf, one< '5' > >, eof >;

   void unit_test()
   {
      const std::string str = "ar01AR23FG45";
      view_input in( str );
      const bool b = parse< my_grammar, my_action >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( accumulator == str );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
