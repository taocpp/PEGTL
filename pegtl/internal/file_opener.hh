// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_FILE_OPENER_HH
#define PEGTL_INTERNAL_FILE_OPENER_HH

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../input_error.hh"

namespace pegtl
{
   namespace internal
   {
      struct file_opener
      {
         explicit
         file_opener( const std::string & filename )
               : m_fd( ::open( filename.c_str(), O_RDONLY ) ),
                 m_source( filename )
         {
            if ( m_fd < 0 ) {
               PEGTL_THROW_INPUT_ERROR( "unable to open() file " << m_source << " for reading" );
            }
         }

         ~file_opener()
         {
            ::close( m_fd );
         }

         file_opener( const file_opener & ) = delete;
         void operator= ( const file_opener & ) = delete;

         size_t size() const
         {
            struct stat st;

            errno = 0;

            if ( ::fstat( m_fd, & st ) < 0 ) {
               PEGTL_THROW_INPUT_ERROR( "unable to fstat() file " << m_source << " descriptor " << m_fd );
            }
            return size_t( st.st_size );
         }

         const int m_fd;
         const std::string m_source;
      };

   } // internal

} // pegtl

#endif
