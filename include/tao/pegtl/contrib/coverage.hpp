// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_COVERAGE_HPP
#define TAO_PEGTL_CONTRIB_COVERAGE_HPP

#include <map>
#include <string_view>

#include "../config.hpp"

#include "../internal/demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct coverage_info
   {
      std::size_t start = 0;
      std::size_t success = 0;
      std::size_t failure = 0;
      // TODO: Apply?
      // TODO: Raise?
   };

   struct coverage_state
   {
      std::map< std::string_view, coverage_info > map;
   };

   template< typename Rule >
   struct coverage_insert
   {
      static void call( coverage_state& state )
      {
         state.map.try_emplace( internal::demangle< Rule >() );
      }
   };

   template< typename Rule >
   struct coverage_control
      : normal< Rule >
   {
      template< typename Input >
      static void start( const Input& /*unused*/, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).start;
      }

      template< typename Input >
      static void success( const Input& /*unused*/, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).success;
      }

      template< typename Input >
      static void failure( const Input& /*unused*/, coverage_state& state )
      {
         ++state.map.at( internal::demangle< Rule >() ).failure;
      }
   };

   // TODO: Generic coverage wrapper function, see src/examples/pegtl/json_coverage.cpp.

}  // namespace TAO_PEGTL_NAMESPACE

#endif
