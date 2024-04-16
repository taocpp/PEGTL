// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_TRACE_STATE_HPP
#define TAO_PEGTL_DEBUG_TRACE_STATE_HPP

#include <ostream>

#include "../config.hpp"

#include "trace_struct.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename TraceTraits, typename ParseInput >
   struct trace_state
   {
      template< typename Rule >
      static constexpr bool enable = TraceTraits::template enable< Rule >;

      trace_struct< TraceTraits, typename ParseInput::error_position_t > state;

      trace_state( std::ostream& os, const ParseInput& in )
         : state( os, in.current_position() )
      {}

      template< typename Rule, typename... States >
      void start( const ParseInput& /*unused*/, States&&... /*unused*/ )
      {
         state.template start< Rule >();
      }

      template< typename Rule, typename... States >
      void success( const ParseInput& in, States&&... /*unused*/ )
      {
         state.template success< Rule >( in.current_position() );
      }

      template< typename Rule, typename... States >
      void failure( const ParseInput& in, States&&... /*unused*/ )
      {
         state.template failure< Rule >( in.current_position() );
      }

      template< typename Rule, typename... States >
      void prep_rewind( const ParseInput& in, States&&... /*unused*/ )
      {
         state.template prep_rewind< Rule >( in.rewind_position() );
      }

      template< typename Rule, typename... States >
      void will_rewind( const ParseInput& in, States&&... /*unused*/ )
      {
         state.template will_rewind< Rule >( in.rewind_position() );
      }

      template< typename Rule, typename... States >
      void wont_rewind( const ParseInput& in, States&&... /*unused*/ )
      {
         state.template wont_rewind< Rule >( in.rewind_position() );
      }

      template< typename Rule, typename... States >
      void raise( const ParseInput& /*unused*/, States&&... /*unused*/ )
      {
         state.template raise< Rule >();
      }

      template< typename Rule, typename Ambient, typename... States >
      void raise_nested( const Ambient& /*unused*/, const ParseInput& /*unused*/, States&&... /*unused*/ )
      {
         state.template raise_nested< Rule >();
      }

      template< typename Rule, typename... States >
      void unwind( const ParseInput& in, States&&... /*unused*/ )
      {
         state.template unwind< Rule >( in.current_position() );
      }

      template< typename Rule, typename... States >
      void apply( const ParseInput& /*unused*/, States&&... /*unused*/ )
      {
         state.template apply< Rule >();
      }

      template< typename Rule, typename... States >
      void apply0( const ParseInput& /*unused*/, States&&... /*unused*/ )
      {
         state.template apply0< Rule >();
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
