// Copyright (c) 2016-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ACTION_INPUT_HPP
#define TAO_PEGTL_INTERNAL_ACTION_INPUT_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "inputerator.hpp"

#include "../config.hpp"
#include "../position.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename ParseInput >
   class action_input
   {
   public:
      using input_t = ParseInput;
      using inputerator_t = typename ParseInput::inputerator_t;

      action_input( const inputerator_t& in_begin, const ParseInput& in_input ) noexcept
         : m_begin( in_begin ),
           m_input( in_input )
      {}

      action_input( const action_input& ) = delete;
      action_input( action_input&& ) = delete;

      ~action_input() = default;

      action_input& operator=( const action_input& ) = delete;
      action_input& operator=( action_input&& ) = delete;

      [[nodiscard]] const inputerator_t& inputerator() const noexcept
      {
         return m_begin;
      }

      [[nodiscard]] const ParseInput& input() const noexcept
      {
         return m_input;
      }

      [[nodiscard]] const char* current() const noexcept
      {
         return begin();
      }

      [[nodiscard]] const char* begin() const noexcept
      {
         if constexpr( std::is_same_v< inputerator_t, const char* > ) {
            return inputerator();
         }
         else {
            return inputerator().data;
         }
      }

      [[nodiscard]] const char* end() const noexcept
      {
         return input().current();
      }

      [[nodiscard]] bool empty() const noexcept
      {
         return begin() == end();
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return std::size_t( end() - begin() );
      }

      [[nodiscard]] std::string string() const
      {
         return std::string( begin(), size() );
      }

      [[nodiscard]] std::string_view string_view() const noexcept
      {
         return std::string_view( begin(), size() );
      }

      [[nodiscard]] char peek_char( const std::size_t offset = 0 ) const noexcept
      {
         return begin()[ offset ];
      }

      [[nodiscard]] std::uint8_t peek_uint8( const std::size_t offset = 0 ) const noexcept
      {
         return static_cast< std::uint8_t >( peek_char( offset ) );
      }

      [[nodiscard]] TAO_PEGTL_NAMESPACE::position position() const
      {
         return input().position( inputerator() );  // NOTE: Not efficient with lazy inputs.
      }

      [[nodiscard]] TAO_PEGTL_NAMESPACE::position current_position() const
      {
         return input().position( inputerator() );  // NOTE: Not efficient with lazy inputs.
      }

   protected:
      const inputerator_t m_begin;
      const ParseInput& m_input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
