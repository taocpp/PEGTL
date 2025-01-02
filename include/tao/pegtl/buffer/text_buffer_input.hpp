// Copyright (c) 2022-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_TEXT_BUFFER_INPUT_HPP
#define TAO_PEGTL_BUFFER_TEXT_BUFFER_INPUT_HPP

#include <cstddef>

#include "../config.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif
#include "../text_position.hpp"

#include "../internal/choose_text_traits.hpp"

#include "buffer_common.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Buffer, typename Eol >
   class text_buffer_input
      : public buffer_common< Buffer >
   {
   public:
      using data_t = typename Buffer::data_t;
      using error_position_t = text_position;
      using offset_position_t = text_position;
      using rewind_position_t = text_position;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      using eol_rule = Eol;

      using buffer_common< Buffer >::buffer_common;

      text_buffer_input( text_buffer_input&& ) = delete;
      text_buffer_input( const text_buffer_input& ) = delete;

      ~text_buffer_input() = default;

      void operator=( text_buffer_input&& ) = delete;
      void operator=( const text_buffer_input& ) = delete;

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return this->m_current - m_position.count + saved.count;
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         // assert( count <= buffer_used_size() );
         scan_input< data_t > in( this->current(), count );
         choose_text_traits< Eol, Rule >( m_position, in );
         this->m_current += count;
      }

      [[nodiscard]] const auto& rewind_position() const noexcept
      {
         return m_position;
      }

      void rewind_to_position( const rewind_position_t saved ) noexcept
      {
         this->m_current = previous( saved );
         m_position = saved;
      }

      [[nodiscard]] const error_position_t& current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] error_position_t previous_position( const rewind_position_t saved ) const noexcept
      {
         return saved;
      }

      [[nodiscard]] std::size_t direct_count() const noexcept
      {
         return m_position.count;
      }

      [[nodiscard]] std::size_t direct_line() const noexcept
      {
         return m_position.line;
      }

      [[nodiscard]] std::size_t direct_column() const noexcept
      {
         return m_position.column;
      }

      [[nodiscard]] const error_position_t& direct_position() const noexcept
      {
         return m_position;
      }

   protected:
      error_position_t m_position;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
