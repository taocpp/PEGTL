// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_ACTION_INPUT_HH
#define PEGTL_INTERNAL_ACTION_INPUT_HH

#include <string>
#include <cstddef>

#include "../config.hh"
#include "../count_data.hh"
#include "../position_info.hh"

namespace PEGTL_NAMESPACE
{
   template< typename Eol >
   class basic_memory_input;

   namespace internal
   {
      template< typename Eol >
      class basic_action_input
      {
      public:
         using eol_t = Eol;
         using action_t = basic_action_input< Eol >;
         using memory_t = basic_memory_input< Eol >;
         using position_t = position_info;
         using exception_t = basic_parse_error< position_info >;

         basic_action_input( const count_data & in_data, const char * in_end, const char * in_source )
               : m_data( in_data ),
                 m_end( in_end ),
                 m_source( in_source )
         { }

         bool empty() const
         {
            return m_data.data == m_end;
         }

         std::size_t size( const std::size_t = 0 ) const
         {
            return std::size_t( m_end - m_data.data );
         }

         const char * begin() const
         {
            return m_data.data;
         }

         const char * end( const std::size_t = 0 ) const
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

         std::string string() const
         {
            return std::string( begin(), end() );
         }

         char peek_char( const std::size_t offset = 0 ) const
         {
            return m_data.data[ offset ];
         }

         unsigned char peek_byte( const std::size_t offset = 0 ) const
         {
            return static_cast< unsigned char >( peek_char( offset ) );
         }

         const count_data & count() const
         {
            return m_data;
         }

         position_t position() const
         {
            return position_info( m_data, m_source );
         }

      private:
         count_data m_data;
         const char * m_end;
         const char * m_source;
      };

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
