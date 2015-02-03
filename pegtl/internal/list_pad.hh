// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_LIST_PAD_HH
#define PEGTL_INTERNAL_LIST_PAD_HH

#include "seq.hh"
#include "pad.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Rule, typename Sep, typename Pad, template< typename ... > class Star >
      using list_pad = seq< Rule, Star< pad< Sep, Pad >, Rule > >;

   } // internal

} // pegtl

#endif
