// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_PAD_OPT_HH
#define PEGTL_INTERNAL_PAD_OPT_HH

#include "../config.hh"

#include "seq.hh"
#include "opt.hh"
#include "star.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule, typename Pad >
      using pad_opt = seq< star< Pad >, opt< Rule, star< Pad > > >;

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
