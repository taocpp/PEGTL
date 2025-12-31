// Copyright (c) 2018-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UNICODE_INTERNAL_ICU_BINARY_PROPERTY_HPP
#define TAO_PEGTL_UNICODE_INTERNAL_ICU_BINARY_PROPERTY_HPP

#include <unicode/uchar.h>

#include "../../config.hpp"
#include "../../type_list.hpp"

#include "../../debug/analyze_traits.hpp"

#include "../../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Peek, UProperty P, bool V = true >
      struct icu_binary_property
      {
         using peek_t = Peek;
         using data_t = typename Peek::data_t;

         using rule_t = icu_binary_property;
         using subs_t = empty_list;

         static constexpr bool implicit_rewind = true;

         [[nodiscard]] static bool test( const data_t c ) noexcept
         {
            return u_hasBinaryProperty( c, P ) == V;
         }

         template< typename ParseInput >
         [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
         {
            if( const auto t = Peek::peek( in ) ) {
               if( test( t.data() ) ) {
                  in.template consume< icu_binary_property >( t.size() );
                  return true;
               }
            }
            return false;
         }
      };

      template< typename Peek, UProperty P, bool V >
      inline constexpr bool enable_control< icu_binary_property< Peek, P, V > > = false;

   }  // namespace internal

   template< typename Name, typename Peek, UProperty P, bool V >
   struct analyze_traits< Name, internal::icu_binary_property< Peek, P, V > >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
