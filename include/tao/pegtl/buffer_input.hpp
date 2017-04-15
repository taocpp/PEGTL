// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_BUFFER_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_BUFFER_INPUT_HPP

#include <cstddef>
#include <cstring>
#include <memory>

#include "config.hpp"
#include "eol.hpp"
#include "memory_input.hpp"
#include "position.hpp"
#include "tracking_mode.hpp"

#include "internal/action_input.hpp"
#include "internal/bump_impl.hpp"
#include "internal/iterator.hpp"
#include "internal/marker.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Reader, typename Eol = lf_crlf_eol >
      class buffer_input
      {
      public:
         using reader_t = Reader;
         using eol_t = Eol;

         using memory_t = memory_input< Eol >;
         using action_t = internal::action_input< Eol, tracking_mode::IMMEDIATE >;

         template< typename... As >
         buffer_input( const char* in_source, const std::size_t maximum, As&&... as )
            : m_reader( std::forward< As >( as )... ),
              m_maximum( maximum ),
              m_buffer( new char[ maximum ] ),
              m_data( { 0, 1, 0, m_buffer.get() } ),
              m_end( m_buffer.get() ),
              m_source( in_source )
         {
         }

         template< typename... As >
         buffer_input( const std::string& in_source, As&&... as )
            : buffer_input( in_source.c_str(), std::forward< As >( as )... )
         {
         }

         buffer_input( const buffer_input& ) = delete;
         void operator=( const buffer_input& ) = delete;

         bool empty()
         {
            require( 1 );
            return m_data.data == m_end;
         }

         std::size_t size( const std::size_t amount )
         {
            require( amount );
            return std::size_t( m_end - m_data.data );
         }

         const char* begin() const noexcept
         {
            return m_data.data;
         }

         const char* end( const std::size_t amount )
         {
            require( amount );
            return m_end;
         }

         std::size_t byte() const noexcept
         {
            return m_data.byte;
         }

         std::size_t line() const noexcept
         {
            return m_data.line;
         }

         std::size_t byte_in_line() const noexcept
         {
            return m_data.byte_in_line;
         }

         const char* source() const noexcept
         {
            return m_source;
         }

         char peek_char( const std::size_t offset = 0 ) const noexcept
         {
            return m_data.data[ offset ];
         }

         unsigned char peek_byte( const std::size_t offset = 0 ) const noexcept
         {
            return static_cast< unsigned char >( peek_char( offset ) );
         }

         void bump( const std::size_t in_count = 1 ) noexcept
         {
            internal::bump( m_data, in_count, Eol::ch );
         }

         void bump_in_this_line( const std::size_t in_count = 1 ) noexcept
         {
            internal::bump_in_this_line( m_data, in_count );
         }

         void bump_to_next_line( const std::size_t in_count = 1 ) noexcept
         {
            internal::bump_to_next_line( m_data, in_count );
         }

         void discard() noexcept
         {
            const auto s = m_end - m_data.data;
            std::memmove( m_buffer.get(), m_data.data, s );
            m_data.data = m_buffer.get();
            m_end = m_buffer.get() + s;
         }

         void require( const std::size_t amount )
         {
            if( m_data.data + amount > m_end ) {
               if( m_data.data + amount <= m_buffer.get() + m_maximum ) {
                  if( const auto r = m_reader( const_cast< char* >( m_end ), amount - std::size_t( m_end - m_data.data ) ) ) {
                     m_end += r;
                  }
                  else {
                     m_maximum = 0;
                  }
               }
            }
         }

         template< rewind_mode M >
         internal::marker< internal::iterator, M > mark() noexcept
         {
            return internal::marker< internal::iterator, M >( m_data );
         }

         TAOCPP_PEGTL_NAMESPACE::position position() const noexcept
         {
            return TAOCPP_PEGTL_NAMESPACE::position( m_data, m_source );
         }

         const internal::iterator& iterator() const noexcept
         {
            return m_data;
         }

      private:
         Reader m_reader;
         std::size_t m_maximum;
         std::unique_ptr< char[] > m_buffer;
         internal::iterator m_data;
         const char* m_end;
         const char* const m_source;
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
