// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_RANGE_HPP
#define TAO_PEGTL_INTERNAL_RANGE_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "one.hpp"
#include "result_on_found.hpp"

#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct range
   {
      using rule_t = range;
      using subs_t = empty_list;

      static_assert( Lo < Hi, "invalid range" );

      template< int Eol >
      static constexpr bool can_match_eol = ( ( ( Lo <= Eol ) && ( Eol <= Hi ) ) == bool( R ) );

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( Peek::peek( in ) ) )
      {
         if( const auto t = Peek::peek( in ) ) {
            if( ( ( Lo <= t.data ) && ( t.data <= Hi ) ) == bool( R ) ) {
               if constexpr( can_match_eol< ParseInput::eol_t::ch > ) {
                  in.bump( t.size );
               }
               else {
                  in.bump_in_this_line( t.size );
               }
               return true;
            }
         }
         return false;
      }
   };

   template< result_on_found R, typename Peek, typename Peek::data_t C >
   struct range< R, Peek, C, C >
      : one< R, Peek, C >
   {};

   template< result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   inline constexpr bool enable_control< range< R, Peek, Lo, Hi > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
