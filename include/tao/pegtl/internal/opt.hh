// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_OPT_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_OPT_HH

#include <type_traits>

#include "../config.hh"

#include "skip_control.hh"
#include "duseltronik.hh"
#include "seq.hh"
#include "trivial.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/generic.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename ... Rules > struct opt;

         template< typename ... Rules >
         struct skip_control< opt< Rules ... > > : std::true_type {};

         template<>
         struct opt<>
               : trivial< true > {};

         template< typename ... Rules >
         struct opt
         {
            using analyze_t = analysis::generic< analysis::rule_type::OPT, Rules ... >;

            template< apply_mode A, rewind_mode, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
            static bool match( Input & in, States && ... st )
            {
               if ( ! in.empty() ) {
                  duseltronik< seq< Rules ... >, A, rewind_mode::REQUIRED, Action, Control >::match( in, st ... );
               }
               return true;
            }
         };

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
