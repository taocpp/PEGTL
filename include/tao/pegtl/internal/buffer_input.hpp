// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_BUFFER_INPUT_HPP
#define TAO_PEGTL_INTERNAL_BUFFER_INPUT_HPP

#include <algorithm>
#include <cstring>
#include <utility>

#if defined( __cpp_exceptions )
#include <exception>
#include <stdexcept>
#else
#include <cstdio>
#endif

#include "../config.hpp"
#include "../count_position.hpp"
#include "../pointer_position.hpp"

#include "rewind_guard.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Base >
   class buffer_input
      : public Base
   {
   public:
      using data_t = typename Base::data_t;
      using error_position_t = count_position;
      using rewind_position_t = pointer_position< data_t >;

      template< typename... As >
      buffer_input( As&&... as )
         : Base( std::forward< As >( as )... ),
           m_current( this->mutable_begin() ),
           m_end( this->mutable_begin() )
      {
         // assert( buffer_chunk_size() > 0 );
         // assert( buffer_chunk_size() < buffer_capacity() );
      }

      buffer_input( buffer_input&& ) = delete;
      buffer_input( const buffer_input& ) = delete;

      ~buffer_input() = default;

      void operator=( buffer_input&& ) = delete;
      void operator=( const buffer_input& ) = delete;

      [[nodiscard]] bool empty()
      {
         return size( 1 ) == 0;
      }

      [[nodiscard]] std::size_t size( const std::size_t amount )
      {
         require( amount );
         return this->buffer_size();
      }

      [[nodiscard]] std::size_t size( const std::size_t minimum, const std::size_t maximum )
      {
         require( maximum );
         buffer_check_size( minimum );
         return this->buffer_size();
      }

      [[nodiscard]] const data_t* current( const std::size_t offset = 0 ) const noexcept
      {
         return m_current + offset;
      }

      [[nodiscard]] const data_t* previous( const error_position_t saved ) const noexcept
      {
         return m_current - m_position.count + saved.count;
      }

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return saved.data;
      }

      [[nodiscard]] const data_t* end( const std::size_t amount )
      {
         require( amount );
         return m_end;
      }

      [[nodiscard]] const data_t* end( const std::size_t minimum, const std::size_t maximum )
      {
         require( maximum );
         buffer_check_size( minimum );
         return m_end;
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         m_current += count;
         m_position.count += count;
      }

      void discard() noexcept
      {
         const std::size_t s = m_end - m_current;

         if( ( s > 0 ) && ( m_current > this->buffer_begin() + this->buffer_chunk_size() ) ) {
            std::memmove( this->mutable_begin(), m_current, s );
         }
         m_current = this->mutable_begin();
         m_end = m_current + s;
      }

      void require( const std::size_t amount )
      {
         if( m_current + amount <= m_end ) {
            return;
         }
         if( ( m_current + amount ) > ( this->buffer_begin() + this->buffer_capacity() ) ) {
#if defined( __cpp_exceptions )
            throw std::overflow_error( "require() beyond end of buffer" );
#else
            std::fputs( "overflow error: require() beyond end of buffer\n", stderr );
            std::terminate();
#endif
         }
         m_end += this->m_reader( const_cast< data_t* >( m_end ), ( std::min )( buffer_free_after_end(), ( std::max )( amount - buffer_size(), this->buffer_chunk_size() ) ) );
      }

      void buffer_check_size( const std::size_t amount )
      {
         if( this->buffer_size() < amount ) {
#if defined( __cpp_exceptions )
            throw std::overflow_error( "require() beyond end of reader" );
#else
            std::fputs( "overflow error: require() beyond end of reader\n", stderr );
            std::terminate();
#endif
         }
      }

      [[nodiscard]] std::size_t buffer_size() const noexcept
      {
         // assert( m_end >= m_current );
         return m_end - m_current;
      }

      [[nodiscard]] std::size_t buffer_free_before_current() const noexcept
      {
         // assert( m_current >= this->buffer_begin() );
         return std::size_t( m_current - this->buffer_begin() );
      }

      [[nodiscard]] std::size_t buffer_free_after_end() const noexcept
      {
         // assert( this->buffer_begin() + m_size >= m_end );
         return std::size_t( this->buffer_begin() + this->buffer_capacity() - m_end );
      }

      template< rewind_mode M >
      [[nodiscard]] auto make_rewind_guard() noexcept
      {
         return rewind_guard< M, buffer_input >( this );
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( m_current );
      }

      void rewind_to_position( const rewind_position_t saved ) noexcept
      {
         m_current = saved.data;
      }

      [[nodiscard]] error_position_t current_position() const noexcept
      {
         return m_position;
      }

      [[nodiscard]] error_position_t previous_position( const rewind_position_t saved ) const noexcept
      {
         return error_position_t( m_position.count + saved.data - m_current );
      }

      [[nodiscard]] error_position_t direct_position() const noexcept
      {
         return m_position;
      }

   protected:
      error_position_t m_position;
      const data_t* m_current;
      const data_t* m_end;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
