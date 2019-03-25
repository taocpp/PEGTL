// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_RESULT_ON_FOUND_HPP
#define TAO_PEGTL_INTERNAL_RESULT_ON_FOUND_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   enum class result_on_found : bool
   {
      success = true,
      failure = false
   };

   constexpr result_on_found operator!( const result_on_found v ) noexcept
   {
      return ( v == result_on_found::success ) ? result_on_found::failure : result_on_found::success;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
