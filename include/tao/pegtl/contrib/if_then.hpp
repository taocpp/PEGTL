// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_IF_THEN_HPP
#define TAO_PEGTL_CONTRIB_IF_THEN_HPP

#include "../config.hpp"

#include "../internal/if_then_else.hpp"
#include "../internal/seq.hpp"
#include "../internal/skip_control.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Cond, typename Then >
         struct if_pair
         {
         };

         template< typename... Pairs >
         struct if_then;

         template< typename Cond, typename Then, typename... Pairs >
         struct if_then< if_pair< Cond, Then >, Pairs... >
            : if_then_else< Cond, Then, if_then< Pairs... > >
         {
            template< typename Cond2, typename... Rules2 >
            using else_if_then = if_then< if_pair< Cond, Then >, Pairs..., if_pair< Cond2, seq< Rules2... > > >;

            template< typename... Rules2 >
            using else_then = if_then_else< Cond, Then, if_then< Pairs..., if_pair< success, seq< Rules2... > > > >;
         };

         template<>
         struct if_then<>
            : failure
         {
         };

         template< typename... Pairs >
         struct skip_control< if_then< Pairs... > > : std::true_type
         {
         };

      }  // namespace internal

      template< typename Cond, typename... Rules >
      using if_then = internal::if_then< internal::if_pair< Cond, seq< Rules... > > >;

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
