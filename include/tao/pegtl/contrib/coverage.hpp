// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_COVERAGE_HPP

#include <cstddef>
#include <iostream>
#include <map>
#include <string_view>

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
      std::size_t failure = 0;
      std::size_t raise = 0;
      std::map< std::string_view, std::size_t > branches;
   };

   struct coverage_state
   {
      std::vector< std::string_view > stack;
      std::map< std::string_view, coverage_info > map;
   };

   template< typename Rule >
   struct coverage_insert
   {
      static void visit( coverage_state& state )
      {
         visit_branches( state.map.try_emplace( internal::demangle< Rule >() ).first->second.branches, typename Rule::subs_t() );
      }

      template< typename... Ts >
      static void visit_branches( std::map< std::string_view, std::size_t >& branches, type_list< Ts... > )
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
      static void start( const ParseInput& in, coverage_state& state )
      {
         auto name = internal::demangle< Rule >();
         state.stack.push_back( name );
         ++state.map.at( name ).start;
         Control< Rule >::start( in, state );
      }

      template< typename ParseInput >
      static void success( const ParseInput& in, coverage_state& state )
      {
         auto name = internal::demangle< Rule >();
         ++state.map.at( name ).success;
         state.stack.pop_back();
         if( !state.stack.empty() ) {
            ++state.map.at( state.stack.back() ).branches.at( name );
         }
         Control< Rule >::success( in, state );
      }

      template< typename ParseInput >
      static void failure( const ParseInput& in, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).failure;
         state.stack.pop_back();
         Control< Rule >::failure( in, state );
      }

      template< typename ParseInput >
      [[noreturn]] static void raise( const ParseInput& in, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).raise;
         state.stack.pop_back();
         Control< Rule >::raise( in, state );
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
                   << "      \"start\": " << v.start << ", \"success\": " << v.success << ", \"failure\": " << v.failure << ", \"raise\": " << v.raise << ",\n";
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
               std::cout << "        { \"branch\": \"" << k2 << "\", \"taken\": " << v2 << " }";
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
