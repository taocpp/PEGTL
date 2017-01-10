// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_ACTION_INPUT_HH
#define PEGTL_INTERNAL_ACTION_INPUT_HH

#include <string>
#include <cstddef>

#include "../position_info.hh"

#include "input_data.hh"
#include "input_mark.hh"

namespace pegtl
{
   template< typename Eol >
   class basic_memory_input;

   namespace internal
   {
      template< typename Eol >
      class action_input
      {
      public:
         using eol_t = Eol;

         using data_t = input_data;

         using action_t = action_input< Eol >;
         using memory_t = basic_memory_input< Eol >;

         using position_t = position_info;
         using exception_t = basic_parse_error< position_info >;

         template< rewind_mode M >
         action_input( const input_mark< M > & m, const input_data & d )
               : m_data( m.byte(), m.line(), m.byte_in_line(), m.begin(), d.begin, d.source )
         { }

         action_input( const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line, const char * in_begin, const char * in_end, const char * in_source )
               : m_data( in_byte, in_line, in_byte_in_line, in_begin, in_end, in_source )
         { }

         bool empty() const
         {
            return m_data.begin == m_data.end;
         }

         std::size_t size( const std::size_t = 0 ) const
         {
            return std::size_t( m_data.end - m_data.begin );
         }

         const char * begin() const
         {
            return m_data.begin;
         }

         const char * end( const std::size_t = 0 ) const
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

         const input_data & data() const
         {
            return m_data;
         }

         position_t position() const
         {
            return position_info( m_data );
         }

      private:
         input_data m_data;
      };

   } // namespace internal

} // namespace pegtl

#endif
