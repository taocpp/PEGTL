// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_COVERAGE_HPP

#include <cassert>
#include <cstddef>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "state_control.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../demangle.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../parse.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"
#include "../visit.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct coverage_info
   {
      std::size_t start = 0;
      std::size_t success = 0;
      std::size_t failure = 0;
      std::size_t unwind = 0;
      std::size_t raise = 0;
   };

   struct coverage_entry
      : coverage_info
   {
      std::map< std::string_view, coverage_info > branches;
   };

   struct coverage_result
   {
      std::string_view grammar;
      std::string source;

      std::map< std::string_view, coverage_entry > map;
      bool result = false;
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

         coverage_result result;
         std::vector< std::string_view > stack;

         template< typename Rule, typename ParseInput, typename... States >
         void start( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.map.at( name ).start;
            if( !stack.empty() ) {
               ++result.map.at( stack.back() ).branches.at( name ).start;
            }
            stack.push_back( name );
         }

         template< typename Rule, typename ParseInput, typename... States >
         void success( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            stack.pop_back();
            const auto name = demangle< Rule >();
            ++result.map.at( name ).success;
            if( !stack.empty() ) {
               ++result.map.at( stack.back() ).branches.at( name ).success;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void failure( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            stack.pop_back();
            const auto name = demangle< Rule >();
            ++result.map.at( name ).failure;
            if( !stack.empty() ) {
               ++result.map.at( stack.back() ).branches.at( name ).failure;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void raise( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            const auto name = demangle< Rule >();
            ++result.map.at( name ).raise;
            if( !stack.empty() ) {
               ++result.map.at( stack.back() ).branches.at( name ).raise;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void unwind( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {
            stack.pop_back();
            const auto name = demangle< Rule >();
            ++result.map.at( name ).unwind;
            if( !stack.empty() ) {
               ++result.map.at( stack.back() ).branches.at( name ).unwind;
            }
         }

         template< typename Rule, typename ParseInput, typename... States >
         void apply( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {}

         template< typename Rule, typename ParseInput, typename... States >
         void apply0( const ParseInput& /*unused*/, States&&... /*unused*/ )
         {}
      };

   }  // namespace internal

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   coverage_result coverage( ParseInput&& in, States&&... st )
   {
      internal::coverage_state state;
      state.result.grammar = demangle< Rule >();
      state.result.source = in.source();

      visit< Rule, internal::coverage_insert >( state.result.map );  // Fill map with all sub-rules of the grammar.
      state.result.result = parse< Rule, Action, state_control< Control >::template type >( in, st..., state );
      assert( state.stack.empty() );

      return std::move( state.result );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
