// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_ISTREAM_READER_HH
#define PEGTL_INTERNAL_ISTREAM_READER_HH

#include <istream>

#include "../input_error.hh"

namespace pegtl
{
   namespace internal
   {
      struct istream_reader
      {
         explicit
         istream_reader( std::istream & s )
               : m_istream( s )
         { }

         size_t operator() ( char * buffer, const std::size_t length )
         {
            m_istream.read( buffer, length );

            if ( const auto r = m_istream.gcount() ) {
               return r;
            }
            if ( m_istream.fail() && ( ! m_istream.eof() ) ) {
               PEGTL_THROW_INPUT_ERROR( "error in istream.read()" );
            }
            return 0;
         }

         std::istream & m_istream;
      };

   } // internal

} // pegtl

#endif
