// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_PAD_OPT_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_PAD_OPT_HH

#include "../config.hh"

#include "seq.hh"
#include "opt.hh"
#include "star.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Rule, typename Pad >
         using pad_opt = seq< star< Pad >, opt< Rule, star< Pad > > >;

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
