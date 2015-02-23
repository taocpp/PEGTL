// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_LIST_MUST_HH
#define PEGTL_INTERNAL_LIST_MUST_HH

#include "seq.hh"
#include "skip_control.hh"
#include "star.hh"
#include "must.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Rule, typename Sep >
      using list_must = seq< Rule, skip_control< star< Sep, skip_control< must< Rule > > > > >;

   } // internal

} // pegtl

#endif
