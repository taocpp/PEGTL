// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_DIRECT_HPP
#define TAO_PEGTL_INTERNAL_PEEK_DIRECT_HPP

#include <cstddef>
#include <cstdint>

#include "data_and_size.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   struct peek_direct
   {
      using data_t = Data;
      using pair_t = data_and_size< Data, void >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 1;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         using raw_t = typename ParseInput::data_t;

         static_assert( is_simple_type_v< raw_t > );
         static_assert( sizeof( data_t ) == sizeof( raw_t ) );

         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? reinterpret_cast< const data_t* >( in.current( offset ) ) : nullptr );
      }
   };

   template<>
   struct peek_direct< void >
   {
      // using data_t = Depends on ParseInput.
      // using pair_t = Depends on ParseInput.

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 1;
      }

      template< typename ParseInput >
      [[nodiscard]] static auto peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? in.current( offset ) : nullptr );
      }
   };

   using peek_data = peek_direct< void >;

   using peek_char = peek_direct< char >;
   using peek_byte = peek_direct< std::byte >;

   // using peek_char8 = peek_direct< char8_t >;  // C++20
   using peek_char16 = peek_direct< char16_t >;
   using peek_char32 = peek_direct< char32_t >;

   using peek_int8 = peek_direct< std::int8_t >;
   using peek_uint8 = peek_direct< std::uint8_t >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
