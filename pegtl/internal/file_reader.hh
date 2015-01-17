// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_FILE_READER_HH
#define PEGTL_INTERNAL_FILE_READER_HH

#include <cstdio>

#include "../input_error.hh"

namespace pegtl
{
   namespace internal
   {
      struct file_reader
      {
         explicit
         file_reader( const std::string & filename )
               : m_file( std::fopen( filename.c_str(), "r" ) ),
                 m_source( filename )
         {
            if ( ! m_file ) {
               PEGTL_THROW_INPUT_ERROR( "unable to open() file " << m_source << " for reading" );
            }
         }

         ~file_reader()
         {
            std::fclose( m_file );
         }

         file_reader( const file_reader & ) = delete;
         void operator= ( const file_reader & ) = delete;

         size_t size() const
         {
            errno = 0;
            if ( std::fseek( m_file, 0, SEEK_END ) ) {
               PEGTL_THROW_INPUT_ERROR( "unable to fseek() to end of file " << m_source );
            }
            const auto s = std::ftell( m_file );
            if ( s < 0 ) {
               PEGTL_THROW_INPUT_ERROR( "unable to ftell() file size of file " << m_source );
            }
            if ( std::fseek( m_file, 0, SEEK_SET ) ) {
               PEGTL_THROW_INPUT_ERROR( "unable to fseek() to beginning of file " << m_source );
            }
            return s;
         }

         std::string read() const
         {
            std::string nrv;
            nrv.resize( size() );
            errno = 0;

            if ( nrv.size() && ( std::fread( & nrv[ 0 ], 1, nrv.size(), m_file ) != nrv.size() ) ) {
               PEGTL_THROW_INPUT_ERROR( "unable to fread() file " << m_source << " size " << nrv.size() );
            }
            return nrv;
         }

      private:
         ::FILE * const m_file;
         const std::string m_source;
      };

   } // internal

} // pegtl

#endif
