// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FUNCTION_HPP
#define TAO_PEGTL_INTERNAL_FUNCTION_HPP

#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Func, Func Tion, typename Peek >
   struct function;

   template< bool E, typename Input, bool ( *Tion )( Input& ) noexcept( E ) >
   struct function< bool ( * )( Input& ) noexcept( E ), Tion, void >
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
   struct function< bool ( * )( Input&, States... ) noexcept( E ), Tion, void >
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

   template< bool E, typename Data, bool ( *Tion )( Data ) noexcept( E ), typename Peek >
   struct function< bool ( * )( Data ) noexcept( E ), Tion, Peek >
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

   template< bool E, typename Data, typename... States, bool ( *Tion )( Data, States... ) noexcept( E ), typename Peek >
   struct function< bool ( * )( Data, States... ) noexcept( E ), Tion, Peek >
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

   template< typename Func, Func Tion, typename Peek >
   inline constexpr bool enable_control< function< Func, Tion, Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
