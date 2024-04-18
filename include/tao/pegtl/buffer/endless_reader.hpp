// Copyright (c) 2016-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_ENDLESS_READER_HPP
#define TAO_PEGTL_BUFFER_ENDLESS_READER_HPP

#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct endless_reader
   {
      explicit endless_reader( const char data )
         : m_endless( std::string::size_type( 1 ), data )
      {}

      explicit endless_reader( std::string&& data )
         : m_endless( std::move( data ) )
      {
         assert( !m_endless.empty() );
      }

      explicit endless_reader( const std::string& data )
         : m_endless( data )
      {
         assert( !m_endless.empty() );
      }

      [[nodiscard]] std::size_t operator()( char* buffer, const std::size_t length ) noexcept
      {
         if( m_endless.size() == 1 ) {
            std::memset( buffer, m_endless[ 0 ], length );
            return length;
         }
         for( std::size_t i = 0; i < length; ++i ) {
            buffer[ i ] = m_endless[ ( m_offset + i ) % m_endless.size() ];
         }
         m_offset += length;
         m_offset %= m_endless.size();
         return length;
      }

   protected:
      std::size_t m_offset = 0;
      std::string m_endless;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
