// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_UNTIL_HH
#define PEGTL_INTERNAL_UNTIL_HH

#include "eof.hh"
#include "star.hh"
#include "bytes.hh"
#include "not_at.hh"
#include "skip_control.hh"
#include "rule_conjunction.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Cond, typename ... Rules > struct until;

      template< typename Cond, typename ... Rules >
      struct skip_control< until< Cond, Rules ... > > : std::true_type {};

      template< typename Cond >
      struct until< Cond >
      {
         using analyze_t = analysis::generic< analysis::rule_type::SEQ, star< not_at< Cond >, not_at< eof >, bytes< 1 > >, Cond >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< M >();

            while ( ! Control< Cond >::template match< A, rewind_mode::REQUIRED, Action, Control >( in, st ... ) ) {
               if ( in.empty() ) {
                  return false;
               }
               in.bump();
            }
            return m( true );
         }
      };

      template< typename Cond, typename ... Rules >
      struct until
      {
         using analyze_t = analysis::generic< analysis::rule_type::SEQ, star< not_at< Cond >, not_at< eof >, Rules ... >, Cond >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< M >();

            while ( ! Control< Cond >::template match< A, rewind_mode::REQUIRED, Action, Control >( in, st ... ) ) {
               if ( in.empty() || ( ! rule_conjunction< Rules ... >::template match< A, rewind_mode::DONTCARE, Action, Control >( in, st ... ) ) ) {
                  return false;
               }
            }
            return m( true );
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
