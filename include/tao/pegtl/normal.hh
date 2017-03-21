// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_NORMAL_HH
#define TAOCPP_PEGTL_INCLUDE_NORMAL_HH

#include "config.hh"
#include "count_data.hh"
#include "apply_mode.hh"
#include "rewind_mode.hh"
#include "parse_error.hh"

#include "internal/demangle.hh"
#include "internal/action_input.hh"
#include "internal/rule_match_one.hh"

namespace TAOCPP_PEGTL_NAMESPACE
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
         throw parse_error( "parse error matching " + internal::demangle< Rule >(), in );
      }

      template< typename Input, template< typename ... > class Action, typename ... States >
      static void apply( const count_data & begin, const count_data & end, const char * source, States && ... st )
      {
         const Input in( begin, end.data, source );
         Action< Rule >::apply( in, st ... );
      }

      template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
      static bool match( Input & in, States && ... st )
      {
         return internal::rule_match_one< Rule, A, M, Action, Control >::match( in, st ... );
      }
   };

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
