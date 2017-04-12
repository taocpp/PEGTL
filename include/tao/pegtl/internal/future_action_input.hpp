// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_FUTURE_ACTION_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_FUTURE_ACTION_INPUT_HPP

#include <cstddef>
#include <string>

#include "../config.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol >
      class basic_future_input;

      namespace internal
      {
         template< typename Eol >
         class future_action_input
         {
         public:
            using eol_t = Eol;
            using memory_t = basic_future_input< Eol >;
            using action_t = future_action_input< Eol >;

            future_action_input( const char* in_begin, const char* in_end, const char* )
               : m_begin( in_begin ),
                 m_end( in_end )
            {
            }

            bool empty() const
            {
               return m_begin == m_end;
            }

            std::size_t size( const std::size_t = 0 ) const
            {
               return std::size_t( m_end - m_begin );
            }

            const char* begin() const
            {
               return m_begin;
            }

            const char* end( const std::size_t = 0 ) const
            {
               return m_end;
            }

            std::string string() const
            {
               return std::string( begin(), end() );
            }

            char peek_char( const std::size_t offset = 0 ) const
            {
               return m_begin[ offset ];
            }

            unsigned char peek_byte( const std::size_t offset = 0 ) const
            {
               return static_cast< unsigned char >( peek_char( offset ) );
            }

         private:
            const char* m_begin;
            const char* m_end;
         };

      } // internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
