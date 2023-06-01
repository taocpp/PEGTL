// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <bitset>
#include <iostream>
#include <utility>

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< bool Optional, typename... Rules >
      struct rnd;

      template< bool Optional >
      struct rnd< Optional >
         : success
      {};

      template< bool Optional, typename... Rules >
      struct rnd
      {
         using rule_t = rnd;
         using subs_t = type_list< Rules... >;

         static constexpr std::size_t rule_count = sizeof...( Rules );

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   std::size_t... Indices,
                   typename ParseInput,
                   typename... States >
         [[nodiscard]] static bool match( std::index_sequence< Indices... > /*unused*/, ParseInput& in, States&&... st )
         {
            std::bitset< rule_count > b;
            auto m = in.template auto_rewind< M >();
            using m_t = decltype( m );
            for( std::size_t i = 0; i < rule_count; ++i ) {
               if( !( ( ( b.test( Indices ) == false ) && ( Control< Rules >::template match< A, m_t::next_rewind_mode, Action, Control >( in, st... ) ) && ( b.set( Indices ), true ) ) || ... ) ) {
                  return m( Optional );
               }
            }
            return m( true );
         }

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput,
                   typename... States >
         [[nodiscard]] static bool match( ParseInput& in, States&&... st )
         {
            if constexpr( sizeof...( Rules ) == 1 ) {
               return Control< Rules... >::template match< A, M, Action, Control >( in, st... );
            }
            else {
               return match< A, M, Action, Control >( std::index_sequence_for< Rules... >(), in, st... );
            }
         }
      };

      template< bool Optional, typename... Rules >
      inline constexpr bool enable_control< rnd< Optional, Rules... > > = false;

   }  // namespace internal

   template< typename... Rules >
   struct rnd
      : internal::rnd< false, Rules... >
   {};

   template< typename... Rules >
   struct rnd_opt
      : internal::rnd< true, Rules... >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

int main( int argc, char** argv )
{
   using namespace TAO_PEGTL_NAMESPACE;

   using grammar1 = seq< rnd< one< 'a' >, one< 'b' >, one< 'c' > >, eolf >;
   using grammar2 = seq< rnd_opt< one< 'a' >, one< 'b' >, one< 'c' > >, eolf >;

   for( int i = 1; i < argc; ++i ) {
      {
         argv_input in( argv, i );
         const bool b = parse< grammar1 >( in );
         std::cout << "input: " << argv[ i ] << " rnd: " << b << std::endl;
      }
      {
         argv_input in( argv, i );
         const bool b = parse< grammar2 >( in );
         std::cout << "input: " << argv[ i ] << " rnd_opt: " << b << std::endl;
      }
   }
   return 0;
}
