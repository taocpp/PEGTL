// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_IF_MUST_ELSE_HH
#define PEGTL_INTERNAL_IF_MUST_ELSE_HH

#include "../config.hh"

#include "if_then_else.hh"
#include "must.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Cond, typename Then, typename Else >
      using if_must_else = if_then_else< Cond, must< Then >, must< Else > >;

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
