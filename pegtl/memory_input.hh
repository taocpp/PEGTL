// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_MEMORY_INPUT_HH
#define PEGTL_MEMORY_INPUT_HH

#include <string>
#include <cstddef>

#include "internal/input_data.hh"
#include "internal/input_mark.hh"

namespace pegtl
{
   class memory_input
   {
   public:
      explicit
      memory_input( const internal::input_data & data )
            : m_data( data )
      { }

      memory_input( const std::size_t line, const std::size_t column, const char * begin, const char * end, const char * source )
            : m_data( line, column, begin, end, source )
      { }

      bool empty() const
      {
         return m_data.begin == m_data.end;
      }

      std::size_t size( const size_t ) const
      {
         return m_data.end - m_data.begin;
      }

      const char * begin() const
      {
         return m_data.begin;
      }

      const char * end( const size_t ) const
      {
         return m_data.end;
      }

      std::size_t line() const
      {
         return m_data.line;
      }

      std::size_t column() const
      {
         return m_data.column;
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
         m_data.bump( count );
      }

      void bump_in_line( const std::size_t count = 1 )
      {
         m_data.bump_in_line( count );
      }

      void bump_next_line( const std::size_t count = 1 )
      {
         m_data.bump_next_line( count );
      }

      void discard()
      { }

      void require( const std::size_t )
      { }

      internal::input_mark mark()
      {
         return internal::input_mark( m_data );
      }

      const internal::input_data & data() const
      {
         return m_data;
      }

   private:
      internal::input_data m_data;
   };

} // pegtl

#endif
