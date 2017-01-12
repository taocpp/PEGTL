// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_MEMORY_INPUT_HH
#define PEGTL_MEMORY_INPUT_HH

#include <string>
#include <cstddef>
#include <utility>

#include "eol.hh"
#include "position_info.hh"
#include "internal/input_data.hh"
#include "internal/input_mark.hh"
#include "internal/action_input.hh"

namespace pegtl
{
   template< typename Eol >
   class basic_memory_input
   {
   public:
      using eol_t = Eol;

      using data_t = internal::input_data;

      using action_t = internal::action_input< Eol >;
      using memory_t = basic_memory_input< Eol >;

      using position_t = position_info;
      using exception_t = basic_parse_error< position_info >;

      explicit
      basic_memory_input( const internal::input_data & d )
            : m_data( d )
      { }

      template< rewind_mode M >
      basic_memory_input( const internal::input_mark< M > & m, const internal::input_data & d )
            : basic_memory_input( m.byte(), m.line(), m.byte_in_line(), m.begin(), d.begin, d.source )
      { }

      basic_memory_input( const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line, const char * in_begin, const char * in_end, const char * in_source )
            : m_data( in_byte, in_line, in_byte_in_line, in_begin, in_end, in_source )
      { }

      bool empty() const
      {
         return m_data.begin == m_data.end;
      }

      std::size_t size( const std::size_t ) const
      {
         return std::size_t( m_data.end - m_data.begin );
      }

      const char * begin() const
      {
         return m_data.begin;
      }

      const char * end( const std::size_t ) const
      {
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
      { }

      void require( const std::size_t )
      { }

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
      internal::input_data m_data;
   };

   using memory_input = basic_memory_input< lf_crlf_eol >;

} // namespace pegtl

#endif
