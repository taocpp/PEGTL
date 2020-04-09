// Copyright (c) 2017-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_REP_ONE_MIN_MAX_HPP
#define TAO_PEGTL_CONTRIB_REP_ONE_MIN_MAX_HPP

#include <algorithm>
#include <type_traits>

#include "../config.hpp"
#include "../rule_list.hpp"

#include "../internal/bump_help.hpp"
#include "../internal/bytes.hpp"
#include "../internal/enable_control.hpp"
#include "../internal/opt.hpp"

#include "forward.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< unsigned Min, unsigned Max, char C >
      struct rep_one_min_max
      {
         using rule_t = rep_one_min_max;
         using subs_t = empty_list;

         static_assert( Min <= Max );

         template< typename Input >
         [[nodiscard]] static bool match( Input& in )
         {
            const auto size = in.size( Max + 1 );
            if( size < Min ) {
               return false;
            }
            std::size_t i = 0;
            while( ( i < size ) && ( in.peek_char( i ) == C ) ) {
               ++i;
            }
            if( ( Min <= i ) && ( i <= Max ) ) {
               bump_help< result_on_found::success, Input, char, C >( in, i );
               return true;
            }
            return false;
         }
      };

      template< unsigned Min, unsigned Max, char C >
      inline constexpr bool enable_control< rep_one_min_max< Min, Max, C > > = false;

   }  // namespace internal

   inline namespace ascii
   {
      template< unsigned Min, unsigned Max, char C >
      struct rep_one_min_max
         : internal::rep_one_min_max< Min, Max, C >
      {};

   }  // namespace ascii

   template< typename Name, unsigned Min, unsigned Max, char C >
   struct analyze_traits< Name, internal::rep_one_min_max< Min, Max, C > >
   {
      using reduced = std::conditional_t< ( Min != 0 ), internal::bytes< 1 >, internal::opt<> >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
