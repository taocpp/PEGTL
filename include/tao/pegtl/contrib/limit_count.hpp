// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_LIMIT_COUNT_HPP
#define TAO_PEGTL_CONTRIB_LIMIT_COUNT_HPP

#include <algorithm>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#if !defined( __cpp_exceptions )
#include <cstdio>
#include <exception>
#endif

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< std::size_t Maximum, typename MemoryInput >
      struct [[nodiscard]] bytes_guard
      {
         MemoryInput& m_in;
         const char* m_end;

         explicit bytes_guard( MemoryInput& in ) noexcept
            : m_in( in ),
              m_end( in.end() )
         {
            m_in.private_set_end( m_in.start() + ( std::min )( m_in.size(), Maximum ) );
         }

         bytes_guard( bytes_guard&& ) = delete;
         bytes_guard( const bytes_guard& ) = delete;

         ~bytes_guard()
         {
            m_in.private_set_end( m_end );
         }

         void operator=( bytes_guard&& ) = delete;
         void operator=( const bytes_guard& ) = delete;
      };

      // C++17 does not allow for partial deduction guides.

   }  // namespace internal

   template< std::size_t Maximum >
   struct limit_count
      : maybe_nothing
   {
      static constexpr const char* error_message = "maximum allowed rule consumption reached";

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         internal::bytes_guard< Maximum, ParseInput > bg( in );
         if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... ) ) {
            if( in.empty() && ( bg.m_end != in.current() ) ) {
#if defined( __cpp_exceptions )
               Control< limit_count >::raise( in );
#else
               std::fputs( "maximum allowed rule consumption reached\n", stderr );
               std::terminate();
#endif
            }
            return true;
         }
         return false;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
