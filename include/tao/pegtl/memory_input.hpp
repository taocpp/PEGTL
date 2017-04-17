// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_MEMORY_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_MEMORY_INPUT_HPP

#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

#include "config.hpp"
#include "eol.hpp"
#include "position.hpp"
#include "tracking_mode.hpp"

#include "internal/action_input.hpp"
#include "internal/bump_impl.hpp"
#include "internal/iterator.hpp"
#include "internal/marker.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Eol, tracking_mode >
         class memory_input_base;

         template< typename Eol >
         class memory_input_base< Eol, tracking_mode::IMMEDIATE >
         {
         public:
            using iterator_t = internal::iterator;

            memory_input_base( const internal::iterator& in_iter, const char* in_end, const char* in_source ) noexcept
               : m_data( in_iter ),
                 m_end( in_end ),
                 m_source( in_source )
            {
            }

            memory_input_base( const internal::iterator& in_iter, const char* in_end, const std::string& in_source ) noexcept
               : memory_input_base( in_iter, in_end, in_source.c_str() )
            {
            }

            memory_input_base( const char* in_begin, const char* in_end, const char* in_source ) noexcept
               : memory_input_base( internal::iterator( in_begin ), in_end, in_source )
            {
            }

            memory_input_base( const char* in_begin, const char* in_end, const std::string& in_source ) noexcept
               : memory_input_base( in_begin, in_end, in_source.c_str() )
            {
            }

            memory_input_base( const char* in_begin, const char* in_end, const char* in_source, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line ) noexcept
               : memory_input_base( { in_byte, in_line, in_byte_in_line, in_begin }, in_end, in_source )
            {
            }

            memory_input_base( const char* in_begin, const char* in_end, const std::string& in_source, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line ) noexcept
               : memory_input_base( in_begin, in_end, in_source.c_str(), in_byte, in_line, in_byte_in_line )
            {
            }

            const char* begin() const noexcept
            {
               return m_data.data;
            }

            const char* end( const std::size_t = 0 ) const noexcept
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

            TAOCPP_PEGTL_NAMESPACE::position position() const noexcept
            {
               return TAOCPP_PEGTL_NAMESPACE::position( m_data, m_source );
            }

            internal::iterator& iterator() noexcept
            {
               return m_data;
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
         class memory_input_base< Eol, tracking_mode::LAZY >
         {
         public:
            using iterator_t = const char*;

            memory_input_base( const char* in_begin, const char* in_end, const char* in_source ) noexcept
               : m_all( in_begin ),
                 m_run( in_begin ),
                 m_end( in_end ),
                 m_source( in_source )
            {
            }

            memory_input_base( const char* in_begin, const char* in_end, const std::string& in_source ) noexcept
               : memory_input_base( in_begin, in_end, in_source.c_str() )
            {
            }

            const char* begin() const noexcept
            {
               return m_run;
            }

            const char* end( const std::size_t = 0 ) const noexcept
            {
               return m_end;
            }

            std::size_t byte() const noexcept
            {
               return std::size_t( begin() - m_all );
            }

            const char* source() const noexcept
            {
               return m_source;
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

            TAOCPP_PEGTL_NAMESPACE::position position() const noexcept
            {
               internal::iterator c( m_all );
               internal::bump( c, byte(), Eol::ch );
               return TAOCPP_PEGTL_NAMESPACE::position( c, m_source );
            }

            const char*& iterator() noexcept
            {
               return m_run;
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

      }  // namespace internal

      template< typename Eol = lf_crlf_eol, tracking_mode P = tracking_mode::IMMEDIATE >
      class memory_input
         : public internal::memory_input_base< Eol, P >
      {
      public:
         using eol_t = Eol;
         static constexpr tracking_mode tracking_mode_v = P;

         using typename internal::memory_input_base< Eol, P >::iterator_t;

         using memory_t = memory_input;
         using action_t = internal::action_input< Eol, P >;

         using internal::memory_input_base< Eol, P >::memory_input_base;

         memory_input( const char* in_begin, const std::size_t in_size, const char* in_source ) noexcept
            : memory_input( in_begin, in_begin + in_size, in_source )
         {
         }

         memory_input( const char* in_begin, const std::size_t in_size, const std::string& in_source ) noexcept
            : memory_input( in_begin, in_size, in_source.c_str() )
         {
         }

         memory_input( const std::string& in_string, const char* in_source ) noexcept
            : memory_input( in_string.data(), in_string.size(), in_source )
         {
         }

         memory_input( const std::string& in_string, const std::string& in_source ) noexcept
            : memory_input( in_string, in_source.c_str() )
         {
         }

         memory_input( const char* in_begin, const char* in_source ) noexcept
            : memory_input( in_begin, std::strlen( in_begin ), in_source )
         {
         }

         memory_input( const char* in_begin, const std::string& in_source ) noexcept
            : memory_input( in_begin, in_source.c_str() )
         {
         }

         bool empty() const noexcept
         {
            return this->begin() == this->end();
         }

         std::size_t size( const std::size_t = 0 ) const noexcept
         {
            return std::size_t( this->end() - this->begin() );
         }

         char peek_char( const std::size_t offset = 0 ) const noexcept
         {
            return this->begin()[ offset ];
         }

         unsigned char peek_byte( const std::size_t offset = 0 ) const noexcept
         {
            return static_cast< unsigned char >( peek_char( offset ) );
         }

         void discard() const noexcept
         {
         }

         void require( const std::size_t ) const noexcept
         {
         }

         template< rewind_mode M >
         internal::marker< iterator_t, M > mark() noexcept
         {
            return internal::marker< iterator_t, M >( this->iterator() );
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
