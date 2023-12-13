// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_PEEKS_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_PEEKS_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   class input_with_peeks
      : public Input
   {
   public:
      using Input::Input;

      using data_t = typename Input::data_t;

      [[nodiscard]] const data_t& peek( const std::size_t offset = 0 ) const noexcept
      {
         return *this->current( offset );
      }

      template< typename T >
      [[nodiscard]] T peek_as( const std::size_t offset = 0 ) const noexcept
      {
         static_assert( sizeof( T ) == sizeof( data_t ) );
         return static_cast< T >( *this->current( offset ) );
      }

      [[nodiscard]] char peek_char( const std::size_t offset = 0 ) const noexcept
      {
         return peek_as< char >( offset );
      }

      [[nodiscard]] std::byte peek_byte( const std::size_t offset = 0 ) const noexcept
      {
         return peek_as< std::byte >( offset );
      }

      [[nodiscard]] std::int8_t peek_int8( const std::size_t offset = 0 ) const noexcept
      {
         return peek_as< std::int8_t >( offset );
      }

      [[nodiscard]] std::uint8_t peek_uint8( const std::size_t offset = 0 ) const noexcept
      {
         return peek_as< std::uint8_t >( offset );
      }

      // [[nodiscard]] std::span< data_t > span() const noexcept

      [[nodiscard]] std::string string() const
      {
         static_assert( sizeof( char ) == sizeof( data_t ) );
         return std::string( static_cast< const char* >( this->current() ), this->size() );
      }

      [[nodiscard]] std::string_view string_view() const noexcept
      {
         static_assert( sizeof( char ) == sizeof( data_t ) );
         return std::string_view( static_cast< const char* >( this->current() ), this->size() );
      }

      [[nodiscard]] std::vector< data_t > vector() const
      {
         return std::vector< data_t >( this->current(), this->current() + this->size() );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
