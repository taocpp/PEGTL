// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_HAS_APPLY0_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_HAS_APPLY0_HPP

#include <type_traits>

#include "../config.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename... S >
         struct type_list
         {
         };

         template< typename A, typename L, typename = void >
         struct has_apply0 : std::false_type
         {
         };

         template< typename A, typename... S >
         struct has_apply0< A, type_list< S... >, decltype( A::apply0( std::declval< S >()... ), void() ) > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
