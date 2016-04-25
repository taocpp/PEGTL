// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_MINUS_HH
#define PEGTL_INTERNAL_MINUS_HH

#include "skip_control.hh"

#include "../apply_mode.hh"
#include "../memory_input.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename M, typename S >
      struct minus
      {
         using analyze_t = typename M::analyze_t;  // NOTE: S is currently ignored for analyze().

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.mark();

            if ( ! Control< M >::template match< A, Action, Control >( in, st ... ) ) {
               return false;
            }
            memory_input i2( m.line(), m.byte_in_line(), m.begin(), in.begin(), in.source() );

            if ( ! Control< S >::template match< apply_mode::NOTHING, Action, Control >( i2, st ... ) ) {
               return m( true );
            }
            return m( ! i2.empty() );
         }
      };

      template< typename M, typename S >
      struct skip_control< minus< M, S > > : std::true_type {};

   } // internal

} // pegtl

#endif
