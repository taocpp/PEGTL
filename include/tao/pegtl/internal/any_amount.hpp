// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_ANY_AMOUNT_HPP
#define TAO_PEGTL_INTERNAL_ANY_AMOUNT_HPP

#include <utility>

#include "../config.hpp"

#include "bump_help.hpp"
#include "result_on_found.hpp"
#include "skip_control.hpp"

#include "../analysis/generic.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< char C, unsigned int N >
         struct any_amount
         {
            using analyze_t = analysis::generic< analysis::rule_type::any >;

            template< typename Input >
            [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.size( N ) ) )
            {
               if( in.size( N ) >= N ) {
                  for (unsigned int i = 0; i < N; i++) {
                     if (in.peek_char(i) != C) {
                        return false;
                     }
                  }
                  bump_help< result_on_found::success, Input, char, C >( in, N );
                  return true;
               }
               return false;
            }
         };

         template< char C, unsigned int N >
         inline constexpr bool skip_control< any_amount< C, N > > = true;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
