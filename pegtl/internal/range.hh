// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RANGE_HH
#define PEGTL_INTERNAL_RANGE_HH

#include "any.hh"
#include "bump_util.hh"
#include "skip_control.hh"
#include "result_on_found.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      struct range
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< int Eol >
         struct can_match_eol
         {
            static constexpr bool value = ( ( ( Lo <= Eol ) && ( Eol <= Hi ) ) == bool( R ) );
         };

         // suppress warning with GCC 4.7
         template< typename T >
         static inline bool dummy_less_or_equal( const T a, const T b )
         {
            return a <= b;
         }

         template< typename Input >
         static bool match( Input & in )
         {
            using eol_t = typename Input::eol_t;

            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  if ( ( dummy_less_or_equal( Lo, t.data ) && dummy_less_or_equal( t.data, Hi ) ) == bool( R ) ) {
                     bump_impl< can_match_eol< eol_t::ch >::value >::bump( in, t.size );
                     return true;
                  }
               }
            }
            return false;
         }
      };

      template< result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      struct skip_control< range< R, Peek, Lo, Hi > > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
