// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_DUSELTRONIK_HPP
#define TAO_PEGTL_INTERNAL_DUSELTRONIK_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

#include "dusel_mode.hpp"
#include "has_unwind.hpp"

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control,
             dusel_mode >
   struct duseltronik;

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control,
             typename ParseInput,
             typename... States >
   [[nodiscard]] bool match( ParseInput& in, States&&... st )
   {
      if constexpr( has_unwind< Control< Rule >, void, const ParseInput&, States... > ) {
         try {
            return duseltronik< Rule, A, M, Action, Control, dusel_mode::nothing >::match( in, st... );
         }
         catch( ... ) {
            Control< Rule >::unwind( static_cast< const ParseInput& >( in ), st... );
            throw;
         }
      }
      else {
         return duseltronik< Rule, A, M, Action, Control, dusel_mode::nothing >::match( in, st... );
      }
   }

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control >
   struct duseltronik< Rule, A, M, Action, Control, dusel_mode::nothing >
   {
      template< typename ParseInput, typename... States >
      [[nodiscard]] static auto match( ParseInput& in, States&&... st )
         -> decltype( Rule::template match< A, M, Action, Control >( in, st... ) )
      {
         return Rule::template match< A, M, Action, Control >( in, st... );
      }

      template< typename ParseInput, typename... States, int = 1 >
      [[nodiscard]] static auto match( ParseInput& in, States&&... /*unused*/ )
         -> decltype( Rule::match( in ) )
      {
         return Rule::match( in );
      }
   };

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control >
   struct duseltronik< Rule, A, M, Action, Control, dusel_mode::control >
   {
      template< typename ParseInput, typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         Control< Rule >::start( static_cast< const ParseInput& >( in ), st... );
         if( internal::match< Rule, A, M, Action, Control >( in, st... ) ) {
            Control< Rule >::success( static_cast< const ParseInput& >( in ), st... );
            return true;
         }
         Control< Rule >::failure( static_cast< const ParseInput& >( in ), st... );
         return false;
      }
   };

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control >
   struct duseltronik< Rule, A, M, Action, Control, dusel_mode::control_and_apply_void >
   {
      template< typename ParseInput, typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         auto m = in.template mark< rewind_mode::required >();
         Control< Rule >::start( static_cast< const ParseInput& >( in ), st... );
         if( internal::match< Rule, A, rewind_mode::active, Action, Control >( in, st... ) ) {
            Control< Rule >::template apply< Action >( m.iterator(), static_cast< const ParseInput& >( in ), st... );
            Control< Rule >::success( static_cast< const ParseInput& >( in ), st... );
            return m( true );
         }
         Control< Rule >::failure( static_cast< const ParseInput& >( in ), st... );
         return false;
      }
   };

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control >
   struct duseltronik< Rule, A, M, Action, Control, dusel_mode::control_and_apply_bool >
   {
      template< typename ParseInput, typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         auto m = in.template mark< rewind_mode::required >();
         Control< Rule >::start( static_cast< const ParseInput& >( in ), st... );
         if( internal::match< Rule, A, rewind_mode::active, Action, Control >( in, st... ) ) {
            if( Control< Rule >::template apply< Action >( m.iterator(), static_cast< const ParseInput& >( in ), st... ) ) {
               Control< Rule >::success( static_cast< const ParseInput& >( in ), st... );
               return m( true );
            }
         }
         Control< Rule >::failure( static_cast< const ParseInput& >( in ), st... );
         return false;
      }
   };

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control >
   struct duseltronik< Rule, A, M, Action, Control, dusel_mode::control_and_apply0_void >
   {
      template< typename ParseInput, typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         Control< Rule >::start( static_cast< const ParseInput& >( in ), st... );
         if( internal::match< Rule, A, M, Action, Control >( in, st... ) ) {
            Control< Rule >::template apply0< Action >( static_cast< const ParseInput& >( in ), st... );
            Control< Rule >::success( static_cast< const ParseInput& >( in ), st... );
            return true;
         }
         Control< Rule >::failure( static_cast< const ParseInput& >( in ), st... );
         return false;
      }
   };

   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... >
             class Action,
             template< typename... >
             class Control >
   struct duseltronik< Rule, A, M, Action, Control, dusel_mode::control_and_apply0_bool >
   {
      template< typename ParseInput, typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         auto m = in.template mark< rewind_mode::required >();
         Control< Rule >::start( static_cast< const ParseInput& >( in ), st... );
         if( internal::match< Rule, A, rewind_mode::active, Action, Control >( in, st... ) ) {
            if( Control< Rule >::template apply0< Action >( static_cast< const ParseInput& >( in ), st... ) ) {
               Control< Rule >::success( static_cast< const ParseInput& >( in ), st... );
               return m( true );
            }
         }
         Control< Rule >::failure( static_cast< const ParseInput& >( in ), st... );
         return false;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#if defined( _MSC_VER )
#pragma warning( pop )
#endif

#endif
