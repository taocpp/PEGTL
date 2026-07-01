// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_VERIFY_CTRL_HPP
#define TAO_PEGTL_SRC_TEST_VERIFY_CTRL_HPP

#include <cstddef>
#include <string_view>
#include <type_traits>
#include <utility>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE::test
{
   struct verify_ctrl_counts
   {
      std::size_t start = 0;
      std::size_t success = 0;
      std::size_t failure = 0;
      std::size_t apply = 0;
      std::size_t apply0 = 0;
   };

   template< typename Rule >
   struct verify_ctrl_state
      : verify_ctrl_counts
   {};

   [[nodiscard]] inline bool operator==( const verify_ctrl_counts& lhs, const verify_ctrl_counts& rhs ) noexcept
   {
      return ( lhs.start == rhs.start ) &&
             ( lhs.success == rhs.success ) &&
             ( lhs.failure == rhs.failure ) &&
             ( lhs.apply == rhs.apply ) &&
             ( lhs.apply0 == rhs.apply0 );
   }

   [[nodiscard]] inline bool operator!=( const verify_ctrl_counts& lhs, const verify_ctrl_counts& rhs ) noexcept
   {
      return !( lhs == rhs );
   }

   template< typename ControlRule >
   struct verify_ctrl_control
      : normal< ControlRule >
   {
      template< typename ParseInput, typename StateRule, typename... States >
      static void start( const ParseInput& /*unused*/, verify_ctrl_state< StateRule >& state, States&&... /*unused*/ ) noexcept
      {
         if constexpr( std::is_same_v< ControlRule, StateRule > ) {
            ++state.start;
         }
      }

      template< typename ParseInput, typename... States >
      static void start( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      template< typename ParseInput, typename StateRule, typename... States >
      static void success( const ParseInput& /*unused*/, verify_ctrl_state< StateRule >& state, States&&... /*unused*/ ) noexcept
      {
         if constexpr( std::is_same_v< ControlRule, StateRule > ) {
            ++state.success;
         }
      }

      template< typename ParseInput, typename... States >
      static void success( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      template< typename ParseInput, typename StateRule, typename... States >
      static void failure( const ParseInput& /*unused*/, verify_ctrl_state< StateRule >& state, States&&... /*unused*/ ) noexcept
      {
         if constexpr( std::is_same_v< ControlRule, StateRule > ) {
            ++state.failure;
         }
      }

      template< typename ParseInput, typename... States >
      static void failure( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      template< template< typename... > class Action,
                typename RewindPosition,
                typename ParseInput,
                typename StateRule,
                typename... States >
      static auto apply( const RewindPosition& begin, const ParseInput& in, verify_ctrl_state< StateRule >& state, States&&... st )
         noexcept( noexcept( normal< ControlRule >::template apply< Action >( begin, in, state, st... ) ) )
            -> decltype( normal< ControlRule >::template apply< Action >( begin, in, state, st... ) )
      {
         if constexpr( std::is_same_v< ControlRule, StateRule > ) {
            ++state.apply;
         }
         return normal< ControlRule >::template apply< Action >( begin, in, state, st... );
      }

      template< template< typename... > class Action,
                typename RewindPosition,
                typename ParseInput,
                typename... States >
      static auto apply( const RewindPosition& begin, const ParseInput& in, States&&... st )
         noexcept( noexcept( normal< ControlRule >::template apply< Action >( begin, in, st... ) ) )
            -> decltype( normal< ControlRule >::template apply< Action >( begin, in, st... ) )
      {
         return normal< ControlRule >::template apply< Action >( begin, in, st... );
      }

      template< template< typename... > class Action,
                typename ParseInput,
                typename StateRule,
                typename... States >
      static auto apply0( const ParseInput& in, verify_ctrl_state< StateRule >& state, States&&... st )
         noexcept( noexcept( normal< ControlRule >::template apply0< Action >( in, state, st... ) ) )
            -> decltype( normal< ControlRule >::template apply0< Action >( in, state, st... ) )
      {
         if constexpr( std::is_same_v< ControlRule, StateRule > ) {
            ++state.apply0;
         }
         return normal< ControlRule >::template apply0< Action >( in, state, st... );
      }

      template< template< typename... > class Action,
                typename ParseInput,
                typename... States >
      static auto apply0( const ParseInput& in, States&&... st )
         noexcept( noexcept( normal< ControlRule >::template apply0< Action >( in, st... ) ) )
            -> decltype( normal< ControlRule >::template apply0< Action >( in, st... ) )
      {
         return normal< ControlRule >::template apply0< Action >( in, st... );
      }
   };

   template< typename StateRule, typename Rule >
   struct verify_ctrl_apply_action
      : nothing< Rule >
   {};

   template< typename StateRule >
   struct verify_ctrl_apply_action< StateRule, StateRule >
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {}
   };

   template< typename StateRule, typename Rule >
   struct verify_ctrl_apply0_action
      : nothing< Rule >
   {};

   template< typename StateRule >
   struct verify_ctrl_apply0_action< StateRule, StateRule >
   {
      template< typename... States >
      static void apply0( States&&... /*unused*/ ) noexcept
      {}
   };

   template< typename StateRule, typename Rule >
   struct verify_ctrl_apply_false_action
      : nothing< Rule >
   {};

   template< typename StateRule >
   struct verify_ctrl_apply_false_action< StateRule, StateRule >
   {
      template< typename ActionInput, typename... States >
      [[nodiscard]] static bool apply( const ActionInput& /*unused*/, States&&... /*unused*/ ) noexcept
      {
         return false;
      }
   };

   template< typename StateRule, typename Rule >
   struct verify_ctrl_apply0_false_action
      : nothing< Rule >
   {};

   template< typename StateRule >
   struct verify_ctrl_apply0_false_action< StateRule, StateRule >
   {
      template< typename... States >
      [[nodiscard]] static bool apply0( States&&... /*unused*/ ) noexcept
      {
         return false;
      }
   };

   template< typename StateRule >
   struct verify_ctrl_apply
   {
      template< typename Rule >
      struct action
         : verify_ctrl_apply_action< StateRule, Rule >
      {};
   };

   template< typename StateRule >
   struct verify_ctrl_apply0
   {
      template< typename Rule >
      struct action
         : verify_ctrl_apply0_action< StateRule, Rule >
      {};
   };

   template< typename StateRule >
   struct verify_ctrl_apply_false
   {
      template< typename Rule >
      struct action
         : verify_ctrl_apply_false_action< StateRule, Rule >
      {};
   };

   template< typename StateRule >
   struct verify_ctrl_apply0_false
   {
      template< typename Rule >
      struct action
         : verify_ctrl_apply0_false_action< StateRule, Rule >
      {};
   };

   template< typename ParseInput >
   class verify_ctrl_rewind
   {
   public:
      explicit verify_ctrl_rewind( ParseInput& in ) noexcept
         : m_input( in ),
           m_saved( in.rewind_position() )
      {}

      verify_ctrl_rewind( verify_ctrl_rewind&& ) = delete;
      verify_ctrl_rewind( const verify_ctrl_rewind& ) = delete;

      void operator=( verify_ctrl_rewind&& ) = delete;
      void operator=( const verify_ctrl_rewind& ) = delete;

      ~verify_ctrl_rewind()
      {
         m_input.rewind_to_position( m_saved );
      }

   private:
      ParseInput& m_input;
      using position_t = std::decay_t< decltype( std::declval< ParseInput& >().rewind_position() ) >;
      position_t m_saved;
   };

   template< typename Rule >
   void verify_ctrl_check( const std::size_t line,
                           const char* file,
                           const char* label,
                           const bool result,
                           const bool expected_result,
                           const verify_ctrl_counts& received,
                           const verify_ctrl_counts& expected )
   {
      if( ( result == expected_result ) && ( received == expected ) ) {
         return;
      }
      TAO_PEGTL_TEST_FAILED( label << " result received/expected [ " << result << " / " << expected_result << " ]"
                             << " start received/expected [ " << received.start << " / " << expected.start << " ]"
                             << " success received/expected [ " << received.success << " / " << expected.success << " ]"
                             << " failure received/expected [ " << received.failure << " / " << expected.failure << " ]"
                             << " apply received/expected [ " << received.apply << " / " << expected.apply << " ]"
                             << " apply0 received/expected [ " << received.apply0 << " / " << expected.apply0 << " ]" );
   }

   template< typename Rule, template< typename... > class Action, typename ParseInput >
   void verify_ctrl_match( const std::size_t line,
                           const char* file,
                           const char* label,
                           ParseInput& in,
                           const bool expected_result,
                           const verify_ctrl_counts& expected )
   {
      verify_ctrl_state< Rule > state;
      const verify_ctrl_rewind< ParseInput > rewind( in );
      const bool result = parse< Rule, Action, verify_ctrl_control >( in, state );
      verify_ctrl_check< Rule >( line, file, label, result, expected_result, state, expected );
   }

}  // namespace TAO_PEGTL_NAMESPACE::test

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, typename ParseInput, std::enable_if_t< !std::is_convertible_v< std::decay_t< ParseInput >, std::string_view >, int > = 0 >
   void verify_ctrl_enabled( const std::size_t line, const char* file, ParseInput&& in, const bool expected_result = true )
   {
      using input_t = std::remove_reference_t< ParseInput >;
      static_assert( !std::is_const_v< input_t > );

      if( expected_result ) {
         test::verify_ctrl_match< Rule, nothing >( line, file, "plain enabled control", in, true, { 1, 1, 0, 0, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply< Rule >::template action >( line, file, "apply enabled control", in, true, { 1, 1, 0, 1, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply0< Rule >::template action >( line, file, "apply0 enabled control", in, true, { 1, 1, 0, 0, 1 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply_false< Rule >::template action >( line, file, "apply failure enabled control", in, false, { 1, 0, 1, 1, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply0_false< Rule >::template action >( line, file, "apply0 failure enabled control", in, false, { 1, 0, 1, 0, 1 } );
      }
      else {
         test::verify_ctrl_match< Rule, nothing >( line, file, "plain enabled control", in, false, { 1, 0, 1, 0, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply< Rule >::template action >( line, file, "apply enabled control", in, false, { 1, 0, 1, 0, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply0< Rule >::template action >( line, file, "apply0 enabled control", in, false, { 1, 0, 1, 0, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply_false< Rule >::template action >( line, file, "apply failure enabled control", in, false, { 1, 0, 1, 0, 0 } );
         test::verify_ctrl_match< Rule, test::verify_ctrl_apply0_false< Rule >::template action >( line, file, "apply0 failure enabled control", in, false, { 1, 0, 1, 0, 0 } );
      }
   }

   template< typename Rule >
   void verify_ctrl_enabled( const std::size_t line, const char* file, const std::string_view data, const bool expected_result = true )
   {
      text_view_input< scan::lf > in( data );
      verify_ctrl_enabled< Rule >( line, file, in, expected_result );
   }

   template< typename Rule, typename ParseInput, std::enable_if_t< !std::is_convertible_v< std::decay_t< ParseInput >, std::string_view >, int > = 0 >
   void verify_ctrl_disabled( const std::size_t line, const char* file, ParseInput&& in, const bool expected_result = true )
   {
      using input_t = std::remove_reference_t< ParseInput >;
      static_assert( !std::is_const_v< input_t > );

      test::verify_ctrl_match< Rule, nothing >( line, file, "plain disabled control", in, expected_result, { 0, 0, 0, 0, 0 } );
      test::verify_ctrl_match< Rule, test::verify_ctrl_apply< Rule >::template action >( line, file, "apply disabled control", in, expected_result, { 0, 0, 0, 0, 0 } );
      test::verify_ctrl_match< Rule, test::verify_ctrl_apply0< Rule >::template action >( line, file, "apply0 disabled control", in, expected_result, { 0, 0, 0, 0, 0 } );
      test::verify_ctrl_match< Rule, test::verify_ctrl_apply_false< Rule >::template action >( line, file, "apply failure disabled control", in, expected_result, { 0, 0, 0, 0, 0 } );
      test::verify_ctrl_match< Rule, test::verify_ctrl_apply0_false< Rule >::template action >( line, file, "apply0 failure disabled control", in, expected_result, { 0, 0, 0, 0, 0 } );
   }

   template< typename Rule >
   void verify_ctrl_disabled( const std::size_t line, const char* file, const std::string_view data, const bool expected_result = true )
   {
      text_view_input< scan::lf > in( data );
      verify_ctrl_disabled< Rule >( line, file, in, expected_result );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
