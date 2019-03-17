// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_MISSING_APPLY0_HPP
#define TAO_PEGTL_INTERNAL_MISSING_APPLY0_HPP

#include "../config.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Control,
                   template< typename... >
                   class Action,
                   typename Input,
                   typename... States >
         void missing_apply0( Input& in, States&&... st )
         {
            Control::template apply0< Action >( in, st... );
         }

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
