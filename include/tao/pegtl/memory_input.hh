// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_MEMORY_INPUT_HH
#define TAOCPP_PEGTL_INCLUDE_MEMORY_INPUT_HH

#include <string>
#include <cstddef>
#include <utility>

#include "config.hh"
#include "eol.hh"
#include "count_data.hh"
#include "position_info.hh"
#include "internal/bump_impl.hh"
#include "internal/input_mark.hh"
#include "internal/action_input.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   template< typename Eol >
   class basic_memory_input
   {
   public:
      using eol_t = Eol;
      using action_t = internal::basic_action_input< Eol >;
      using memory_t = basic_memory_input< Eol >;

      basic_memory_input( const char * in_begin, const char * in_end, const char * in_source )
            : basic_memory_input( { 0, 1, 0, in_begin }, in_end, in_source )
      { }

      basic_memory_input( const char * in_begin, const char * in_end, const char * in_source, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line )
            : basic_memory_input( { in_byte, in_line, in_byte_in_line, in_begin }, in_end, in_source )
      { }

      basic_memory_input( const count_data & in_data, const char * in_end, const char * in_source )
            : m_data( in_data ),
              m_end( in_end ),
              m_source( in_source )
      { }

      bool empty() const
      {
         return m_end == m_data.data;
      }

      std::size_t size( const std::size_t ) const
      {
         return std::size_t( m_end - m_data.data );
      }

      const char * begin() const
      {
         return m_data.data;
      }

      const char * end( const std::size_t ) const
      {
         return m_end;
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
         return m_source;
      }

      char peek_char( const std::size_t offset = 0 ) const
      {
         return m_data.data[ offset ];
      }

      unsigned char peek_byte( const std::size_t offset = 0 ) const
      {
         return static_cast< unsigned char >( peek_char( offset ) );
      }

      void bump( const std::size_t in_count = 1 )
      {
         internal::bump( m_data, in_count, Eol::ch );
      }

      void bump_in_this_line( const std::size_t in_count = 1 )
      {
         internal::bump_in_this_line( m_data, in_count );
      }

      void bump_to_next_line( const std::size_t in_count = 1 )
      {
         internal::bump_to_next_line( m_data, in_count );
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

      position_info position() const
      {
         return position_info( m_data, m_source );
      }

      const count_data & count() const
      {
         return m_data;
      }

   private:
      count_data m_data;
      const char * m_end;
      const char * m_source;
   };

   using memory_input = basic_memory_input< lf_crlf_eol >;

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
