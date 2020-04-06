// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_BYTES_HPP
#define TAO_PEGTL_INTERNAL_BYTES_HPP

#include "../config.hpp"

#include "skip_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< unsigned Cnt >
   struct bytes
   {
      using rule_t = bytes;
      using analyze_t = analysis::counted< analysis::rule_type::any, Cnt >;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.size( 0 ) ) )
      {
         if( in.size( Cnt ) >= Cnt ) {
            in.bump( Cnt );
            return true;
         }
         return false;
      }
   };

   template< unsigned Cnt >
   inline constexpr bool skip_control< bytes< Cnt > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
