// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_TEXT_INPUT_WITH_SOURCE_HPP

#include <cstddef>
#include <utility>

#include "../config.hpp"
#include "../position_with_source.hpp"
#include "../text_position.hpp"

#include "input_with_lines.hpp"
#include "text_eol_bump.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Source, typename Input >
   class text_input_with_source
      : public input_with_lines< Eol, Input >
   {
   public:
      using base_t = input_with_lines< Eol, Input >;
      using data_t = typename Input::data_t;
      using error_position_t = position_with_source< Source, text_position >;
      using rewind_position_t = text_position;

      using eol_rule = Eol;

      template< typename S, typename... Ts >
      text_input_with_source( S&& s, Ts&&... ts )
         : input_with_lines< Eol, Input >( std::forward< Ts >( ts )... ),
           m_position( std::forward< S >( s ) )
      {}

      void restart() noexcept
      {
         base_t::restart();

         m_position.count = 0;
         m_position.line = 1;
         m_position.column = 1;
      };

      void restart( const rewind_position_t& initial ) noexcept
      {
         base_t::restart();

         m_position = initial;
      }

      [[nodiscard]] const char* previous( const rewind_position_t& saved ) const noexcept
      {
         return this->current() - m_position.count + saved.count;
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         text_eol_bump< Eol, Rule >( m_position, this->current(), count );
         Input::template consume< Rule >( count );
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
         base_t::rewind_to_position( pointer_position< data_t >( previous( saved ) ) );

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

      [[nodiscard]] const Source& direct_source() const noexcept  // TODO: Keep?
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

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif