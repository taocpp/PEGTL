// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_PARSE_ARG_HPP
#define TAOCPP_PEGTL_INCLUDE_PARSE_ARG_HPP

#include <cassert>
#include <cstring>
#include <sstream>
#include <string>

#include "apply_mode.hpp"
#include "config.hpp"
#include "eol.hpp"
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
                typename Eol = lf_crlf_eol,
                position_tracking P = position_tracking::IMMEDIATE,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_arg( const int argn, char** argv, States&&... st )
      {
         std::ostringstream os;
         os << "argv[" << argn << ']';
         const std::string source = os.str();
         assert( argv[ argn ] );
         memory_input< Eol, P > in( argv[ argn ], argv[ argn ] + std::strlen( argv[ argn ] ), source.c_str() );
         return parse< Rule, Action, Control, A, M >( in, st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
