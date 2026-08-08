// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SOR1_HPP
#define TAO_PEGTL_INTERNAL_SOR1_HPP

#include <type_traits>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "failure.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct sor1
   {
      using rule_t = sor1;
      using subs_t = type_list< Rules... >;

      using head_t = type_list_head_t< Rules... >;

      static_assert( ( ( std::is_same_v< typename head_t::case_t::peek_t, typename Rules::case_t::peek_t > ) && ... ) );

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         bool result = false;

         using case_t = typename head_t::case_t;
         using peek_t = typename case_t::peek_t;

         if( const auto t = peek_t::peek( in ) ) {
            (void)( ( Rules::case_t::test( t.data() ) ? ( ( result = Control< Rules >::template match< A, M, Action, Control >( in, st... ) ), true ) : false ) || ... );
         }
         return result;
      }
   };

   template<>
   struct sor1<>
      : failure
   {};

   template< typename... Rules >
   inline constexpr bool enable_control< sor1< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
