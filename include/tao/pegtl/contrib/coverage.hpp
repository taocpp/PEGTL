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

#include "remove_first_state.hpp"
#include "shuffle_states.hpp"

#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../parse.hpp"
#include "../type_list.hpp"
#include "../visit.hpp"

#include "../internal/demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct coverage_info
   {
      std::size_t start = 0;
      std::size_t success = 0;
      std::size_t local_failure = 0;
      std::size_t global_failure = 0;
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
      struct coverage_state
      {
         coverage_result result;
         std::vector< std::string_view > stack;
      };

      template< typename Rule >
      struct coverage_insert
      {
         static void visit( coverage_state& state )
         {
            visit_branches( state.result.map.try_emplace( internal::demangle< Rule >() ).first->second.branches, typename Rule::subs_t() );
         }

         template< typename... Ts >
         static void visit_branches( std::map< std::string_view, coverage_info >& branches, type_list< Ts... > /*unused*/ )
         {
            ( branches.try_emplace( internal::demangle< Ts >() ), ... );
         }
      };

      template< template< typename... > class Control = normal >
      struct make_coverage_control
      {
         template< typename Rule >
         struct control
            : remove_first_state< Control< Rule > >
         {
            template< typename ParseInput, typename... States >
            [[noreturn]] static void raise( const ParseInput& in, coverage_state& state, States&&... st )
            {
               const auto name = internal::demangle< Rule >();
               ++state.result.map.at( name ).raise;
               if( state.stack.size() > 1 ) {
                  ++state.result.map.at( state.stack.at( state.stack.size() - 2 ) ).branches.at( name ).raise;
               }
               Control< Rule >::raise( in, st... );
            }

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control2,
                      typename ParseInput,
                      typename... States >
            [[nodiscard]] static bool match( ParseInput& in, States&&... st )
            {
               coverage_entry dummy;
               auto& state = std::get< sizeof...( st ) - 1 >( std::tie( st... ) );
               const auto name = internal::demangle< Rule >();
               auto& entry = state.result.map.at( name );
               auto& previous = state.stack.empty() ? dummy : state.result.map.at( state.stack.back() ).branches.at( name );
               ++entry.start;
               ++previous.start;
               state.stack.push_back( name );
               try {
                  const bool result = Control< Rule >::template match< A, M, Action, Control2 >( in, st... );
                  state.stack.pop_back();
                  if( result ) {
                     ++entry.success;
                     ++previous.success;
                  }
                  else {
                     ++entry.local_failure;
                     ++previous.local_failure;
                  }
                  return result;
               }
               catch( ... ) {
                  state.stack.pop_back();
                  ++entry.global_failure;
                  ++previous.global_failure;
                  throw;
               }
            }
         };

         template< typename Rule >
         using type = rotate_states_right< control< Rule > >;
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
      state.result.grammar = internal::demangle< Rule >();
      state.result.source = in.source();

      visit< Rule, internal::coverage_insert >( state );  // Fill state.result.map with all sub-rules of the grammar.
      state.result.result = parse< Rule, Action, internal::make_coverage_control<>::template type >( in, st..., state );
      assert( state.stack.empty() );

      return std::move( state.result );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
