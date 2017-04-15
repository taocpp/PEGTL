// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_PARSE_STREAM_HPP
#define TAOCPP_PEGTL_INCLUDE_PARSE_STREAM_HPP

#include <cstdio>
#include <string>

#include "apply_mode.hpp"
#include "buffer_input.hpp"
#include "config.hpp"
#include "eol.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse.hpp"
#include "rewind_mode.hpp"

#include "internal/cstream_reader.hpp"
#include "internal/istream_reader.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                typename Eol = lf_crlf_eol,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_cstream( std::FILE* stream, const char* source, const std::size_t maximum, States&&... st )
      {
         buffer_input< internal::cstream_reader, Eol > in( source, maximum, stream );
         return parse< Rule, Action, Control, A, M >( in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                typename Eol = lf_crlf_eol,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_istream( std::istream& stream, const std::string& source, const std::size_t maximum, States&&... st )
      {
         buffer_input< internal::istream_reader, Eol > in( source.c_str(), maximum, stream );
         return parse< Rule, Action, Control, A, M >( in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_cstream_nested( const Outer& oi, std::FILE* stream, const char* source, const std::size_t maximum, States&&... st )
      {
         buffer_input< internal::cstream_reader, typename Outer::eol_t > in( source, maximum, stream );
         return parse_nested< Rule, Action, Control, A, M >( oi, in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_istream_nested( const Outer& oi, std::istream& stream, const std::string& source, const std::size_t maximum, States&&... st )
      {
         buffer_input< internal::istream_reader, typename Outer::eol_t > in( source.c_str(), maximum, stream );
         return parse_nested< Rule, Action, Control, A, M >( oi, in, st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
