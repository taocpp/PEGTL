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
   };

   struct coverage_state
   {
      std::map< std::string_view, coverage_info > map;
   };

   template< typename Rule >
   struct coverage_insert
   {
      static void visit( coverage_state& state )
      {
         state.map.try_emplace( internal::demangle< Rule >() );
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
         ++state.map.at( internal::demangle< Rule >() ).start;
         Control< Rule >::start( in, state );
      }

      template< typename ParseInput >
      static void success( const ParseInput& in, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).success;
         Control< Rule >::success( in, state );
      }

      template< typename ParseInput >
      static void failure( const ParseInput& in, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).failure;
         Control< Rule >::failure( in, state );
      }

      template< typename ParseInput >
      [[noreturn]] static void raise( const ParseInput& in, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).raise;
         Control< Rule >::raise( in, state );
      }
   };

   template< typename Rule >
   using coverage_control = basic_coverage_control< Rule, normal >;

   template< typename Grammar, typename ParseInput >
   bool coverage( ParseInput&& in )
   {
      coverage_state state;
      visit< coverage_insert, Grammar >( state );
      const auto result = parse< Grammar, nothing, coverage_control >( in, state );
      std::cout << "{ \"grammar\": \"" << internal::demangle< Grammar >() << "\", \"source\": \"" << in.source() << "\", \"result\": " << ( result ? "true" : "false" ) << ", \"coverage\": [" << std::endl;
      for( const auto& [ k, v ] : state.map ) {
         std::cout << "   { \"rule\": \"" << k << "\", \"start\": " << v.start << ", \"success\": " << v.success << ", \"failure\": " << v.failure << ", \"raise\": " << v.raise << " }," << std::endl;
      }
      std::cout << "] }" << std::endl;
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
