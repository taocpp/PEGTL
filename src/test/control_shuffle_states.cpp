// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/control/shuffle_states.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static_assert( internal::reverse::value< 0, 5 > == 4 );
   static_assert( internal::reverse::value< 1, 5 > == 3 );
   static_assert( internal::reverse::value< 2, 5 > == 2 );
   static_assert( internal::reverse::value< 3, 5 > == 1 );
   static_assert( internal::reverse::value< 4, 5 > == 0 );

   static_assert( internal::rotate_left< 1 >::value< 0, 3 > == 1 );
   static_assert( internal::rotate_left< 1 >::value< 1, 3 > == 2 );
   static_assert( internal::rotate_left< 1 >::value< 2, 3 > == 0 );

   static_assert( internal::rotate_right< 1 >::value< 0, 3 > == 2 );
   static_assert( internal::rotate_right< 1 >::value< 1, 3 > == 0 );
   static_assert( internal::rotate_right< 1 >::value< 2, 3 > == 1 );

   static_assert( internal::rotate_left< 2 >::value< 0, 5 > == 2 );
   static_assert( internal::rotate_left< 2 >::value< 1, 5 > == 3 );
   static_assert( internal::rotate_left< 2 >::value< 2, 5 > == 4 );
   static_assert( internal::rotate_left< 2 >::value< 3, 5 > == 0 );
   static_assert( internal::rotate_left< 2 >::value< 4, 5 > == 1 );

   static_assert( internal::rotate_right< 2 >::value< 0, 5 > == 3 );
   static_assert( internal::rotate_right< 2 >::value< 1, 5 > == 4 );
   static_assert( internal::rotate_right< 2 >::value< 2, 5 > == 0 );
   static_assert( internal::rotate_right< 2 >::value< 3, 5 > == 1 );
   static_assert( internal::rotate_right< 2 >::value< 4, 5 > == 2 );

   struct foo
   {};

   struct bar
   {};

   struct baz
   {};

   template< std::size_t I, std::size_t S >
   inline constexpr std::size_t test_value = 42;

   template<>
   inline constexpr std::size_t test_value< 0, 4 > = 1;

   template<>
   inline constexpr std::size_t test_value< 1, 4 > = 3;

   template<>
   inline constexpr std::size_t test_value< 2, 4 > = 2;

   template<>
   inline constexpr std::size_t test_value< 3, 4 > = 0;

   struct test_shuffle
   {
      template< std::size_t I, std::size_t S >
      static constexpr std::size_t value = test_value< I, S >;
   };

   template< typename Rule >
   struct test_shuffled_control
   {
      static constexpr bool enable = internal::enable_control< Rule >;

      template< typename ParseInput >
      static void start( const ParseInput& in, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {
         normal< Rule >::start( in );
      }

      template< typename ParseInput >
      static void success( const ParseInput& in, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {
         normal< Rule >::success( in );
      }

      template< typename ParseInput >
      static void failure( const ParseInput& in, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {
         normal< Rule >::failure( in );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      static auto guard( ParseInput& in, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {
         return normal< Rule >::template guard< A, M, Action, Control >( in );
      }

      template< typename ParseInput >
      [[noreturn]] static void raise( const ParseInput& in, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {
         normal< Rule >::raise( in );
      }

      template< typename Ambient, typename ParseInput >
      [[noreturn]] static void raise( const Ambient& am, const ParseInput& in, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {
         normal< Rule >::raise_nested( am, in );
      }

      template< typename ParseInput >
      static void unwind( const ParseInput& /*unused*/, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ )
      {}

      template< template< typename... > class Action,
                typename RewindPosition,
                typename ParseInput >
      static auto apply( const RewindPosition& begin, const ParseInput& in, const foo& f, const baz& z, const bar& b, const int& i ) -> decltype( Action< Rule >::apply( std::declval< const action_input< ParseInput >& >(), std::declval< const foo& >(), std::declval< const baz& >(), std::declval< const bar& >(), std::declval< const int& >() ) )
      {
         return normal< Rule >::template apply< Action >( begin, in, f, z, b, i );
      }

      template< template< typename... > class Action,
                typename ParseInput >
      static auto apply0( const ParseInput& in, const foo& f, const baz& z, const bar& b, const int& i ) -> decltype( Action< Rule >::apply0( std::declval< const foo& >(), std::declval< const baz& >(), std::declval< const bar& >(), std::declval< const int& >() ) )
      {
         return normal< Rule >::template apply0< Action >( in, f, z, b, i );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, const int& i, const foo& f, const bar& b, const baz& z )
      {
         return normal< Rule >::template match< A, M, Action, Control >( in, i, f, b, z );
      }
   };

   template< typename Rule >
   struct test_shuffle_control
      : shuffle_states< test_shuffled_control< Rule >, test_shuffle >
   {};

   struct test_grammar
      : sor< one< 'a' >, seq< one< 'b' >, must< one< 'c' > > > > {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< one< 'b' > >
   {
      static void apply0( const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ ) noexcept
      {}
   };

   template<>
   struct test_action< one< 'c' > >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, const foo& /*unused*/, const baz& /*unused*/, const bar& /*unused*/, const int& /*unused*/ ) noexcept
      {}
   };

   void unit_test()
   {
      const int i = 42;
      const foo f;
      const bar b;
      const baz z;
      {
         view_input<> in( "bc" );
         const bool result = parse< test_grammar, test_action, test_shuffle_control >( in, i, f, b, z );
         TAO_PEGTL_TEST_ASSERT( result );
      }
      {
         view_input<> in( "bd" );
         TAO_PEGTL_TEST_THROWS( (void)parse< test_grammar, test_action, test_shuffle_control >( in, i, f, b, z ) );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
