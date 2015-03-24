// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RANGES_HH
#define PEGTL_INTERNAL_RANGES_HH

#include "skip_control.hh"
#include "any.hh"
#include "range.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Char, Char ... Cs > struct ranges_impl;

      template< typename Char >
      struct ranges_impl< Char >
      {
         static bool match( const Char )
         {
            return false;
         }
      };

      template< typename Char, Char Eq >
      struct ranges_impl< Char, Eq >
      {
         static bool match( const Char c )
         {
            return c == Eq;
         }
      };

      template< typename Char, Char Lo, Char Hi, Char ... Cs >
      struct ranges_impl< Char, Lo, Hi, Cs ... >
      {
         static bool match( const Char c )
         {
            return ( ( Lo <= c ) && ( c <= Hi ) ) || ranges_impl< Char, Cs ... >::match( c );
         }
      };

      template< typename Peek, typename Peek::data_t ... Cs >
      struct ranges
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  if ( ranges_impl< typename Peek::data_t, Cs ... >::match( t.data ) ) {
                     in.bump( t.size );
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

   } // internal

} // pegtl

#endif
