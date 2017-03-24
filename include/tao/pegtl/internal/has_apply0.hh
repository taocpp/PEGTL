// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_HAS_APPLY0_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_HAS_APPLY0_HH

#include <type_traits>

#include "../config.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename ... S > struct type_list {};

      template< typename A, typename L, typename = void >
      struct has_apply0 : std::false_type {};

      template< typename A, typename ... S >
      struct has_apply0< A, type_list< S ... >, typename std::enable_if< std::is_same< decltype( A::apply0( std::declval< S >() ... ) ), void >::value >::type > : std::true_type {};

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
