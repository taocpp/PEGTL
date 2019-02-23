// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

// Based on PR #150 from Paul Le Roux, see https://github.com/taocpp/PEGTL/pull/150

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename,
                   template< typename... >
                   class Base,
                   typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         struct has_match
            : std::false_type
         {};

         template< template< typename... >
                   class Base,
                   typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         struct has_match< decltype( (void)Action< Rule >::template match< Base, Rule, A, M, Action, Control >( std::declval< Input& >(), std::declval< States&& >()... ), void() ), Base, Rule, A, M, Action, Control, Input, States... >
            : std::true_type
         {};

      }  // namespace internal

      template< template< typename... > class Base >
      struct enable_action_match
      {
         template< class Rule >
         struct control : Base< Rule >
         {
            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control,
                      typename Input,
                      typename... States >
            [[nodiscard]] static bool match( Input& in, States&&... st )
            {
               if constexpr( internal::has_match< void, Base, Rule, A, M, Action, Control, Input, States... >::value ) {
                  return Action< Rule >::template match< Base, Rule, A, M, Action, Control >( in, st... );
               }
               else {
                  return Base< Rule >::template match< A, M, Action, Control >( in, st... );
               }
            }
         };
      };

      template< typename Rule >
      using action_match = enable_action_match< normal >::control< Rule >;

      template< template< typename... > class NewAction >
      struct change_action
      {
         template< template< typename... >
                   class Base,
                   typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            return Base< Rule >::template match< A, M, NewAction, Control >( in, st... );
         }
      };

      struct remove_state
      {
         template< template< typename... >
                   class Base,
                   typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... /*unused*/ )
         {
            return Base< Rule >::template match< A, M, Action, Control >( in );
         }
      };

      // further generic helpers could be build, e.g.
      //
      // - change_control
      // - remove prefix/suffix from input (e.g. remove surrounding quotes)
      // - replace the input completely?
      // - append states
      // - prepend states
      // - ...

      std::size_t global_state = 0;

      struct state_one
      {
         int byte_in_line_a;
         int byte_in_line_b;
      };

      // clang-format off
      struct grammar_inner : one< 'a' > {};
      struct grammar_one_c : seq< grammar_inner > {};
      struct grammar_one_b : seq< grammar_inner, grammar_one_c > {};
      struct grammar_one_a : seq< grammar_inner, grammar_one_b, eof > {};

      template< typename Rule >
      struct action_one_b : nothing< Rule > {};

      template< typename Rule >
      struct action_one_a : nothing< Rule > {};
      // clang-format on

      template<>
      struct action_one_b< grammar_one_c >
         : remove_state,
           nothing< grammar_one_c >  // TODO: Get rid of this!
      {};

      template<>
      struct action_one_b< grammar_inner >
      {
         template< typename Input >
         static void apply( const Input& /*unused*/ )
         {
            ++global_state;
         }

         template< typename Input >
         static void apply( const Input& in, state_one& state )
         {
            state.byte_in_line_b += in.input().byte();
         }
      };

      template<>
      struct action_one_a< grammar_one_b >
         : change_action< action_one_b >
      {};

      template<>
      struct action_one_a< grammar_inner >
      {
         template< typename Input >
         static void apply( const Input& in, state_one& state )
         {
            state.byte_in_line_a += in.input().byte();
         }
      };

      void unit_test()
      {
         state_one state{ 0, 0 };
         bool parse_result = parse< grammar_one_a, action_one_a, action_match >( memory_input( "aaa", __FUNCTION__ ), state );
         TAO_PEGTL_TEST_ASSERT( parse_result );
         TAO_PEGTL_TEST_ASSERT( state.byte_in_line_a == 1 );
         TAO_PEGTL_TEST_ASSERT( state.byte_in_line_b == 2 );
         TAO_PEGTL_TEST_ASSERT( global_state == 1 );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
