// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FUNCTION_HPP
#define TAO_PEGTL_INTERNAL_FUNCTION_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Func, Func Tion >
   struct function;

#if defined( __GNUC__ ) || defined( __clang__ )

   template< bool E, typename Input, bool ( *Tion )( Input& ) noexcept( E ) >
   struct function< void, bool ( * )( Input& ) noexcept( E ), Tion >
   {
      using rule_t = function;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( E )
      {
         return Tion( in );
      }
   };

   template< bool E, typename Input, typename... States, bool ( *Tion )( Input&, States... ) noexcept( E ) >
   struct function< void, bool ( * )( Input&, States... ) noexcept( E ), Tion >
   {
      using rule_t = function;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, States... st ) noexcept( E )
      {
         return Tion( in, st... );
      }
   };

   template< typename Peek, bool E, typename Data, bool ( *Tion )( Data ) noexcept( E ) >
   struct function< Peek, bool ( * )( Data ) noexcept( E ), Tion >
   {
      using peek_t = Peek;
      // using data_t = typename Peek::data_t;

      using rule_t = function;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Tion( t.data() ) ) {
               in.template consume< function >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek, bool E, typename Data, typename... States, bool ( *Tion )( Data, States... ) noexcept( E ) >
   struct function< Peek, bool ( * )( Data, States... ) noexcept( E ), Tion >
   {
      using peek_t = Peek;
      // using data_t = typename Peek::data_t;

      using rule_t = function;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, States... st )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Tion( t.data(), st... ) ) {
               in.template consume< function >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

#else

   template< typename Input, bool ( *Tion )( Input& ) noexcept( true ) >
   struct function< void, bool ( * )( Input& ) noexcept( true ), Tion >
   {
      using rule_t = function;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( true )
      {
         return Tion( in );
      }
   };

   template< typename Input, bool ( *Tion )( Input& ) noexcept( false ) >
   struct function< void, bool ( * )( Input& ) noexcept( false ), Tion >
   {
      using rule_t = function;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( false )
      {
         return Tion( in );
      }
   };

   template< typename Input, typename... States, bool ( *Tion )( Input&, States... ) noexcept( true ) >
   struct function< void, bool ( * )( Input&, States... ) noexcept( true ), Tion >
   {
      using rule_t = function;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, States... st ) noexcept( true )
      {
         return Tion( in, st... );
      }
   };

   template< typename Input, typename... States, bool ( *Tion )( Input&, States... ) noexcept( false ) >
   struct function< void, bool ( * )( Input&, States... ) noexcept( false ), Tion >
   {
      using rule_t = function;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, States... st ) noexcept( false )
      {
         return Tion( in, st... );
      }
   };

   template< typename Peek, typename Data, bool ( *Tion )( Data ) noexcept( true ) >
   struct function< Peek, bool ( * )( Data ) noexcept( true ), Tion >
   {
      using peek_t = Peek;
      // using data_t = typename Peek::data_t;

      using rule_t = function;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Tion( t.data() ) ) {
               in.template consume< function >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek, typename Data, bool ( *Tion )( Data ) noexcept( false ) >
   struct function< Peek, bool ( * )( Data ) noexcept( false ), Tion >
   {
      using peek_t = Peek;
      // using data_t = typename Peek::data_t;

      using rule_t = function;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Tion( t.data() ) ) {
               in.template consume< function >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek, typename Data, typename... States, bool ( *Tion )( Data, States... ) noexcept( true ) >
   struct function< Peek, bool ( * )( Data, States... ) noexcept( true ), Tion >
   {
      using peek_t = Peek;
      // using data_t = typename Peek::data_t;

      using rule_t = function;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, States... st )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Tion( t.data(), st... ) ) {
               in.template consume< function >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek, typename Data, typename... States, bool ( *Tion )( Data, States... ) noexcept( false ) >
   struct function< Peek, bool ( * )( Data, States... ) noexcept( false ), Tion >
   {
      using peek_t = Peek;
      // using data_t = typename Peek::data_t;

      using rule_t = function;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in, States... st )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( Tion( t.data(), st... ) ) {
               in.template consume< function >( t.size() );
               return true;
            }
         }
         return false;
      }
   };

#endif

   template< typename Peek, typename Func, Func Tion >
   inline constexpr bool enable_control< function< Peek, Func, Tion > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
