// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RAISE_HH
#define PEGTL_INTERNAL_RAISE_HH

#include <cassert>

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename T >
      struct raise
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            Control< T >::raise( static_cast< const Input & >( in ), st ... );
            assert( false ); // unreachable
         }
      };

      template< typename Exception >
      struct skip_control< raise< Exception > > : std::true_type {};

   } // internal

} // pegtl

#endif
