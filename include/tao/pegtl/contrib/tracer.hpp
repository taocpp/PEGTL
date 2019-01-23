// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_TRACER_HPP
#define TAO_PEGTL_CONTRIB_TRACER_HPP

#include <cassert>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../normal.hpp"

#include "../internal/demangle.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Input >
         void print_current( const Input& in )
         {
            if( in.empty() ) {
               std::cerr << "<eof>";
            }
            else {
               const auto c = in.peek_byte();
               switch( c ) {
                  case 0:
                     std::cerr << "<nul> = ";
                     break;
                  case 9:
                     std::cerr << "<ht> = ";
                     break;
                  case 10:
                     std::cerr << "<lf> = ";
                     break;
                  case 13:
                     std::cerr << "<cr> = ";
                     break;
                  default:
                     if( isprint( c ) ) {
                        std::cerr << '\'' << c << "' = ";
                     }
               }
               std::cerr << "(char)" << unsigned( c );
            }
         }

      }  // namespace internal

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
         static void start( const Input& in, States&&... /*unused*/ )
         {
            std::cerr << in.position() << "  start  " << internal::demangle< Rule >() << "; current ";
            print_current( in );
            std::cerr << std::endl;
         }

         template< typename Input >
         static void start( const Input& in, trace_state& ts )
         {
            std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ++ts.rule << " ";
            start( in );
            ts.stack.push_back( ts.rule );
         }

         template< typename Input, typename... States >
         static void success( const Input& in, States&&... /*unused*/ )
         {
            std::cerr << in.position() << " success " << internal::demangle< Rule >() << "; next ";
            print_current( in );
            std::cerr << std::endl;
         }

         template< typename Input >
         static void success( const Input& in, trace_state& ts )
         {
            assert( !ts.stack.empty() );
            std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ts.stack.back() << " ";
            success( in );
            ts.stack.pop_back();
         }

         template< typename Input, typename... States >
         static void failure( const Input& in, States&&... /*unused*/ )
         {
            std::cerr << in.position() << " failure " << internal::demangle< Rule >() << std::endl;
         }

         template< typename Input >
         static void failure( const Input& in, trace_state& ts )
         {
            assert( !ts.stack.empty() );
            std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ts.stack.back() << " ";
            failure( in );
            ts.stack.pop_back();
         }

         template< template< typename... > class Action, typename Input, typename... States >
         static auto apply0( const Input& /*unused*/, States&&... st )
            -> decltype( Action< Rule >::apply0( st... ) )
         {
            std::cerr << "apply0 " << internal::demangle< Action< Rule > >() << std::endl;
            return Action< Rule >::apply0( st... );
         }

         template< template< typename... > class Action, typename Input, typename... States >
         static auto apply0( const Input& /*unused*/, trace_state& ts, States&&... st )
            -> decltype( Action< Rule >::apply0( ts, st... ) )
         {
            std::cerr << std::setw( 6 ) << ++ts.line << "        " << internal::demangle< Action< Rule > >() << "::apply0()" << std::endl;
            return Action< Rule >::apply0( ts, st... );
         }

         template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
         static auto apply( const Iterator& begin, const Input& in, States&&... st )
            -> decltype( Action< Rule >::apply( std::declval< const typename Input::action_t& >(), st... ) )
         {
            std::cerr << "apply " << internal::demangle< Action< Rule > >() << std::endl;
            using action_t = typename Input::action_t;
            const action_t action_input( begin, in );
            return Action< Rule >::apply( action_input, st... );
         }

         template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
         static auto apply( const Iterator& begin, const Input& in, trace_state& ts, States&&... st )
            -> decltype( Action< Rule >::apply( std::declval< const typename Input::action_t& >(), ts, st... ) )
         {
            std::cerr << std::setw( 6 ) << ++ts.line << "        " << internal::demangle< Action< Rule > >() << "::apply()" << std::endl;
            using action_t = typename Input::action_t;
            const action_t action_input( begin, in );
            return Action< Rule >::apply( action_input, ts, st... );
         }
      };

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
