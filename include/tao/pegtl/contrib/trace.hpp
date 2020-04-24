// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_TRACE_HPP
#define TAO_PEGTL_CONTRIB_TRACE_HPP

#include <cassert>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../demangle.hpp"
#include "../normal.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename ParseInput >
      void print_current( const ParseInput& in )
      {
         if( in.empty() ) {
            std::cerr << "<eof>";
         }
         else {
            const auto c = in.peek_uint8();
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

   template< typename Rule, template< typename... > class Control >
   struct basic_trace_control
      : Control< Rule >
   {
      template< typename ParseInput, typename... States >
      static void start( const ParseInput& in, States&&... st )
      {
         std::cerr << in.position() << "  start  " << demangle< Rule >() << "; current ";
         print_current( in );
         std::cerr << std::endl;
         Control< Rule >::start( in, st... );
      }

      template< typename ParseInput, typename... States >
      static void start( const ParseInput& in, trace_state& ts, States&&... st )
      {
         std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ++ts.rule << " ";
         start( in, st... );
         ts.stack.push_back( ts.rule );
      }

      template< typename ParseInput, typename... States >
      static void success( const ParseInput& in, States&&... st )
      {
         std::cerr << in.position() << " success " << demangle< Rule >() << "; next ";
         print_current( in );
         std::cerr << std::endl;
         Control< Rule >::success( in, st... );
      }

      template< typename ParseInput, typename... States >
      static void success( const ParseInput& in, trace_state& ts, States&&... st )
      {
         assert( !ts.stack.empty() );
         std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ts.stack.back() << " ";
         success( in, st... );
         ts.stack.pop_back();
      }

      template< typename ParseInput, typename... States >
      static void failure( const ParseInput& in, States&&... st )
      {
         std::cerr << in.position() << " failure " << demangle< Rule >() << std::endl;
         Control< Rule >::failure( in, st... );
      }

      template< typename ParseInput, typename... States >
      static void failure( const ParseInput& in, trace_state& ts, States&&... st )
      {
         assert( !ts.stack.empty() );
         std::cerr << std::setw( 6 ) << ++ts.line << " " << std::setw( 6 ) << ts.stack.back() << " ";
         failure( in, st... );
         ts.stack.pop_back();
      }

      template< template< typename... > class Action, typename Iterator, typename ParseInput, typename... States >
      static auto apply( const Iterator& begin, const ParseInput& in, States&&... st )
         -> decltype( Control< Rule >::template apply< Action >( begin, in, st... ) )
      {
         std::cerr << in.position() << "  apply  " << demangle< Rule >() << std::endl;
         return Control< Rule >::template apply< Action >( begin, in, st... );
      }

      template< template< typename... > class Action, typename Iterator, typename ParseInput, typename... States >
      static auto apply( const Iterator& begin, const ParseInput& in, trace_state& ts, States&&... st )
         -> decltype( apply< Action >( begin, in, st... ) )
      {
         std::cerr << std::setw( 6 ) << ++ts.line << "        ";
         return apply< Action >( begin, in, st... );
      }

      template< template< typename... > class Action, typename ParseInput, typename... States >
      static auto apply0( const ParseInput& in, States&&... st )
         -> decltype( Control< Rule >::template apply0< Action >( in, st... ) )
      {
         std::cerr << in.position() << "  apply0 " << demangle< Rule >() << std::endl;
         return Control< Rule >::template apply0< Action >( in, st... );
      }

      template< template< typename... > class Action, typename ParseInput, typename... States >
      static auto apply0( const ParseInput& in, trace_state& ts, States&&... st )
         -> decltype( apply0< Action >( in, st... ) )
      {
         std::cerr << std::setw( 6 ) << ++ts.line << "        ";
         return apply0< Action >( in, st... );
      }
   };

   template< typename Rule >
   using trace_control = basic_trace_control< Rule, normal >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
