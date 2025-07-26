// Copyright (c) 2016-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_ENDLESS_STREAM_HPP
#define TAO_PEGTL_SRC_TEST_ENDLESS_STREAM_HPP

#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

#include <tao/pegtl/stream.hpp>

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct endless_reader
   {
      explicit endless_reader( const char data )
         : m_endless( std::string::size_type( 1 ), data )
      {}

      explicit endless_reader( std::string&& data )
         : m_endless( std::move( data ) )
      {
         assert( !m_endless.empty() );
      }

      explicit endless_reader( const std::string& data )
         : m_endless( data )
      {
         assert( !m_endless.empty() );
      }

      [[nodiscard]] std::size_t operator()( char* buffer, const std::size_t length ) noexcept
      {
         if( m_endless.size() == 1 ) {
            std::memset( buffer, m_endless[ 0 ], length );
            return length;
         }
         for( std::size_t i = 0; i < length; ++i ) {
            buffer[ i ] = m_endless[ ( m_offset + i ) % m_endless.size() ];
         }
         m_offset += length;
         m_offset %= m_endless.size();
         return length;
      }

   protected:
      std::size_t m_offset = 0;
      std::string m_endless;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

namespace TAO_PEGTL_NAMESPACE
{
   using alloc_endless_buffer = internal::alloc_buffer< char, internal::endless_reader >;

   template< std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using array_endless_buffer = internal::array_buffer< char, internal::endless_reader, BufferSize, ChunkSize >;

   using other_endless_buffer = internal::other_buffer< char, internal::endless_reader >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using alloc_endless_input = stream_input< alloc_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using array_endless_input = stream_input< array_endless_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using other_endless_input = stream_input< other_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using alloc_text_endless_input = text_stream_input< alloc_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using array_text_endless_input = text_stream_input< array_endless_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using other_text_endless_input = text_stream_input< other_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using alloc_endless_auto_input = stream_auto_input< alloc_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using array_endless_auto_input = stream_auto_input< array_endless_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using other_endless_auto_input = stream_auto_input< other_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using alloc_text_endless_auto_input = text_stream_auto_input< alloc_endless_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using array_text_endless_auto_input = text_stream_auto_input< array_endless_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_stream_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using other_text_endless_auto_input = text_stream_auto_input< other_endless_buffer, Eol, InputSource, ErrorSource >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
