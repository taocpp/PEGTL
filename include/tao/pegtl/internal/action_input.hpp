// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_MEMORY_ACTION_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_MEMORY_ACTION_INPUT_HPP

#include <cstddef>
#include <string>

#include "iterator.hpp"

#include "../config.hpp"
#include "../eol.hpp"
#include "../position.hpp"
#include "../tracking_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      // template< typename Eol = lf_crlf_eol, tracking_mode P = tracking_mode::IMMEDIATE >
      template< typename, tracking_mode >
      class memory_input;

      namespace internal
      {
         template< typename Eol, tracking_mode >
         class action_input_base;

         template< typename Eol >
         class action_input_base< Eol, tracking_mode::IMMEDIATE >
         {
         public:
            action_input_base( const iterator& in_data, const iterator& in_end, const char* in_source ) noexcept
               : m_data( in_data ),
                 m_end( in_end.data ),
                 m_source( in_source )
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

            TAOCPP_PEGTL_NAMESPACE::position position() const
            {
               return TAOCPP_PEGTL_NAMESPACE::position( m_data, m_source );
            }

         private:
            const iterator m_data;
            const char* const m_end;
            const char* const m_source;
         };

         template< typename Eol >
         class action_input_base< Eol, tracking_mode::LAZY >
         {
         public:
            action_input_base( const char* in_begin, const char* in_end, const char* ) noexcept
               : m_begin( in_begin ),
                 m_end( in_end )
            {
            }

            const char* begin() const noexcept
            {
               return m_begin;
            }

            const char* end( const std::size_t = 0 ) const noexcept
            {
               return m_end;
            }

         private:
            const char* const m_begin;
            const char* const m_end;
         };

         template< typename Eol, tracking_mode P >
         class action_input
            : public action_input_base< Eol, P >
         {
         public:
            using eol_t = Eol;
            static constexpr tracking_mode tracking_mode_v = P;

            using memory_t = memory_input< Eol, P >;
            using action_t = action_input;

            using action_input_base< Eol, P >::action_input_base;

            bool empty() const noexcept
            {
               return this->begin() == this->end();
            }

            std::size_t size( const std::size_t s = 0 ) const noexcept
            {
               return std::size_t( this->end( s ) - this->begin() );
            }

            std::string string() const
            {
               return std::string( this->begin(), this->end() );
            }

            char peek_char( const std::size_t offset = 0 ) const noexcept
            {
               return this->begin()[ offset ];
            }

            unsigned char peek_byte( const std::size_t offset = 0 ) const noexcept
            {
               return static_cast< unsigned char >( peek_char( offset ) );
            }
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
