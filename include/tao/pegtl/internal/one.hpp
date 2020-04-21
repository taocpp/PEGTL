// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_ONE_HPP
#define TAO_PEGTL_INTERNAL_ONE_HPP

#include <cstddef>

#include "../config.hpp"

#include "any.hpp"
#include "bump_help.hpp"
#include "enable_control.hpp"
#include "failure.hpp"
#include "result_on_found.hpp"

#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< result_on_found R, typename Peek, typename Peek::data_t... Cs >
   struct one
   {
      using rule_t = one;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( ( ( t.data == Cs ) || ... ) == bool( R ) ) {
               bump_help< R, ParseInput, typename Peek::data_t, Cs... >( in, t.size );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek >
   struct one< result_on_found::success, Peek >
      : failure
   {};

   template< typename Peek >
   struct one< result_on_found::failure, Peek >
      : any< Peek >
   {};

   template< result_on_found R, typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< one< R, Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
