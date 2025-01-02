// Copyright (c) 2016-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_CSTREAM_READER_HPP
#define TAO_PEGTL_BUFFER_CSTREAM_READER_HPP

#include <cassert>
#include <cstddef>
#include <cstdio>

#if defined( __cpp_exceptions )
#include <system_error>
#else
#include <exception>
#endif

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct cstream_reader
   {
      explicit cstream_reader( std::FILE* s ) noexcept
         : m_cstream( s )
      {
         assert( m_cstream != nullptr );
      }

      [[nodiscard]] std::size_t operator()( char* buffer, const std::size_t length ) const
      {
         if( const auto r = std::fread( buffer, 1, length, m_cstream ) ) {
            return r;
         }
         if( std::feof( m_cstream ) != 0 ) {
            return 0;
         }

         // Please contact us if you know how to provoke the following exception.
         // The example on cppreference.com doesn't work, at least not on macOS.

         // LCOV_EXCL_START
#if defined( __cpp_exceptions )
         const auto ec = std::ferror( m_cstream );
         assert( ec != 0 );
         throw std::system_error( ec, std::system_category(), "std::fread() failed" );
#else
         std::fputs( "system error: std::fread() failed\n", stderr );
         std::terminate();
#endif
         // LCOV_EXCL_STOP
      }

   protected:
      std::FILE* m_cstream;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
