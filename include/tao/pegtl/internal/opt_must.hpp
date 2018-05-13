// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_OPT_MUST_HPP
#define TAO_PEGTL_INTERNAL_OPT_MUST_HPP

#include "../config.hpp"

#include "if_must.hpp"
#include "opt.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Cond, typename... Rules >
         using opt_must = opt< if_must< Cond, Rules... > >;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
