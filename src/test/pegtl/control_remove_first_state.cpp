// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/control/remove_first_state.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< one< 'F' > >
   {
      static void apply0( unsigned& u ) noexcept
      {
         u |= 1;
      }
   };

   template<>
   struct test_action< eof >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, unsigned& u ) noexcept
      {
         u |= 2;
      }
   };

   template< typename Rule >
   struct control_impl
      : normal< Rule >
   {
      template< typename ParseInput >
      static void start( ParseInput& /*unused*/, unsigned& u ) noexcept
      {
         u |= 64;
      }

      template< typename ParseInput >
      static void success( ParseInput& /*unused*/, unsigned& u ) noexcept
      {
         u |= 32;
      }

      template< typename ParseInput >
      static void failure( ParseInput& /*unused*/, unsigned& u ) noexcept
      {
         u |= 16;
      }

#if defined( __cpp_exceptions )
      template< typename ParseInput >
      [[noreturn]] static void raise( ParseInput& in, unsigned& u )
      {
         u |= 4;
         throw_parse_error( "raise", in );
      }

      template< typename ParseInput >
      static void unwind( ParseInput& /*unused*/, unsigned& u ) noexcept
      {
         u |= 8;
      }
#endif
   };

   template< typename Rule >
   struct test_control
      : remove_first_state< control_impl< Rule > >
   {};

#if defined( __cpp_exceptions )
   struct test_rule
      : seq< sor< try_catch_return_false< must< one< 'a' > > >, one< 'F' > >, eof >
   {};
#else
   struct test_rule
      : seq< sor< one< 'a' >, one< 'F' > >, eof >
   {};
#endif

   void unit_test()
   {
      unsigned u = 0;
      const std::string d = "F";
      text_view_input< scan::lf > in( d );
      TAO_PEGTL_TEST_ASSERT( parse< test_rule, test_action, test_control >( in, d, u ) );
#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_ASSERT( u == 127 );
#else
      TAO_PEGTL_TEST_ASSERT( u == 115 );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
