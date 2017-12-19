// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_RESULT_ON_FOUND_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_RESULT_ON_FOUND_HPP

#include "../config.hpp"

namespace tao
{
   namespace pegtl
   {
      namespace internal
      {
         enum class result_on_found : bool
         {
            SUCCESS = true,
            FAILURE = false
         };

      }  // namespace internal

   }  // namespace pegtl

}  // namespace tao

#endif
