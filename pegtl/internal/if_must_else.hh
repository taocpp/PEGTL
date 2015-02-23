// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IF_MUST_ELSE_HH
#define PEGTL_INTERNAL_IF_MUST_ELSE_HH

#include "if_then_else.hh"
#include "skip_control.hh"
#include "must.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Cond, typename Then, typename Else >
      using if_must_else = if_then_else< Cond, skip_control< must< Then > >, skip_control< must< Else > > >;

   } // internal

} // pegtl

#endif
