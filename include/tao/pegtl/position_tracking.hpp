// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_POSITION_TRACKING_HPP
#define TAOCPP_PEGTL_INCLUDE_POSITION_TRACKING_HPP

#include "config.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      enum class position_tracking : bool
      {
         IMMEDIATE,
         LAZY
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
