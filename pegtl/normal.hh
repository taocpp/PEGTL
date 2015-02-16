// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_NORMAL_HH
#define PEGTL_NORMAL_HH

#include "apply_mode.hh"
#include "error_mode.hh"

#include "parse_error.hh"

#include "internal/rule_match_one.hh"

namespace pegtl
{
   template< typename Rule >
   struct normal
   {
      template< typename Input, typename ... States >
      static void start( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void success( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void failure( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void raise( const Input & in, States && ... )
      {
         PEGTL_THROW_PARSE_ERROR( RuLe, in );
      }

      template< apply_mode A, error_mode E, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         return internal::rule_match_one< Rule, A, E, Action, Control >( in, st ... );
      }
   };

} // pegtl

#endif
