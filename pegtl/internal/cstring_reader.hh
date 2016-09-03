// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_CSTRING_READER_HH
#define PEGTL_INTERNAL_CSTRING_READER_HH

#include <cassert>
#include <cstddef>

#include "../input_error.hh"

namespace pegtl
{
   namespace internal
   {
      struct cstring_reader
      {
         explicit
         cstring_reader( const char * zero_terminated )
               : m_cstring( zero_terminated )
         {
            assert( m_cstring );
         }

         std::size_t operator() ( char * buffer, const std::size_t length )
         {
            std::size_t i = 0;
            char c;

            while ( ( i < length ) && ( ( c = m_cstring[ i ] ) != 0 ) ) {
               *buffer++ = c;
               ++i;
            }
            m_cstring += i;
            return i;
         }

         const char * m_cstring;
      };

   } // namespace internal

} // namespace pegtl

#endif
