// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_PARSE_MEMORY_HPP
#define TAOCPP_PEGTL_INCLUDE_PARSE_MEMORY_HPP

#include "apply_mode.hpp"
#include "config.hpp"
#include "eol.hpp"
#include "memory_input.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse.hpp"
#include "position_tracking.hpp"
#include "rewind_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                typename Eol = lf_crlf_eol,
                position_tracking P = position_tracking::IMMEDIATE,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_memory( basic_memory_input< Eol, P > in, States&&... st )
      {
         return parse< Rule, Action, Control, A, M >( in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_memory_nested( const Outer& oi, basic_memory_input< typename Outer::eol_t, Outer::position_tracking_v > in, States&&... st )
      {
         try {
            return parse< Rule, Action, Control, A, M >( in, st... );
         }
         catch( parse_error& e ) {
            e.positions.push_back( oi.position() );
            throw;
         }
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
