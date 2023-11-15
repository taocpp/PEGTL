// Copyright (c) 2016-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ACTION_INPUT_HPP
#define TAO_PEGTL_INTERNAL_ACTION_INPUT_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename ParseInput >
   class action_input
   {
   public:
      using data_t = typename ParseInput::data_t;
      using input_t = ParseInput;
      using rewind_position_t = typename ParseInput::rewind_position_t;

      action_input( const rewind_position_t& in_begin, const ParseInput& in_input ) noexcept
         : m_begin( in_begin ),
           m_input( in_input )
      {}

      action_input( action_input&& ) = delete;
      action_input( const action_input& ) = delete;

      ~action_input() = default;

      void operator=( action_input&& ) = delete;
      void operator=( const action_input& ) = delete;

      [[nodiscard]] const auto* begin() const noexcept
      {
         return m_begin.data;
      }

      [[nodiscard]] const auto* current() const noexcept
      {
         return m_begin.data;
      }

      [[nodiscard]] const auto* end() const noexcept
      {
         return m_input.current();
      }

      [[nodiscard]] bool empty() const noexcept
      {
         return begin() == end();
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return std::size_t( end() - m_begin.data );
      }

      [[nodiscard]] std::string string() const
      {
         return std::string( static_cast< const char* >( m_begin.data ), size() );
      }

      [[nodiscard]] std::string_view string_view() const noexcept
      {
         return std::string_view( static_cast< const char* >( m_begin.data ), size() );
      }

      [[nodiscard]] data_t peek( const std::size_t offset = 0 ) const noexcept
      {
         return m_begin.data[ offset ];
      }

      [[nodiscard]] char peek_char( const std::size_t offset = 0 ) const noexcept
      {
         return static_cast< char >( peek( offset ) );
      }

      [[nodiscard]] std::byte peek_byte( const std::size_t offset = 0 ) const noexcept
      {
         return static_cast< std::byte >( peek( offset ) );
      }

      [[nodiscard]] std::uint8_t peek_uint8( const std::size_t offset = 0 ) const noexcept
      {
         return static_cast< std::uint8_t >( peek( offset ) );
      }

      [[nodiscard]] const ParseInput& input() const noexcept
      {
         return m_input;
      }

      [[nodiscard]] const rewind_position_t& rewind_position() const noexcept
      {
         return m_begin;
      }

      [[nodiscard]] decltype( auto ) current_position() const
      {
         return m_input.previous_position( m_begin );  // NOTE: O(n) with lazy inputs -- n is return value!
      }

   protected:
      const rewind_position_t m_begin;
      const ParseInput& m_input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
