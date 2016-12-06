// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_MEMORY_INPUT_HH
#define PEGTL_MEMORY_INPUT_HH

#include <string>
#include <cstddef>
#include <utility>

#include "eol_mode.hh"

#include "internal/input_data.hh"
#include "internal/input_mark.hh"

namespace pegtl
{
   template< eol_mode EOL >
   class basic_action_input;

   template< eol_mode EOL >
   class basic_memory_input
   {
   public:
      using Fragment = basic_action_input< EOL >;

   public:
      explicit
      basic_memory_input( const internal::input_data & data )
            : m_data( data )
      { }

      basic_memory_input( const std::size_t in_line, const std::size_t in_byte_in_line, const char * in_begin, const char * in_end, const char * in_source )
            : m_data( in_line, in_byte_in_line, in_begin, in_end, in_source )
      { }

      bool empty() const
      {
         return m_data.begin == m_data.end;
      }

      std::size_t size( const std::size_t ) const
      {
         return m_data.end - m_data.begin;
      }

      const char * begin() const
      {
         return m_data.begin;
      }

      const char * end( const std::size_t ) const
      {
         return m_data.end;
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
         m_data.bump( count, eol );
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

      internal::input_mark mark()
      {
         return internal::input_mark( m_data );
      }

      static constexpr eol_mode eol = EOL;

   private:
      internal::input_data m_data;
   };

   using memory_input = basic_memory_input< eol_mode::LF_WITH_CRLF >;

} // namespace pegtl

#endif
