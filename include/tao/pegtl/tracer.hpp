// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TRACER_HPP
#define TAOCPP_PEGTL_INCLUDE_TRACER_HPP

#include <cassert>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

#include "config.hpp"
#include "normal.hpp"

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

         template< template< typename... > class Action, typename... States >
         static void apply0( States&&... st )
         {
            std::cerr << "apply0 " << internal::demangle< Action< Rule > >() << std::endl;
            Action< Rule >::apply0( st... );
         }

         template< template< typename... > class Action >
         static void apply0( trace_state& ts )
         {
            std::cerr << std::setw( 6 ) << ++ts.line << "        " << internal::demangle< Action< Rule > >() << "::apply0()" << std::endl;
            Action< Rule >::apply0( ts );
         }

         template< typename Input, template< typename... > class Action, typename Iterator, typename... States >
         static void apply( const Iterator begin, const Iterator end, const char* source, States&&... st )
         {
            std::cerr << "apply " << internal::demangle< Action< Rule > >() << std::endl;
            const Input in( begin, end, source );
            Action< Rule >::apply( in, st... );
         }

         template< typename Input, template< typename... > class Action, typename Iterator >
         static void apply( const Iterator begin, const Iterator end, const char* source, trace_state& ts )
         {
            std::cerr << std::setw( 6 ) << ++ts.line << "        " << internal::demangle< Action< Rule > >() << "::apply()" << std::endl;
            const Input in( begin, end, source );
            Action< Rule >::apply( in, ts );
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
