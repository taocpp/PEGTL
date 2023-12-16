// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INTEGER_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_INTEGER_TRAITS_HPP

#include <cstdint>
#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< unsigned Size >
   struct integer_traits;

   template<>
   struct integer_traits< 1 >
   {
      using signed_t = std::int8_t;
      using unsigned_t = std::uint8_t;
   };

   template<>
   struct integer_traits< 2 >
   {
      using signed_t = std::int16_t;
      using unsigned_t = std::uint16_t;
   };

   template<>
   struct integer_traits< 4 >
   {
      using signed_t = std::int32_t;
      using unsigned_t = std::uint32_t;
   };

   template<>
   struct integer_traits< 8 >
   {
      using signed_t = std::int64_t;
      using unsigned_t = std::uint64_t;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
