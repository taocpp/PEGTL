// Copyright (c) 2016-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ACTION_INPUT_HPP
#define TAO_PEGTL_INTERNAL_ACTION_INPUT_HPP

#include <cstddef>

#include "../config.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif
#include "input_with_funcs.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename ParseInput >
   class action_input_impl
   {
   public:
      using data_t = typename ParseInput::data_t;
      using input_t = ParseInput;
      using error_position_t = typename ParseInput::error_position_t;
      using rewind_position_t = typename ParseInput::rewind_position_t;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      action_input_impl( const rewind_position_t& begin, const ParseInput& input ) noexcept
         : m_saved( begin ),
           m_input( input )
      {}

      action_input_impl( action_input_impl&& ) = delete;
      action_input_impl( const action_input_impl& ) = delete;

      ~action_input_impl() = default;

      void operator=( action_input_impl&& ) = delete;
      void operator=( const action_input_impl& ) = delete;

      [[nodiscard]] const data_t* begin() const noexcept
      {
         return m_input.previous( m_saved );
      }

      [[nodiscard]] const data_t* current( const std::size_t offset = 0 ) const noexcept
      {
         return m_input.previous( m_saved ) + offset;
      }

      [[nodiscard]] const data_t* end() const noexcept
      {
         return m_input.current();
      }

      [[nodiscard]] bool empty() const noexcept
      {
         return begin() == end();
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return std::size_t( end() - begin() );
      }

      [[nodiscard]] const ParseInput& input() const noexcept
      {
         return m_input;
      }

      [[nodiscard]] decltype( auto ) current_position() const
      {
         return m_input.previous_position( m_saved );  // NOTE: O(n) with lazy inputs!
      }

      [[nodiscard]] const rewind_position_t& rewind_position() const noexcept
      {
         return m_saved;
      }

      [[nodiscard]] decltype( auto ) direct_source() const noexcept
      {
         return m_input.direct_source();  // Not all inputs have this.
      }

      [[nodiscard]] decltype( auto ) direct_position() const noexcept
      {
         return m_input.direct_position();  // Not all inputs have this.
      }

   protected:
      const rewind_position_t m_saved;
      const ParseInput& m_input;
   };

   template< typename ParseInput >
   using action_input = input_with_funcs< action_input_impl< ParseInput > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
