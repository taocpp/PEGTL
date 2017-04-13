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

#include "internal/bump_impl.hpp"
#include "internal/iterator.hpp"
#include "internal/memory_action_input.hpp"
#include "internal/memory_mark.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol >
      class basic_memory_input
      {
      public:
         using eol_t = Eol;
         using memory_t = basic_memory_input< Eol >;
         using action_t = internal::basic_memory_action_input< Eol >;

         basic_memory_input( const char* in_begin, const char* in_end, const char* in_source )
            : basic_memory_input( { 0, 1, 0, in_begin }, in_end, in_source )
         {
         }

         basic_memory_input( const char* in_begin, const char* in_end, const char* in_source, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line )
            : basic_memory_input( { in_byte, in_line, in_byte_in_line, in_begin }, in_end, in_source )
         {
         }

         basic_memory_input( const internal::iterator& in_iter, const char* in_end, const char* in_source )
            : m_data( in_iter ),
              m_end( in_end ),
              m_source( in_source )
         {
         }

         bool empty() const noexcept
         {
            return m_end == m_data.data;
         }

         std::size_t size( const std::size_t ) const noexcept
         {
            return std::size_t( m_end - m_data.data );
         }

         const char* begin() const noexcept
         {
            return m_data.data;
         }

         const char* end( const std::size_t ) const noexcept
         {
            return m_end;
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
            return m_data.data[ offset ];
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
         internal::memory_mark< M > mark() noexcept
         {
            return internal::memory_mark< M >( m_data );
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

      using memory_input = basic_memory_input< lf_crlf_eol >;

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
