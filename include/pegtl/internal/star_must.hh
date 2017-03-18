// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_STAR_MUST_HH
#define PEGTL_INTERNAL_STAR_MUST_HH

#include "../config.hh"

#include "star.hh"
#include "if_must.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
     template< typename Cond, typename ... Rules >
     using star_must = star< if_must< Cond, Rules ... > >;

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
