// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_LIST_MUST_HH
#define PEGTL_INTERNAL_LIST_MUST_HH

#include "../config.hh"

#include "seq.hh"
#include "star.hh"
#include "must.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule, typename Sep >
      using list_must = seq< Rule, star< Sep, must< Rule > > >;

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
