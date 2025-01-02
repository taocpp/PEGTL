// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_META_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_META_HPP

#include <type_traits>

#include <tao/pegtl/debug/analyze.hpp>
#include <tao/pegtl/type_list.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Name, typename Rule, typename... Rules >
   void verify_meta()
   {
      static_assert( std::is_same_v< typename Name::rule_t, Rule > );
      static_assert( std::is_same_v< typename Name::subs_t, type_list< Rules... > > );

      if constexpr( !std::is_same_v< Name, Rule > ) {
         TAO_PEGTL_TEST_ASSERT( internal::enable_control< Name > );
         TAO_PEGTL_TEST_ASSERT( !internal::enable_control< Rule > );
      }
   }

   template< typename Rule >
   void verify_analyze( const unsigned line, const char* file, const bool expect_consume, const bool expect_problems )
   {
      internal::analyze_cycles< Rule > a( -1 );

      const auto problems = a.problems();

      TAO_PEGTL_TEST_ASSERT( problems == analyze< Rule >( -1 ) );

      const bool has_problems = ( problems != 0 );
      const bool does_consume = a.template consumes< Rule >();

      if( has_problems != expect_problems ) {
         TAO_PEGTL_TEST_FAILED( "analyze -- problems received/expected [ " << has_problems << " / " << expect_problems << " ]" );  // LCOV_EXCL_LINE
      }
      if( does_consume != expect_consume ) {
         TAO_PEGTL_TEST_FAILED( "analyze -- consumes received/expected [ " << does_consume << " / " << expect_consume << " ]" );  // LCOV_EXCL_LINE
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
