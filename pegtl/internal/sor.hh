// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_SOR_HH
#define PEGTL_INTERNAL_SOR_HH

#include "skip_control.hh"
#include "rule_disjunction.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct sor
            : rule_disjunction< Rules ... >
      {
         using analyze_t = analysis::generic< analysis::rule_type::SOR, Rules ... >;
      };

      template< typename ... Rules >
      struct skip_control< sor< Rules ... > > : std::true_type {};

   } // internal

} // pegtl

#endif
