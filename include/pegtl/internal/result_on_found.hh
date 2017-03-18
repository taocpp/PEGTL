// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RESULT_ON_FOUND_HH
#define PEGTL_INTERNAL_RESULT_ON_FOUND_HH

#include "../config.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      enum class result_on_found : bool
      {
         SUCCESS = true,
         FAILURE = false
      };

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
