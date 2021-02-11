// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_LIMIT_DEPTH_HPP
#define TAO_PEGTL_LIMIT_DEPTH_HPP

#include "apply_mode.hpp"
#include "config.hpp"
#include "match.hpp"
#include "nothing.hpp"
#include "rewind_mode.hpp"

#if defined( __cpp_exceptions )
#include "parse_error.hpp"
#else
#include <cstdio>
#include <exception>
#endif

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct depth_guard
      {
         std::size_t& m_depth;

         explicit depth_guard( std::size_t& depth ) noexcept
            : m_depth( depth )
         {
            ++m_depth;
         }

         depth_guard( const depth_guard& ) = delete;
         depth_guard( depth_guard&& ) = delete;

         ~depth_guard()
         {
            --m_depth;
         }

         depth_guard& operator=( const depth_guard& ) = delete;
         depth_guard& operator=( depth_guard&& ) = delete;
      };

   }  // namespace internal

   template< std::size_t Max >
   struct limit_depth
   {
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
         const internal::depth_guard dg( in.m_depth );
         if( in.m_depth > Max ) {
#if defined( __cpp_exceptions )
            throw TAO_PEGTL_NAMESPACE::parse_error( "maximum parser depth exceeded", in );
#else
            std::fputs( "maximum parser depth exceeded\n", stderr );
            std::terminate();
#endif
         }
         return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
