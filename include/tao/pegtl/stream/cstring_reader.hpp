// Copyright (c) 2016-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_STREAM_CSTRING_READER_HPP
#define TAO_PEGTL_STREAM_CSTRING_READER_HPP

#include <cassert>
#include <cstddef>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   class cstring_reader
   {
   public:
      explicit cstring_reader( const char* zero_terminated ) noexcept
         : m_cstring( zero_terminated )
      {
         assert( m_cstring != nullptr );
      }

      [[nodiscard]] std::size_t read( char* buffer, const std::size_t length ) noexcept
      {
         for( std::size_t i = 0; i < length; ++i ) {
            const char c = m_cstring[ i ];
            if( c == '\0' ) {
               m_cstring += i;
               return i;
            }
            buffer[ i ] = c;
         }
         m_cstring += length;
         return length;
      }

   protected:
      const char* m_cstring;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
