// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ACTION_INPUT_HH
#define PEGTL_ACTION_INPUT_HH

#include <string>
#include <cstddef>

#include "eol_mode.hh"

#include "internal/input_data.hh"
#include "internal/input_mark.hh"

namespace pegtl
{
   template< eol_mode EOL >
   class basic_action_input
   {
   public:

      basic_action_input( const std::size_t in_line, const std::size_t in_byte_in_line, const char * in_begin, const char * in_end, const char * in_source )
            : m_data( in_line, in_byte_in_line, in_begin, in_end, in_source )
      { }

      basic_action_input( const internal::input_mark& from, const internal::input_mark& to )
            : m_data( from.line(), from.byte_in_line(), from.begin(), to.begin(), from.source() )
      { }

      bool empty() const
      {
         return m_data.begin == m_data.end;
      }

      std::size_t size( const std::size_t = 0 ) const
      {
         return m_data.end - m_data.begin;
      }

      const char * begin() const
      {
         return m_data.begin;
      }

      const char * end( const std::size_t = 0 ) const
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

      static constexpr eol_mode eol = EOL;

   private:
      internal::input_data m_data;
   };

   using action_input = basic_action_input< eol_mode::LF_WITH_CRLF >;

} // namespace pegtl

#endif
