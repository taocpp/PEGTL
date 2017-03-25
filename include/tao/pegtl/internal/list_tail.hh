// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_LIST_TAIL_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_LIST_TAIL_HH

#include "../config.hh"

#include "seq.hh"
#include "list.hh"
#include "opt.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Rule, typename Sep >
         using list_tail = seq< list< Rule, Sep >, opt< Sep > >;

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
