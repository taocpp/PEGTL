// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_SEPARATED_HPP
#define TAO_PEGTL_CONTRIB_SEPARATED_HPP

#include "../config.hpp"

#include "../internal/seq.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename... >
      struct separated;

      template< typename... Ts, typename S, typename Rule, typename... Rules >
      struct separated< seq< Ts... >, S, Rule, Rules... >
         : separated< seq< Ts..., Rule, S >, S, Rules... >
      {};

      template< typename... Ts, typename S, typename Rule >
      struct separated< seq< Ts... >, S, Rule >
      {
         using type = seq< Ts..., Rule >;
      };

      template< typename S >
      struct separated< seq<>, S >
      {
         using type = seq<>;
      };

   }  // namespace internal

   template< typename S, typename... Rules >
   struct separated
      : internal::separated< internal::seq<>, S, Rules... >::type
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
