// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_COVERAGE_HPP

#include <cstddef>
#include <iostream>
#include <map>
#include <string_view>
#include <vector>

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

   struct coverage_state
   {
      std::vector< std::string_view > stack;
      std::map< std::string_view, coverage_entry > map;
   };

   template< typename Rule >
   struct coverage_insert
   {
      static void visit( coverage_state& state )
      {
         visit_branches( state.map.try_emplace( internal::demangle< Rule >() ).first->second.branches, typename Rule::subs_t() );
      }

      template< typename... Ts >
      static void visit_branches( std::map< std::string_view, coverage_info >& branches, type_list< Ts... > /*unused*/ )
      {
         ( branches.try_emplace( internal::demangle< Ts >() ), ... );
      }
   };

   template< typename Rule, template< typename... > class Control >
   struct basic_coverage_control
      : Control< Rule >
   {
      static constexpr bool enable = true;

      template< typename ParseInput >
      [[noreturn]] static void raise( const ParseInput& in, coverage_state& state )
      {
         auto name = internal::demangle< Rule >();
         ++state.map.at( name ).raise;
         if( state.stack.size() > 1 ) {
            ++state.map.at( state.stack.at( state.stack.size() - 2 ) ).branches.at( name ).raise;
         }
         Control< Rule >::raise( in, state );
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
         auto name = internal::demangle< Rule >();
         auto& entry = state.map.at( name );
         auto& previous = state.stack.empty() ? dummy : state.map.at( state.stack.back() ).branches.at( name );
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
   using coverage_control = basic_coverage_control< Rule, normal >;

   template< typename Grammar, typename ParseInput >
   bool coverage( ParseInput&& in )
   {
      coverage_state state;
      visit< Grammar, coverage_insert >( state );
      const auto result = parse< Grammar, nothing, coverage_control >( in, state );
      std::cout << "{\n"
                << "  \"grammar\": \"" << internal::demangle< Grammar >() << "\",\n"
                << "  \"source\": \"" << in.source() << "\",\n"
                << "  \"result\": " << ( result ? "true" : "false" ) << ",\n"
                << "  \"coverage\":\n"
                << "  [\n";
      bool f = true;
      for( const auto& [ k, v ] : state.map ) {
         if( f ) {
            f = false;
         }
         else {
            std::cout << ",\n";
         }
         std::cout << "    {\n"
                   << "      \"rule\": \"" << k << "\",\n"
                   << "      \"start\": " << v.start << ", \"success\": " << v.success << ", \"local_failure\": " << v.local_failure << ", \"global_failure\": " << v.global_failure << ", \"raise\": " << v.raise << ",\n";
         if( v.branches.empty() ) {
            std::cout << "      \"branches\": []\n";
         }
         else {
            std::cout << "      \"branches\": [\n";
            bool f2 = true;
            for( const auto& [ k2, v2 ] : v.branches ) {
               if( f2 ) {
                  f2 = false;
               }
               else {
                  std::cout << ",\n";
               }
               std::cout << "        { \"branch\": \"" << k2 << "\", \"start\": " << v2.start << ", \"success\": " << v2.success << ", \"local_failure\": " << v2.local_failure << ", \"global_failure\": " << v2.global_failure << ", \"raise\": " << v2.raise << " }";
            }
            std::cout << "\n      ]\n";
         }
         std::cout << "    }";
      }
      std::cout << "\n"
                << "  ]\n"
                << "}" << std::endl;
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
