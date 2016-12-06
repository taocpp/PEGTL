// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RANGES_HH
#define PEGTL_INTERNAL_RANGES_HH

#include "any.hh"
#include "range.hh"
#include "bump_util.hh"
#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< int EOL, typename Char, Char ... Cs > struct ranges_impl;

      template< int EOL, typename Char >
      struct ranges_impl< EOL, Char >
      {
         static constexpr bool can_match_eol = false;

         static bool match( const Char )
         {
            return false;
         }
      };

      template< int EOL, typename Char, Char Eq >
      struct ranges_impl< EOL, Char, Eq >
      {
         static constexpr bool can_match_eol = ( Eq == EOL );

         static bool match( const Char c )
         {
            return c == Eq;
         }
      };

      template< int EOL, typename Char, Char Lo, Char Hi, Char ... Cs >
      struct ranges_impl< EOL, Char, Lo, Hi, Cs ... >
      {
         static constexpr bool can_match_eol = ( ( ( Lo <= EOL ) && ( EOL <= Hi ) ) || ranges_impl< EOL, Char, Cs ... >::can_match_eol );

         static bool match( const Char c )
         {
            return ( ( Lo <= c ) && ( c <= Hi ) ) || ranges_impl< EOL, Char, Cs ... >::match( c );
         }
      };

      template< typename Peek, typename Peek::data_t ... Cs >
      struct ranges
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< int EOL >
         struct can_match_eol
         {
            static constexpr bool value = ranges_impl< EOL, typename Peek::data_t, Cs ... >::can_match_eol;
         };

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  if ( ranges_impl< eol_mode_to_int( Input::eol ), typename Peek::data_t, Cs ... >::match( t.data ) ) {
                     bump_impl< can_match_eol< eol_mode_to_int( Input::eol ) >::value >::bump( in, t.size );
                     return true;
                  }
               }
            }
            return false;
         }
      };

      template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      struct ranges< Peek, Lo, Hi >
            : range< result_on_found::SUCCESS, Peek, Lo, Hi > {};

      template< typename Peek, typename Peek::data_t ... Cs >
      struct skip_control< ranges< Peek, Cs ... > > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
