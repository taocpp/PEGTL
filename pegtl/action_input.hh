// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ACTION_INPUT_HH
#define PEGTL_ACTION_INPUT_HH

#include <string>
#include <cstddef>

#include "internal/input_data.hh"
#include "internal/input_mark.hh"

namespace pegtl
{
   class action_input
   {
   public:
      action_input( const internal::input_data & data, const internal::input_mark & mark )
            : m_data( mark.m_line, mark.m_column, mark.m_begin, data.begin, data.source, data.from )
      { }

      action_input( const std::size_t line, const std::size_t column, const char * begin, const char * end, const char * source )
            : m_data( line, column, begin, end, source )
      { }

      bool empty() const
      {
         return m_data.begin == m_data.end;
      }

      std::size_t size( const size_t = 0 ) const
      {
         return m_data.end - m_data.begin;
      }

      const char * begin() const
      {
         return m_data.begin;
      }

      const char * end( const size_t = 0 ) const
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

      std::string string() const
      {
         return std::string( m_data.begin, m_data.end );
      }

      char peek_char( const std::size_t offset = 0 ) const
      {
         return m_data.begin[ offset ];
      }

      unsigned char peek_byte( const std::size_t offset = 0 ) const
      {
         return static_cast< unsigned char >( peek_char( offset ) );
      }

      const internal::input_data & data() const
      {
         return m_data;
      }

      internal::input_data & internal_data()
      {
         return m_data;
      }

   private:
      internal::input_data m_data;
   };

} // pegtl

#endif
