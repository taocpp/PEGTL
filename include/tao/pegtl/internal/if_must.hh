// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_IF_MUST_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_IF_MUST_HH

#include "../config.hh"

#include "seq.hh"
#include "must.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Cond, typename ... Thens >
      using if_must = seq< Cond, must< Thens ... > >;

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
