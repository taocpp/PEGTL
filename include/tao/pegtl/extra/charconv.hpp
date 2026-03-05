// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_CHARCONV_HPP
#define TAO_PEGTL_EXTRA_CHARCONV_HPP

#include <cstdint>

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"

#include "internal/charconv.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   template< std::uint8_t Base = 0 >
   struct from_chars_raw : internal::from_chars_auto< Base > {};
   struct from_chars_bin : internal::from_chars_auto< 2 > {};
   struct from_chars_oct : internal::from_chars_auto< 8 > {};
   struct from_chars_dec : internal::from_chars_auto< 10 > {};
   struct from_chars_hex : internal::from_chars_auto< 16 > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_int8 : internal::from_chars_type< Base, std::int8_t > {};
   struct from_chars_bin_int8 : internal::from_chars_type< 2, std::int8_t > {};
   struct from_chars_oct_int8 : internal::from_chars_type< 8, std::int8_t > {};
   struct from_chars_dec_int8 : internal::from_chars_type< 10, std::int8_t > {};
   struct from_chars_hex_int8 : internal::from_chars_type< 16, std::int8_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_uint8 : internal::from_chars_type< Base, std::uint8_t > {};
   struct from_chars_bin_uint8 : internal::from_chars_type< 2, std::uint8_t > {};
   struct from_chars_oct_uint8 : internal::from_chars_type< 8, std::uint8_t > {};
   struct from_chars_dec_uint8 : internal::from_chars_type< 10, std::uint8_t > {};
   struct from_chars_hex_uint8 : internal::from_chars_type< 16, std::uint8_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_int16 : internal::from_chars_type< Base, std::int16_t > {};
   struct from_chars_bin_int16 : internal::from_chars_type< 2, std::int16_t > {};
   struct from_chars_oct_int16 : internal::from_chars_type< 8, std::int16_t > {};
   struct from_chars_dec_int16 : internal::from_chars_type< 10, std::int16_t > {};
   struct from_chars_hex_int16 : internal::from_chars_type< 16, std::int16_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_uint16 : internal::from_chars_type< Base, std::uint16_t > {};
   struct from_chars_bin_uint16 : internal::from_chars_type< 2, std::uint16_t > {};
   struct from_chars_oct_uint16 : internal::from_chars_type< 8, std::uint16_t > {};
   struct from_chars_dec_uint16 : internal::from_chars_type< 10, std::uint16_t > {};
   struct from_chars_hex_uint16 : internal::from_chars_type< 16, std::uint16_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_int32 : internal::from_chars_type< Base, std::int32_t > {};
   struct from_chars_bin_int32 : internal::from_chars_type< 2, std::int32_t > {};
   struct from_chars_oct_int32 : internal::from_chars_type< 8, std::int32_t > {};
   struct from_chars_dec_int32 : internal::from_chars_type< 10, std::int32_t > {};
   struct from_chars_hex_int32 : internal::from_chars_type< 16, std::int32_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_uint32 : internal::from_chars_type< Base, std::uint32_t > {};
   struct from_chars_bin_uint32 : internal::from_chars_type< 2, std::uint32_t > {};
   struct from_chars_oct_uint32 : internal::from_chars_type< 8, std::uint32_t > {};
   struct from_chars_dec_uint32 : internal::from_chars_type< 10, std::uint32_t > {};
   struct from_chars_hex_uint32 : internal::from_chars_type< 16, std::uint32_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_int64 : internal::from_chars_type< Base, std::int64_t > {};
   struct from_chars_bin_int64 : internal::from_chars_type< 2, std::int64_t > {};
   struct from_chars_oct_int64 : internal::from_chars_type< 8, std::int64_t > {};
   struct from_chars_dec_int64 : internal::from_chars_type< 10, std::int64_t > {};
   struct from_chars_hex_int64 : internal::from_chars_type< 16, std::int64_t > {};

   template< std::uint8_t Base = 0 >
   struct from_chars_raw_uint64 : internal::from_chars_type< Base, std::uint64_t > {};
   struct from_chars_bin_uint64 : internal::from_chars_type< 2, std::uint64_t > {};
   struct from_chars_oct_uint64 : internal::from_chars_type< 8, std::uint64_t > {};
   struct from_chars_dec_uint64 : internal::from_chars_type< 10, std::uint64_t > {};
   struct from_chars_hex_uint64 : internal::from_chars_type< 16, std::uint64_t > {};
   // clang-format on

   template< typename Name, std::uint8_t Base >
   struct analyze_traits< Name, internal::from_chars_rule_auto< Base > >
      : analyze_any_traits<>
   {};

   template< typename Name, std::uint8_t Base, typename Integral >
   struct analyze_traits< Name, internal::from_chars_rule_type< Base, Integral > >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
