// Copyright (c) 2017-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_INPUT_HPP
#define TAO_PEGTL_INTERNAL_TEXT_INPUT_HPP

#include <cstddef>
#include <utility>

#include "../config.hpp"
#include "../pointer_position.hpp"
#include "../rewind_mode.hpp"
#include "../text_position.hpp"

#include "input_with_lines.hpp"
#include "text_eol_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   class text_input
      : public Input
   {
   public:
      using data_t = typename Input::data_t;
      using error_position_t = text_position;
      using rewind_position_t = text_position;

      using eol_rule = Eol;

      using Input::Input;

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
         text_eol_scan< Eol, Rule >( m_position, this->current(), count );
         this->m_current += count;
      }

      template< rewind_mode M >
      [[nodiscard]] auto make_rewind_guard() noexcept
      {
         return rewind_guard< M, text_input >( this );
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( m_position );
      }

      void rewind_to_position( const rewind_position_t& saved ) noexcept
      {
         Input::rewind_to_position( pointer_position< data_t >( previous( saved ) ) );
         m_position = saved;
      }

      [[nodiscard]] const auto& current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] auto previous_position( const rewind_position_t& saved ) const noexcept
      {
         return saved;
      }

      [[nodiscard]] std::size_t direct_count() const noexcept  // TODO: Keep?
      {
         return m_position.count;
      }

      [[nodiscard]] std::size_t direct_line() const noexcept  // TODO: Keep?
      {
         return m_position.line;
      }

      [[nodiscard]] std::size_t direct_column() const noexcept  // TODO: Keep?
      {
         return m_position.column;
      }

      [[nodiscard]] const auto& direct_position() const noexcept
      {
         return m_position;
      }

   protected:
      error_position_t m_position;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
