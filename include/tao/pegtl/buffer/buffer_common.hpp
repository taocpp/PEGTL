// Copyright (c) 2022-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_BUFFER_COMMON_HPP
#define TAO_PEGTL_BUFFER_BUFFER_COMMON_HPP

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <utility>

#if defined( __cpp_exceptions )
#include <exception>
#include <stdexcept>
#else
#include <cstdio>
#endif

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Buffer >
   class buffer_common
      : public Buffer
   {
   public:
      using data_t = typename Buffer::data_t;

      template< typename... As >
      explicit buffer_common( As&&... as )
         : Buffer( std::forward< As >( as )... ),
           m_current( this->buffer_start() ),
           m_end( this->buffer_start() )
      {}

      [[nodiscard]] bool empty()
      {
         return size( 1 ) == 0;
      }

      [[nodiscard]] std::size_t size( const std::size_t amount )
      {
         require( amount );
         return buffer_used_size();
      }

      [[nodiscard]] std::size_t size( const std::size_t minimum, const std::size_t maximum )
      {
         require( maximum );
         buffer_check_size( minimum );
         return buffer_used_size();
      }

      [[nodiscard]] const data_t* current( const std::size_t offset = 0 ) const noexcept
      {
         return m_current + offset;
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

      void consume( const std::size_t count ) noexcept
      {
         // assert( count <= buffer_used_size() );
         m_current += count;
      }

      void compact() noexcept
      {
         compact( buffer_used_size() );
      }

      void compact( const std::size_t used ) noexcept
      {
         std::memmove( this->mutable_start(), m_current, used );
      }

      void discard() noexcept
      {
         const std::size_t used = buffer_used_size();

         if( ( used > 0 ) && ( m_current > ( this->buffer_start() + this->buffer_chunk_size() ) ) ) {
            compact( used );
         }
         m_current = this->mutable_start();
         m_end = m_current + used;
      }

      void require( const std::size_t amount )
      {
         if( ( m_current + amount ) <= m_end ) {
            return;
         }
         if( ( m_current + amount ) > ( this->buffer_start() + this->buffer_capacity() ) ) {
#if defined( __cpp_exceptions )
            throw std::overflow_error( "require() beyond end of buffer" );
#else
            std::fputs( "overflow error: require() beyond end of buffer\n", stderr );
            std::terminate();
#endif
         }
         m_end += this->m_reader( const_cast< data_t* >( m_end ), ( std::min )( buffer_free_after_end(), ( std::max )( amount - buffer_used_size(), this->buffer_chunk_size() ) ) );
      }

      void buffer_check_size( const std::size_t amount )
      {
         if( buffer_used_size() < amount ) {
#if defined( __cpp_exceptions )
            throw std::overflow_error( "require() beyond end of reader" );
#else
            std::fputs( "overflow error: require() beyond end of reader\n", stderr );
            std::terminate();
#endif
         }
      }

      [[nodiscard]] std::size_t buffer_used_size() const noexcept
      {
         // assert( m_end >= m_current );
         return m_end - m_current;
      }

      [[nodiscard]] std::size_t buffer_free_before_current() const noexcept
      {
         // assert( m_current >= this->buffer_start() );
         return std::size_t( m_current - this->buffer_start() );
      }

      [[nodiscard]] std::size_t buffer_free_after_end() const noexcept
      {
         // assert( this->buffer_start() + m_size >= m_end );
         return std::size_t( this->buffer_start() + this->buffer_capacity() - m_end );
      }

   protected:
      const data_t* m_current;
      const data_t* m_end;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
