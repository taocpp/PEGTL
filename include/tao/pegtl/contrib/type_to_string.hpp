// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_TYPE_TO_STRING_HPP
#define TAO_PEGTL_CONTRIB_TYPE_TO_STRING_HPP

#include <string>
#include <string_view>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< char... Cs >
      struct char_list
      {};

      template< typename >
      struct type_to_string;

      template< template< char... > class X, char... Cs >
      struct type_to_string< X< Cs... > >
      {
         static constexpr char array[] = { Cs..., 0 };

         [[nodiscard]] static std::string string()
         {
            return std::string( array, sizeof...( Cs ) );
         }

         [[nodiscard]] static std::string_view string_view() noexcept
         {
            return std::string_view( array, sizeof...( Cs ) );
         }
      };

   }  // namespace internal

   template< typename T >
   [[nodiscard]] std::string type_to_string()
   {
      return internal::type_to_string< T >::string();
   }

   template< char... Cs >
   [[nodiscard]] std::string type_to_string()
   {
      return internal::type_to_string< internal::char_list< Cs... > >::string();
   }

   template< typename T >
   [[nodiscard]] std::string_view type_to_string_view() noexcept
   {
      return internal::type_to_string< T >::string_view();
   }

   template< char... Cs >
   [[nodiscard]] std::string_view type_to_string_view() noexcept
   {
      return internal::type_to_string< internal::char_list< Cs... > >::string_view();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
