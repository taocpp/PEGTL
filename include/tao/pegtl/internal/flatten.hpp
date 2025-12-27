// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FLATTEN_HPP
#define TAO_PEGTL_INTERNAL_FLATTEN_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< template< typename... > class S, typename D, typename T >
   struct flatten_impl;

   template< template< typename... > class S, typename... Ds, typename T, typename... Ts >
   struct flatten_impl< S, S< Ds... >, S< T, Ts... > >
      : flatten_impl< S, S< Ds..., T >, S< Ts... > >
   {};

   template< template< typename... > class S, typename... Ds, typename... Ns, typename... Ts >
   struct flatten_impl< S, S< Ds... >, S< S< Ns... >, Ts... > >
      : flatten_impl< S, S< Ds..., Ns... >, S< Ts... > >
   {};

   template< template< typename... > class S, typename... Ds >
   struct flatten_impl< S, S< Ds... >, S<> >
   {
      using type = S< Ds... >;
   };

   template< typename T >
   struct flatten_help;

   template< template< typename... > class S, typename... Ts >
   struct flatten_help< S< Ts... > >
      : flatten_impl< S, S<>, S< Ts... > >
   {};

   template< typename T >
   using flatten_t = typename flatten_help< T >::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
