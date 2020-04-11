// Copyright (c) 2019-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_MISSING_APPLY0_HPP
#define TAO_PEGTL_INTERNAL_MISSING_APPLY0_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Control,
             template< typename... >
             class Action,
             typename ParseInput,
             typename... States >
   void missing_apply0( ParseInput& in, States&&... st )
   {
      (void)Control::template apply0< Action >( in, st... );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
