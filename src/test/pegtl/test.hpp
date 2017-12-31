// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_TEST_HPP
#define TAOCPP_PEGTL_INCLUDE_TEST_TEST_HPP

#include <cassert>
#include <cstddef>
#include <iostream>

#include <tao/pegtl.hpp>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      std::size_t failed = 0;                                        // NOLINT
      std::vector< std::pair< std::string, std::string > > applied;  // NOLINT

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#define TAOCPP_PEGTL_TEST_UNWRAP( ... ) __VA_ARGS__

#define TAOCPP_PEGTL_TEST_FAILED( MeSSaGe )            \
   do {                                                \
      std::cerr << "pegtl: unit test failed for [ "    \
                << internal::demangle< Rule >()        \
                << " ] "                               \
                << TAOCPP_PEGTL_TEST_UNWRAP( MeSSaGe ) \
                << " in line [ "                       \
                << line                                \
                << " ] file [ "                        \
                << file << " ]"                        \
                << std::endl;                          \
      ++failed;                                        \
   } while( false )

#define TAOCPP_PEGTL_TEST_ASSERT( eXPReSSioN )     \
   do {                                            \
      if( !( eXPReSSioN ) ) {                      \
         std::cerr << "pegtl: unit test assert [ " \
                   << ( #eXPReSSioN )              \
                   << " ] failed in line [ "       \
                   << __LINE__                     \
                   << " ] file [ "                 \
                   << __FILE__ << " ]"             \
                   << std::endl;                   \
         ++failed;                                 \
      }                                            \
   } while( false )

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< unsigned Size, apply_mode B, rewind_mode N, typename... Rules >
      struct test_rule
      {
         using analyze_t = typename seq< Rules... >::analyze_t;

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   typename Input,
                   typename... States >
         static bool match( Input& in, States&&... st )
         {
            static_assert( A == B, "unexpected apply mode" );
            static_assert( M == N, "unexpected rewind mode" );

            TAOCPP_PEGTL_TEST_ASSERT( in.size() == Size );

            return seq< Rules... >::template match< A, M, Action, Control >( in, st... );
         }
      };

      template< typename Rule >
      struct test_action
      {
         template< typename Input >
         static void apply( const Input& in )
         {
            applied.push_back( std::make_pair( internal::demangle< Rule >(), in.string() ) );
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
