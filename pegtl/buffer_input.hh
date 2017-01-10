// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_BUFFER_INPUT_HH
#define PEGTL_BUFFER_INPUT_HH

#include <memory>
#include <cstring>
#include <cstddef>

#include "eol.hh"
#include "position_info.hh"
#include "internal/input_data.hh"
#include "internal/input_mark.hh"
#include "internal/action_input.hh"

namespace pegtl
{
   template< typename Eol >
   class basic_memory_input;

   template< typename Eol, typename Reader >
   class basic_buffer_input
   {
   public:
      using eol_t = Eol;

      using reader_t = Reader;

      using data_t = internal::input_data;

      using action_t = internal::action_input< Eol >;
      using memory_t = basic_memory_input< Eol >;

      using position_t = position_info;
      using exception_t = basic_parse_error< position_info >;


      template< typename ... As >
      basic_buffer_input( const char * in_source, const std::size_t maximum, As && ... as )
            : m_reader( std::forward< As >( as ) ... ),
              m_maximum( maximum ),
              m_buffer( new char[ maximum ] ),
              m_data( 0, 1, 0, m_buffer.get(), m_buffer.get(), in_source )

      { }

      basic_buffer_input( const basic_buffer_input & ) = delete;
      void operator= ( const basic_buffer_input & ) = delete;

      bool empty()
      {
         require( 1 );
         return m_data.begin == m_data.end;
      }

      std::size_t size( const std::size_t amount )
      {
         require( amount );
         return std::size_t( m_data.end - m_data.begin );
      }

      const char * begin() const
      {
         return m_data.begin;
      }

      const char * end( const std::size_t amount )
      {
         require( amount );
         return m_data.end;
      }

      std::size_t byte() const
      {
         return m_data.byte;
      }

      std::size_t line() const
      {
         return m_data.line;
      }

      std::size_t byte_in_line() const
      {
         return m_data.byte_in_line;
      }

      const char * source() const
      {
         return m_data.source;
      }

      char peek_char( const std::size_t offset = 0 ) const
      {
         return m_data.begin[ offset ];
      }

      unsigned char peek_byte( const std::size_t offset = 0 ) const
      {
         return static_cast< unsigned char >( peek_char( offset ) );
      }

      void bump( const std::size_t count = 1 )
      {
         m_data.bump( count, Eol::ch );
      }

      void bump_in_this_line( const std::size_t count = 1 )
      {
         m_data.bump_in_this_line( count );
      }

      void bump_to_next_line( const std::size_t count = 1 )
      {
         m_data.bump_to_next_line( count );
      }

      void discard()
      {
         const auto s = m_data.end - m_data.begin;
         std::memmove( m_buffer.get(), m_data.begin, s );
         m_data.begin = m_buffer.get();
         m_data.end = m_buffer.get() + s;
      }

      void require( const std::size_t amount )
      {
         if ( m_data.begin + amount > m_data.end ) {
            if ( m_data.begin + amount <= m_buffer.get() + m_maximum ) {
               if ( const auto r = m_reader( const_cast< char * >( m_data.end ), amount - std::size_t( m_data.end - m_data.begin ) ) ) {
                  m_data.end += r;
               }
               else {
                  m_maximum = 0;
               }
            }
         }
      }

      template< rewind_mode M >
      internal::input_mark< M > mark()
      {
         return internal::input_mark< M >( m_data );
      }

      const internal::input_data & data() const
      {
         return m_data;
      }

      position_t position() const
      {
         return position_info( m_data );
      }

   private:
      Reader m_reader;
      std::size_t m_maximum;
      std::unique_ptr< char[] > m_buffer;
      internal::input_data m_data;
   };

   template< typename Reader >
   using buffer_input = basic_buffer_input< lf_crlf_eol, Reader >;

} // namespace pegtl

#endif
