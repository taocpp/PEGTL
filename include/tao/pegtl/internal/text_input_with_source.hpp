// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_TEXT_INPUT_WITH_SOURCE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

#include "../config.hpp"
#include "../position_with_source.hpp"
#include "../text_position.hpp"

#include "choose_text_traits.hpp"
#include "text_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename InputSource, typename ErrorSource, typename Input >
   class text_input_with_source
      : public Input
   {
   public:
      using data_t = typename Input::data_t;
      using input_source_t = InputSource;
      using error_source_t = ErrorSource;
      using error_position_t = position_with_source< ErrorSource, text_position >;
      using offset_position_t = text_position;
      using rewind_position_t = text_position;

      using eol_rule = Eol;

      static_assert( !std::is_same_v< Eol, void > );

      static_assert( std::is_same_v< InputSource, ErrorSource >, "TODO?" );

      template< typename S, typename... Ts >
      text_input_with_source( S&& s, Ts&&... ts )
         : Input( std::forward< Ts >( ts )... ),
           m_position( std::forward< S >( s ) )
      {}

      [[nodiscard]] const data_t* start() const noexcept
      {
         return this->m_current - m_position.count;
      }

      void restart() noexcept
      {
         this->m_current -= m_position.count;
         m_position.count = 0;
         m_position.line = 1;
         m_position.column = 1;
      };

      [[nodiscard]] const data_t* previous( const rewind_position_t& saved ) const noexcept
      {
         return this->current() - m_position.count + saved.count;
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         scan_input< data_t > in( this->current(), count );
         choose_text_traits< Eol, Rule >( m_position, in );
         this->m_current += count;
      }

      template< rewind_mode M >
      [[nodiscard]] auto make_rewind_guard() noexcept
      {
         return rewind_guard< M, text_input_with_source >( this );
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( m_position );
      }

      void rewind_to_position( const rewind_position_t& saved ) noexcept
      {
         this->m_current = previous( saved );
         m_position.base() = saved;
      }

      [[nodiscard]] const auto& current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] auto previous_position( const rewind_position_t& saved ) const
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

      [[nodiscard]] const InputSource& direct_source() const noexcept
      {
         return m_position.source;
      }

      [[nodiscard]] const auto& direct_position() const noexcept
      {
         return m_position;
      }

   protected:
      error_position_t m_position;
   };

   template< typename Eol, typename Input >
   class text_input_with_source< Eol, void, void, Input >
      : public text_input< Eol, Input >
   {
   public:
      using text_input< Eol, Input >::text_input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
