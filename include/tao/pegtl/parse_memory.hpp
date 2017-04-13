// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_PARSE_MEMORY_HPP
#define TAOCPP_PEGTL_INCLUDE_PARSE_MEMORY_HPP

#include <cstddef>
#include <string>

#include "apply_mode.hpp"
#include "config.hpp"
#include "memory_input.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse.hpp"
#include "rewind_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_memory( const char* data, const char* dend, const char* source, States&&... st )
      {
         memory_input< position_tracking::IMMEDIATE > in( data, dend, source );
         return parse< Rule, Action, Control, A, M >( in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_memory( const char* data, const std::size_t size, const char* source, States&&... st )
      {
         return parse_memory< Rule, Action, Control, A, M >( data, data + size, source, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_string( const std::string& data, const std::string& source, States&&... st )
      {
         return parse_memory< Rule, Action, Control, A, M >( data.data(), data.data() + data.size(), source.c_str(), st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_memory_nested( const Outer& oi, const char* data, const char* dend, const char* source, States&&... st )
      {
         basic_memory_input< typename Outer::eol_t, position_tracking::IMMEDIATE > in( data, dend, source );
         return parse_nested< Rule, Action, Control, A, M >( oi, in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_memory_nested( const Outer& oi, const char* data, const std::size_t size, const char* source, States&&... st )
      {
         return parse_memory_nested< Rule, Action, Control, A, M >( oi, data, data + size, source, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_string_nested( const Outer& oi, const std::string& data, const std::string& source, States&&... st )
      {
         return parse_memory_nested< Rule, Action, Control, A, M >( oi, data.data(), data.data() + data.size(), source.c_str(), st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
