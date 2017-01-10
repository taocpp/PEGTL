// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_MINUS_HH
#define PEGTL_INTERNAL_MINUS_HH

#include "skip_control.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"
#include "../memory_input.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename R, typename S >
      struct minus
      {
         using analyze_t = typename R::analyze_t;  // NOTE: S is currently ignored for analyze().

         template< apply_mode A, rewind_mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< rewind_mode::REQUIRED >();

            if ( ! Control< R >::template match< A, rewind_mode::DONTCARE, Action, Control >( in, st ... ) ) {
               return false;
            }
            using memory_t = typename Input::memory_t;
            memory_t i2( m, in.data() );

            if ( ! Control< S >::template match< apply_mode::NOTHING, rewind_mode::DONTCARE, Action, Control >( i2, st ... ) ) {
               return m( true );
            }
            return m( ! i2.empty() );
         }
      };

      template< typename R, typename S >
      struct skip_control< minus< R, S > > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
