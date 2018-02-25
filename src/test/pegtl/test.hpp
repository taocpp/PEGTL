// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_HPP  // NOLINT
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <tao/pegtl.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      std::size_t failed = 0;                                        // NOLINT
      std::vector< std::pair< std::string, std::string > > applied;  // NOLINT

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#define TAO_PEGTL_TEST_UNWRAP( ... ) __VA_ARGS__

#define TAO_PEGTL_TEST_FAILED( MeSSaGe )            \
   do {                                             \
      std::cerr << "pegtl: unit test failed for [ " \
                << internal::demangle< Rule >()     \
                << " ] "                            \
                << TAO_PEGTL_TEST_UNWRAP( MeSSaGe ) \
                << " in line [ "                    \
                << line                             \
                << " ] file [ "                     \
                << file << " ]"                     \
                << std::endl;                       \
      ++failed;                                     \
   } while( false )

#define TAO_PEGTL_TEST_ASSERT( eXPReSSioN )        \
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

#define TAO_PEGTL_TEST_THROWS( ... )                \
   do {                                             \
      try {                                         \
         __VA_ARGS__;                               \
         std::cerr << "pegtl: unit test [ "         \
                   << ( #__VA_ARGS__ )              \
                   << " ] did not throw in line [ " \
                   << __LINE__                      \
                   << " ] file [ "                  \
                   << __FILE__ << " ]"              \
                   << std::endl;                    \
         ++failed;                                  \
      }                                             \
      catch( ... ) {                                \
      }                                             \
   } while( false )

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
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

            TAO_PEGTL_TEST_ASSERT( in.size() == Size );

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

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
