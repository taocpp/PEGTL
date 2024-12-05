// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/action/change_control.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< template< typename... > class Control2 >
   struct test_control
   {
      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& /*unused*/ ) noexcept
      {
         return std::is_same_v< Control< eof >, Control2< eof > >;
      }
   };

   template< typename Rule >
   struct normal2
      : normal< Rule >
   {};

   template< typename Rule >
   struct action2
      : nothing< Rule >
   {};

   template<>
   struct action2< seq< one< 'a' >, test_control< normal2 > > >
      : change_control< normal2 >
   {};

   struct grammar
      : must< test_control< normal >, seq< one< 'a' >, test_control< normal2 > >, eof >
   {};

   void unit_test()
   {
      view_input<> in( "a" );
      const bool b = parse< grammar, action2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
