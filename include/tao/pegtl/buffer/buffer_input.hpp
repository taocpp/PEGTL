// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_BUFFER_INPUT_HPP
#define TAO_PEGTL_BUFFER_BUFFER_INPUT_HPP

#include <algorithm>
#include <cstring>
#include <utility>

#include "../config.hpp"
#include "../count_position.hpp"
#include "../pointer_position.hpp"

#include "../internal/rewind_guard.hpp"

#include "buffer_common.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Buffer >
   class buffer_input
      : public buffer_common< Buffer >
   {
   public:
      using data_t = typename Buffer::data_t;
      using error_position_t = count_position;
      using offset_position_t = count_position;
      using rewind_position_t = pointer_position< data_t >;

      using buffer_common< Buffer >::buffer_common;

      buffer_input( buffer_input&& ) = delete;
      buffer_input( const buffer_input& ) = delete;

      ~buffer_input() = default;

      void operator=( buffer_input&& ) = delete;
      void operator=( const buffer_input& ) = delete;

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
         // assert( count <= buffer_used_size() );
         this->m_current += count;
         m_position.count += count;
      }

      template< rewind_mode M >
      [[nodiscard]] auto make_rewind_guard() noexcept
      {
         return rewind_guard< M, buffer_input >( this );
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( this->m_current );
      }

      void rewind_to_position( const rewind_position_t saved ) noexcept
      {
         m_position = previous_position( saved );
         this->m_current = saved.data;
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
