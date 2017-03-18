// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_EOLF_HH
#define PEGTL_INTERNAL_EOLF_HH

#include "../config.hh"

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      struct eolf
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT >;

         template< typename Input >
         static bool match( Input & in )
         {
            using eol_t = typename Input::eol_t;
            const auto p = eol_t::match( in );
            return p.first || ( ! p.second );
         }
      };

      template<>
      struct skip_control< eolf > : std::true_type {};

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
