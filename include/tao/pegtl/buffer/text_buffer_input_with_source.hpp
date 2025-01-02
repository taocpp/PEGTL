// Copyright (c) 2022-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_TEXT_BUFFER_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_BUFFER_TEXT_BUFFER_INPUT_WITH_SOURCE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

#include "../config.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif
#include "../position_with_source.hpp"
#include "../text_position.hpp"

#include "../internal/choose_text_traits.hpp"

#include "buffer_common.hpp"
#include "text_buffer_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Buffer, typename Eol, typename InputSource, typename ErrorSource >
   class text_buffer_input_with_source
      : public buffer_common< Buffer >
   {
   public:
      using data_t = typename Buffer::data_t;
      using input_source_t = InputSource;
      using error_source_t = ErrorSource;
      using error_position_t = position_with_source< ErrorSource, text_position >;
      using offset_position_t = text_position;
      using rewind_position_t = text_position;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      using eol_rule = Eol;

      static_assert( std::is_same_v< InputSource, ErrorSource >, "TODO?" );

      template< typename S, typename... Ts >
      text_buffer_input_with_source( S&& s, Ts&&... ts )
         : buffer_common< Buffer >( std::forward< Ts >( ts )... ),
           m_position( std::forward< S >( s ) )
      {}

      text_buffer_input_with_source( text_buffer_input_with_source&& ) = delete;
      text_buffer_input_with_source( const text_buffer_input_with_source& ) = delete;

      ~text_buffer_input_with_source() = default;

      void operator=( text_buffer_input_with_source&& ) = delete;
      void operator=( const text_buffer_input_with_source& ) = delete;

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
         return static_cast< const rewind_position_t& >( m_position );
      }

      void rewind_to_position( const rewind_position_t& saved ) noexcept
      {
         this->m_current = previous( saved );
         m_position.base() = saved;
      }

      [[nodiscard]] const error_position_t& current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] error_position_t previous_position( const rewind_position_t saved ) const noexcept
      {
         return error_position_t( m_position.source, saved );
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

      [[nodiscard]] const error_source_t& direct_source() const noexcept
      {
         return m_position.source;
      }

      [[nodiscard]] const error_position_t& direct_position() const noexcept
      {
         return m_position;
      }

   protected:
      error_position_t m_position;
   };

   template< typename Buffer, typename Eol >
   class text_buffer_input_with_source< Buffer, Eol, void, void >
      : public text_buffer_input< Buffer, Eol >
   {
   public:
      using text_buffer_input< Buffer, Eol >::text_buffer_input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
