// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_FILE_READER_HPP
#define TAO_PEGTL_INTERNAL_FILE_READER_HPP

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline std::FILE* file_open( const std::filesystem::path& path )
   {
      errno = 0;
#if defined( _MSC_VER )
      std::FILE* file;
      if( ::fopen_s( &file, path.u8string().c_str(), "rb" ) == 0 ) {
         return file;
      }
      const std::error_code ec( errno, std::system_category() );
      throw std::filesystem::filesystem_error( "fopen_s() failed", path, ec );
#else
#if defined( __MINGW32__ )
      if( auto* file = std::fopen( path.u8string().c_str(), "rb" ) )
#else
      if( auto* file = std::fopen( path.u8string().c_str(), "rbe" ) )
#endif
      {
         return file;
      }
      const std::error_code ec( errno, std::system_category() );
      throw std::filesystem::filesystem_error( "std::fopen() failed", path, ec );
#endif
   }

   struct file_close
   {
      void operator()( FILE* f ) const noexcept
      {
         std::fclose( f );
      }
   };

   class file_reader
   {
   public:
      explicit file_reader( const std::filesystem::path& path )
         : m_path( path ),
           m_file( file_open( path ) )
      {}

      file_reader( FILE* file, const std::filesystem::path& path )
         : m_path( path ),
           m_file( file )
      {}

      file_reader( const file_reader& ) = delete;
      file_reader( file_reader&& ) = delete;

      ~file_reader() = default;

      void operator=( const file_reader& ) = delete;
      void operator=( file_reader&& ) = delete;

      [[nodiscard]] std::size_t size() const
      {
         errno = 0;
         if( std::fseek( m_file.get(), 0, SEEK_END ) != 0 ) {
            // LCOV_EXCL_START
            const std::error_code ec( errno, std::system_category() );
            throw std::filesystem::filesystem_error( "std::fseek() failed [SEEK_END]", m_path, ec );
            // LCOV_EXCL_STOP
         }
         errno = 0;
         const auto s = std::ftell( m_file.get() );
         if( s < 0 ) {
            // LCOV_EXCL_START
            const std::error_code ec( errno, std::system_category() );
            throw std::filesystem::filesystem_error( "std::ftell() failed", m_path, ec );
            // LCOV_EXCL_STOP
         }
         errno = 0;
         if( std::fseek( m_file.get(), 0, SEEK_SET ) != 0 ) {
            // LCOV_EXCL_START
            const std::error_code ec( errno, std::system_category() );
            throw std::filesystem::filesystem_error( "std::fseek() failed [SEEK_SET]", m_path, ec );
            // LCOV_EXCL_STOP
         }
         return std::size_t( s );
      }

      [[nodiscard]] std::string read() const
      {
         std::string nrv;
         nrv.resize( size() );
         errno = 0;
         if( !nrv.empty() && ( std::fread( &nrv[ 0 ], nrv.size(), 1, m_file.get() ) != 1 ) ) {
            // LCOV_EXCL_START
            const std::error_code ec( errno, std::system_category() );
            throw std::filesystem::filesystem_error( "std::fread() failed", m_path, ec );
            // LCOV_EXCL_STOP
         }
         return nrv;
      }

   private:
      const std::filesystem::path m_path;
      const std::unique_ptr< std::FILE, file_close > m_file;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
