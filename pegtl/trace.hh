// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_TRACE_HH
#define PEGTL_TRACE_HH

#include <utility>
#include <iostream>

#include "parse.hh"
#include "normal.hh"
#include "nothing.hh"
#include "position_info.hh"

#include "internal/demangle.hh"

namespace pegtl
{
   template< typename Rule >
   struct tracer
         : normal< Rule >
   {
      template< typename Input, typename ... States >
      static void start( const Input & in, States && ... )
      {
         std::cerr << pegtl::position_info( in ) << "  start  " << internal::demangle< Rule >() << std::endl;
      }

      template< typename Input, typename ... States >
      static void success( const Input & in, States && ... )
      {
         std::cerr << pegtl::position_info( in ) << " success " << internal::demangle< Rule >() << std::endl;
      }

      template< typename Input, typename ... States >
      static void failure( const Input & in, States && ... )
      {
         std::cerr << pegtl::position_info( in ) << " failure " << internal::demangle< Rule >() << std::endl;
      }
   };

   template< typename Rule, template< typename ... > class Action = nothing, typename Input, typename ... States >
   bool trace_input( Input & in, States && ... st )
   {
      return parse_input< Rule, Action, tracer >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_arg( Args && ... args )
   {
      return parse_arg< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_memory( Args && ... args )
   {
      return parse_memory< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_string( Args && ... args )
   {
      return parse_string< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_cstream( Args && ... args )
   {
      return parse_cstream< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_stdin( Args && ... args )
   {
      return parse_stdin< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_cstring( Args && ... args )
   {
      return parse_cstring< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, typename ... Args >
   bool trace_istream( Args && ... args )
   {
      return parse_istream< Rule, Action, tracer >( std::forward< Args >( args ) ... );
   }

} // pegtl

#endif
