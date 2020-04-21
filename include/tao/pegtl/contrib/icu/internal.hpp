// Copyright (c) 2018-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ICU_INTERNAL_HPP
#define TAO_PEGTL_CONTRIB_ICU_INTERNAL_HPP

#include <unicode/uchar.h>

#include "../../config.hpp"
#include "../../type_list.hpp"

#include "../../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   namespace icu
   {
      template< typename Peek, UProperty P, bool V = true >
      struct binary_property
      {
         using rule_t = binary_property;
         using subs_t = empty_list;

         template< typename ParseInput >
         [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
         {
            if( const auto r = Peek::peek( in ) ) {
               if( u_hasBinaryProperty( r.data, P ) == V ) {
                  in.bump( r.size );
                  return true;
               }
            }
            return false;
         }
      };

      template< typename Peek, UProperty P, int V >
      struct property_value
      {
         using rule_t = property_value;
         using subs_t = empty_list;

         template< typename ParseInput >
         [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
         {
            if( const auto r = Peek::peek( in ) ) {
               if( u_getIntPropertyValue( r.data, P ) == V ) {
                  in.bump( r.size );
                  return true;
               }
            }
            return false;
         }
      };

   }  // namespace icu

   template< typename Peek, UProperty P, bool V >
   inline constexpr bool enable_control< icu::binary_property< Peek, P, V > > = false;

   template< typename Peek, UProperty P, int V >
   inline constexpr bool enable_control< icu::property_value< Peek, P, V > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
