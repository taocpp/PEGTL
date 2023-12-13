// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FILE_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_FILE_INPUT_WITH_SOURCE_HPP

#include <utility>

#include "../config.hpp"
#include "../position_with_source.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Source, typename Input >
   class file_input_with_source
      : public Input
   {
   public:
      using base_t = Input;
      using data_t = typename Input::data_t;
      using error_position_t = position_with_source< Source, typename Input::error_position_t >;
      using rewind_position_t = typename Input::rewind_position_t;
#error
      template< typename S, typename... Ts >
      file_input_with_source( S&& s, Ts&&... ts )
         : m_source( std::forward< S >( s ) ),
           Input( std::forward< Ts >( ts )... )
      {}

      [[nodiscard]] const auto& current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] const auto& previous_position( const rewind_position_t& saved ) const
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

      [[nodiscard]] const Source& direct_source() const noexcept  // TODO: Keep?
      {
         return m_source;
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
