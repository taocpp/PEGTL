// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_MEMORY_ACTION_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_MEMORY_ACTION_INPUT_HPP

#include <cstddef>
#include <string>

#include "iterator.hpp"

#include "../config.hpp"
#include "../position.hpp"
#include "../position_tracking.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol, position_tracking >
      class basic_memory_input;

      namespace internal
      {
         template< typename Eol, position_tracking >
         class basic_memory_action_input;

         template< typename Eol >
         class basic_memory_action_input< Eol, position_tracking::IMMEDIATE >
         {
         public:
            using eol_t = Eol;
            static constexpr position_tracking position_tracking_v = position_tracking::IMMEDIATE;

            using memory_t = basic_memory_input< Eol, position_tracking::IMMEDIATE >;
            using action_t = basic_memory_action_input;

            basic_memory_action_input( const internal::iterator& in_data, const internal::iterator& in_end, const char* in_source )
               : m_data( in_data ),
                 m_end( in_end.data ),
                 m_source( in_source )
            {
            }

            const char* begin() const
            {
               return m_data.data;
            }

            const char* end( const std::size_t = 0 ) const
            {
               return m_end;
            }

            bool empty() const
            {
               return begin() == m_end;
            }

            std::size_t size( const std::size_t = 0 ) const
            {
               return std::size_t( m_end - begin() );
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

            const char* source() const
            {
               return m_source;
            }

            std::string string() const
            {
               return std::string( begin(), end() );
            }

            char peek_char( const std::size_t offset = 0 ) const
            {
               return begin()[ offset ];
            }

            unsigned char peek_byte( const std::size_t offset = 0 ) const
            {
               return static_cast< unsigned char >( peek_char( offset ) );
            }

            TAOCPP_PEGTL_NAMESPACE::position position() const
            {
               return TAOCPP_PEGTL_NAMESPACE::position( m_data, m_source );
            }

         private:
            internal::iterator m_data;
            const char* m_end;
            const char* m_source;
         };

         template< typename Eol >
         class basic_memory_action_input< Eol, position_tracking::LAZY >
         {
         public:
            using eol_t = Eol;
            static constexpr position_tracking position_tracking_v = position_tracking::LAZY;

            using memory_t = basic_memory_input< Eol, position_tracking::LAZY >;
            using action_t = basic_memory_action_input;

            basic_memory_action_input( const char* in_begin, const char* in_end, const char* )
               : m_begin( in_begin ),
                 m_end( in_end )
            {
            }

            const char* begin() const
            {
               return m_begin;
            }

            const char* end( const std::size_t = 0 ) const
            {
               return m_end;
            }

            bool empty() const
            {
               return begin() == m_end;
            }

            std::size_t size( const std::size_t = 0 ) const
            {
               return std::size_t( m_end - begin() );
            }

            std::string string() const
            {
               return std::string( begin(), end() );
            }

            char peek_char( const std::size_t offset = 0 ) const
            {
               return begin()[ offset ];
            }

            unsigned char peek_byte( const std::size_t offset = 0 ) const
            {
               return static_cast< unsigned char >( peek_char( offset ) );
            }

         private:
            const char* m_begin;
            const char* m_end;
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
