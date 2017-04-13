// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_FUTURE_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_FUTURE_INPUT_HPP

#include <cstddef>

#include "config.hpp"
#include "position.hpp"
#include "rewind_mode.hpp"

#include "internal/future_action_input.hpp"
#include "internal/marker.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol >
      class basic_future_input
      {
      public:
         using eol_t = Eol;
         using memory_t = basic_future_input< Eol >;
         using action_t = internal::future_action_input< Eol >;

         basic_future_input( const char* in_begin, const char* in_end, const char* in_source ) noexcept
            : m_all( in_begin ),
              m_run( in_begin ),
              m_end( in_end ),
              m_source( in_source )
         {
         }

         bool empty() const noexcept
         {
            return m_end == m_run;
         }

         std::size_t size( const std::size_t ) const noexcept
         {
            return std::size_t( m_end - m_run );
         }

         const char* begin() const noexcept
         {
            return m_run;
         }

         const char* end( const std::size_t ) const noexcept
         {
            return m_end;
         }

         std::size_t byte() const noexcept
         {
            return std::size_t( m_run - m_all );
         }

         const char* source() const noexcept
         {
            return m_source;
         }

         char peek_char( const std::size_t offset = 0 ) const noexcept
         {
            return m_run[ offset ];
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

      using future_input = basic_future_input< lf_crlf_eol >;

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
