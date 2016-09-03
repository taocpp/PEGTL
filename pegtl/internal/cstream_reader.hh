// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_CSTREAM_READER_HH
#define PEGTL_INTERNAL_CSTREAM_READER_HH

#include <cstdio>
#include <cstddef>

#include "../input_error.hh"

namespace pegtl
{
   namespace internal
   {
      struct cstream_reader
      {
         explicit
         cstream_reader( std::FILE * s )
               : m_cstream( s )
         { }

         std::size_t operator() ( char * buffer, const std::size_t length )
         {
            if ( const auto r = std::fread( buffer, 1, length, m_cstream ) ) {
               return r;
            }
            if ( std::feof( m_cstream ) != 0 ) {
               return 0;
            }
            // Please contact us if you know how to provoke the following exception.
            // The example on cppreference.com doesn't work, at least not on Mac OS X.
            PEGTL_THROW_INPUT_ERROR( "error in fread() from cstream" );  // LCOV_EXCL_LINE
         }

         std::FILE * m_cstream;
      };

   } // namespace internal

} // namespace pegtl

#endif
