// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_LIST_HH
#define PEGTL_INTERNAL_LIST_HH

#include "../config.hh"

#include "seq.hh"
#include "star.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule, typename Sep >
      using list = seq< Rule, star< Sep, Rule > >;

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
