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
         cstream_reader( ::FILE * s )
               : m_cstream( s )
         { }

         size_t operator() ( char * buffer, const std::size_t length )
         {
            if ( const auto r = std::fread( buffer, 1, length, m_cstream ) ) {
               return r;
            }
            if ( std::ferror( m_cstream ) ) {
               PEGTL_THROW_INPUT_ERROR( "error in fread() from cstream" );
            }
            return 0;
         }

         ::FILE * m_cstream;
      };

   } // internal

} // pegtl

#endif
