// Copyright (c) 2022-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_STREAM_STREAM_INPUT_HPP
#define TAO_PEGTL_STREAM_STREAM_INPUT_HPP

#include <cstddef>

#include "../config.hpp"
#include "../count_position.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif
#include "../pointer_position.hpp"

#include "stream_input_base.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Buffer >
   class stream_input
      : public stream_input_base< Buffer >
   {
   public:
      using data_t = typename Buffer::data_t;
      using error_position_t = count_position;
      using offset_position_t = count_position;
      using rewind_position_t = pointer_position< data_t >;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      using stream_input_base< Buffer >::stream_input_base;

      stream_input( stream_input&& ) = delete;
      stream_input( const stream_input& ) = delete;

      ~stream_input() = default;

      void operator=( stream_input&& ) = delete;
      void operator=( const stream_input& ) = delete;

      [[nodiscard]] const data_t* previous( const error_position_t saved ) const noexcept
      {
         return this->m_current - m_position.count + saved.count;
      }

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return saved.data;
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         assert( count <= this->buffer_used_size() );
         this->buffer_assert_consistency();
         this->m_current += count;
         this->buffer_assert_consistency();
         m_position.count += count;
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( this->m_current );
      }

      void rewind_to_position( const rewind_position_t saved ) noexcept
      {
         m_position = previous_position( saved );
         this->buffer_assert_consistency();
         this->m_current = saved.data;
         this->buffer_assert_consistency();
      }

      [[nodiscard]] const error_position_t& current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] error_position_t previous_position( const rewind_position_t saved ) const noexcept
      {
         return error_position_t( m_position.count + saved.data - this->m_current );
      }

      [[nodiscard]] std::size_t direct_count() const noexcept
      {
         return m_position.count;
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
