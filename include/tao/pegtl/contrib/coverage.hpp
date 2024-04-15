// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_COVERAGE_HPP

#include <cstddef>
#include <map>
#include <string_view>
#include <vector>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../demangle.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../parse.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"
#include "../visit.hpp"

#include "../control/rewind_state_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct coverage_info
   {
      std::size_t start = 0;
      std::size_t success = 0;
      std::size_t failure = 0;
      std::size_t unwind = 0;
      std::size_t raise = 0;
      std::size_t raise_nested = 0;
   };

   struct rewind_coverage_info
   {
      std::size_t prep_rewind = 0;
      std::size_t will_rewind = 0;
      std::size_t wont_rewind = 0;
   };

   struct coverage_entry
      : coverage_info
   {
      std::map< std::string_view, coverage_info > branches;
   };

   struct coverage_result
   {
      std::map< std::string_view, coverage_entry > coverage;
      std::map< std::string_view, rewind_coverage_info > rewind;
   };

   namespace internal
   {
      template< typename Rule >
      struct coverage_insert
      {
         static void visit( std::map< std::string_view, coverage_entry >& map )
         {
            visit_branches( map.try_emplace( demangle< Rule >() ).first->second.branches, typename Rule::subs_t() );
         }

         template< typename... Ts >
         static void visit_branches( std::map< std::string_view, coverage_info >& branches, type_list< Ts... > /*unused*/ )
         {
            ( branches.try_emplace( demangle< Ts >() ), ... );
         }
      };

      struct coverage_state
      {
         template< typename Rule >
         static constexpr bool enable = true;

         explicit coverage_state( coverage_result& in_result )
            : result( in_result )
         {}

         coverage_result& result;
         std::vector< std::string_view > stack;

         template< typename Rule, typename ParseInput, typename... States >
         void start( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.coverage.at( name ).start;
            if( !stack.empty() ) {
               ++result.coverage.at( stack.back() ).branches.at( name ).start;
            }
            stack.push_back( name );
         }

         template< typename Rule, typename ParseInput, typename... States >
         void success( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            stack.pop_back();
            const auto name = demangle< Rule >();
            ++result.coverage.at( name ).success;
            if( !stack.empty() ) {
               ++result.coverage.at( stack.back() ).branches.at( name ).success;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void failure( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            stack.pop_back();
            const auto name = demangle< Rule >();
            ++result.coverage.at( name ).failure;
            if( !stack.empty() ) {
               ++result.coverage.at( stack.back() ).branches.at( name ).failure;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void prep_rewind( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.rewind[ name ].prep_rewind;
         }

         template< typename Rule, typename ParseInput, typename... States >
         void will_rewind( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.rewind[ name ].will_rewind;
         }

         template< typename Rule, typename ParseInput, typename... States >
         void wont_rewind( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.rewind[ name ].wont_rewind;
         }

         template< typename Rule, typename ParseInput, typename... States >
         void raise( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.coverage.at( name ).raise;
            if( !stack.empty() ) {
               ++result.coverage.at( stack.back() ).branches.at( name ).raise;
            }
         }

         template< typename Rule, typename Ambient, typename ParseInput, typename... States >
         void raise_nested( const Ambient& /*unused*/, const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.coverage.at( name ).raise_nested;
            if( !stack.empty() ) {
               ++result.coverage.at( stack.back() ).branches.at( name ).raise_nested;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void unwind( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            stack.pop_back();
            const auto name = demangle< Rule >();
            ++result.coverage.at( name ).unwind;
            if( !stack.empty() ) {
               ++result.coverage.at( stack.back() ).branches.at( name ).unwind;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void apply( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
         {}

         template< typename Rule, typename ParseInput, typename... States >
         void apply0( const ParseInput& /*unused*/, States&&... /*unused*/ ) noexcept
         {}
      };

   }  // namespace internal

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   bool coverage( ParseInput&& in, coverage_result& result, States&&... st )
   {
      internal::coverage_state state( result );
      visit< Rule, internal::coverage_insert >( state.result.coverage );  // Fill coverage map with all sub-rules of the grammar.
      return parse< Rule, Action, rewind_state_control< Control >::template type >( in, st..., state );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
