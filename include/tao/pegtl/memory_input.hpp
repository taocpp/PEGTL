// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_MEMORY_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_MEMORY_INPUT_HPP

#include <cstddef>
#include <string>
#include <utility>

#include "config.hpp"
#include "eol.hpp"
#include "position.hpp"
#include "position_tracking.hpp"

#include "internal/bump_impl.hpp"
#include "internal/iterator.hpp"
#include "internal/marker.hpp"
#include "internal/memory_action_input.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol, position_tracking >
      class basic_memory_input;

      template< typename Eol >
      class basic_memory_input< Eol, position_tracking::IMMEDIATE >
      {
      public:
         using eol_t = Eol;
         using memory_t = basic_memory_input;
         using action_t = internal::basic_memory_action_input< Eol, position_tracking::IMMEDIATE >;

         basic_memory_input( const internal::iterator& in_iter, const char* in_end, const char* in_source )
            : m_data( in_iter ),
              m_end( in_end ),
              m_source( in_source )
         {
         }

         basic_memory_input( const char* in_begin, const char* in_end, const char* in_source )
            : basic_memory_input( { 0, 1, 0, in_begin }, in_end, in_source )
         {
         }

         basic_memory_input( const char* in_begin, const std::size_t in_size, const char* in_source )
            : basic_memory_input( in_begin, in_begin + in_size, in_source )
         {
         }

         basic_memory_input( const std::string& in, const char* in_source )
            : basic_memory_input( in.data(), in.size(), in_source )
         {
         }

         basic_memory_input( const char* in_begin, const char* in_end, const char* in_source, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line )
            : basic_memory_input( { in_byte, in_line, in_byte_in_line, in_begin }, in_end, in_source )
         {
         }

         const char* begin() const noexcept
         {
            return m_data.data;
         }

         const char* end( const std::size_t ) const noexcept
         {
            return m_end;
         }

         bool empty() const noexcept
         {
            return begin() == m_end;
         }

         std::size_t size( const std::size_t ) const noexcept
         {
            return std::size_t( m_end - begin() );
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
            return begin()[ offset ];
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

         void discard() const noexcept
         {
         }

         void require( const std::size_t ) const noexcept
         {
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
         internal::iterator m_data;
         const char* const m_end;
         const char* const m_source;
      };

      template< typename Eol >
      class basic_memory_input< Eol, position_tracking::LAZY >
      {
      public:
         using eol_t = Eol;
         using memory_t = basic_memory_input;
         using action_t = internal::basic_memory_action_input< Eol, position_tracking::LAZY >;

         basic_memory_input( const char* in_begin, const char* in_end, const char* in_source ) noexcept
            : m_all( in_begin ),
              m_run( in_begin ),
              m_end( in_end ),
              m_source( in_source )
         {
         }

         const char* begin() const noexcept
         {
            return m_run;
         }

         const char* end( const std::size_t ) const noexcept
         {
            return m_end;
         }

         bool empty() const noexcept
         {
            return begin() == m_end;
         }

         std::size_t size( const std::size_t ) const noexcept
         {
            return std::size_t( m_end - begin() );
         }

         std::size_t byte() const noexcept
         {
            return std::size_t( begin() - m_all );
         }

         const char* source() const noexcept
         {
            return m_source;
         }

         char peek_char( const std::size_t offset = 0 ) const noexcept
         {
            return begin()[ offset ];
         }

         unsigned char peek_byte( const std::size_t offset = 0 ) const noexcept
         {
            return static_cast< unsigned char >( peek_char( offset ) );
         }

         void bump( const std::size_t in_count = 1 ) noexcept
         {
            m_run += in_count;
         }

         void bump_in_this_line( const std::size_t in_count = 1 ) noexcept
         {
            m_run += in_count;
         }

         void bump_to_next_line( const std::size_t in_count = 1 ) noexcept
         {
            m_run += in_count;
         }

         void discard() const noexcept
         {
         }

         void require( const std::size_t ) const noexcept
         {
         }

         template< rewind_mode M >
         internal::marker< const char*, M > mark() noexcept
         {
            return internal::marker< const char*, M >( m_run );
         }

         TAOCPP_PEGTL_NAMESPACE::position position() const noexcept
         {
            internal::iterator c{ 0, 1, 0, m_all };
            internal::bump( c, m_run - m_all, eol_t::ch );
            return TAOCPP_PEGTL_NAMESPACE::position( c, m_source );
         }

         const char* iterator() const noexcept
         {
            return m_run;
         }

      private:
         const char* const m_all;
         const char* m_run;
         const char* const m_end;
         const char* const m_source;
      };

      template< position_tracking P = position_tracking::IMMEDIATE >
      using memory_input = basic_memory_input< lf_crlf_eol, P >;

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
