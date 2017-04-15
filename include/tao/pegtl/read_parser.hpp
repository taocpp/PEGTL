// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_READ_PARSER_HPP
#define TAOCPP_PEGTL_INCLUDE_READ_PARSER_HPP

#include <string>

#include "apply_mode.hpp"
#include "config.hpp"
#include "eol.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "position_tracking.hpp"
#include "rewind_mode.hpp"
#include "string_parser.hpp"

#include "internal/file_reader.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol = lf_crlf_eol, position_tracking P = position_tracking::IMMEDIATE >
      struct read_parser
         : string_parser< Eol, P >
      {
         explicit read_parser( const std::string& filename )
            : string_parser< Eol, P >( internal::file_reader( filename ).read(), filename )
         {
         }
      };

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                typename Eol = lf_crlf_eol,
                position_tracking P = position_tracking::IMMEDIATE,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_read( const std::string& filename, States&&... st )
      {
         return read_parser< Eol, P >( filename ).template parse< Rule, Action, Control, A, M >( st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_read_nested( Outer& oi, const std::string& filename, States&&... st )
      {
         return read_parser< typename Outer::eol_t, Outer::position_tracking_v >( filename ).template parse_nested< Rule, Action, Control, A, M >( oi, st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
