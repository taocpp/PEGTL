// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TRACE_HPP
#define TAOCPP_PEGTL_INCLUDE_TRACE_HPP

#include <cassert>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

#include "config.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse.hpp"
#include "parse_memory.hpp"
#include "parse_stream.hpp"
#include "position_info.hpp"

#include "internal/demangle.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      struct trace_state
      {
         unsigned rule = 0;
         unsigned line = 0;
         std::vector< unsigned > stack;
      };

      template< typename Rule >
      struct tracer
         : normal< Rule >
      {
         template< typename Input, typename... States >
         static void start( const Input& in, States&&... )
         {
            std::cerr << in.position() << "  start  " << internal::demangle< Rule >() << std::endl;
         }

         template< typename Input >
         static void start( const Input& in, trace_state& ts )
         {
            std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ++ts.rule << " " << in.position() << "  start  " << internal::demangle< Rule >() << std::endl;
            ts.stack.push_back( ts.rule );
         }

         template< typename Input, typename... States >
         static void success( const Input& in, States&&... )
         {
            std::cerr << in.position() << " success " << internal::demangle< Rule >() << std::endl;
         }

         template< typename Input >
         static void success( const Input& in, trace_state& ts )
         {
            assert( !ts.stack.empty() );
            std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ts.stack.back() << " " << in.position() << " success " << internal::demangle< Rule >() << std::endl;
            ts.stack.pop_back();
         }

         template< typename Input, typename... States >
         static void failure( const Input& in, States&&... )
         {
            std::cerr << in.position() << " failure " << internal::demangle< Rule >() << std::endl;
         }

         template< typename Input >
         static void failure( const Input& in, trace_state& ts )
         {
            assert( !ts.stack.empty() );
            std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ts.stack.back() << " " << in.position() << " failure " << internal::demangle< Rule >() << std::endl;
            ts.stack.pop_back();
         }
      };

      template< typename Rule,
                template< typename... > class Action = nothing,
                typename Input,
                typename... States >
      bool trace( Input& in, States&&... st )
      {
         return parse< Rule, Action, tracer >( in, st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                typename... Args >
      bool trace_memory( Args&&... args )
      {
         return parse_memory< Rule, Action, tracer >( std::forward< Args >( args )... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                typename... Args >
      bool trace_string( Args&&... args )
      {
         return parse_string< Rule, Action, tracer >( std::forward< Args >( args )... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                typename... Args >
      bool trace_cstream( Args&&... args )
      {
         return parse_cstream< Rule, Action, tracer >( std::forward< Args >( args )... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                typename... Args >
      bool trace_istream( Args&&... args )
      {
         return parse_istream< Rule, Action, tracer >( std::forward< Args >( args )... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
