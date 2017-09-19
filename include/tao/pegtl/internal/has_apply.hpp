// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_HAS_APPLY_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_HAS_APPLY_HPP

#include <type_traits>

#include "../config.hpp"

#include "type_list.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename A, typename I, typename L, typename = void >
         struct has_apply_void : std::false_type
         {
         };

         template< typename A, typename I, typename... S >
         struct has_apply_void< A, I, type_list< S... >, decltype( A::apply( std::declval< I >(), std::declval< S >()... ) ) > : std::true_type
         {
         };

         template< typename A, typename I, typename L, typename = bool >
         struct has_apply_bool : std::false_type
         {
         };

         template< typename A, typename I, typename... S >
         struct has_apply_bool< A, I, type_list< S... >, decltype( A::apply( std::declval< I >(), std::declval< S >()... ) ) > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
