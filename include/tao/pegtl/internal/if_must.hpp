// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_IF_MUST_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_IF_MUST_HPP

#include "../config.hpp"

#include "must.hpp"
#include "seq.hpp"

namespace tao
{
   namespace pegtl
   {
      namespace internal
      {
         template< typename Cond, typename... Thens >
         using if_must = seq< Cond, must< Thens... > >;

      }  // namespace internal

   }  // namespace pegtl

}  // namespace tao

#endif
