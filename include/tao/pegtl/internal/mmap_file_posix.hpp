// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MMAP_FILE_POSIX_HPP
#define TAO_PEGTL_INTERNAL_MMAP_FILE_POSIX_HPP

#include <fcntl.h>
#include <filesystem>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

#if !defined( __cpp_exceptions )
#include <cstdio>
#include <exception>
#endif

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline int file_open( const std::filesystem::path& path )
   {
      errno = 0;
      const int fh = ::open( path.c_str(),
                             O_RDONLY
#if defined( O_CLOEXEC )
                                | O_CLOEXEC
#endif
      );
      if( fh >= 0 ) {
         return fh;
      }
#if defined( __cpp_exceptions )
      const std::error_code ec( errno, std::system_category() );
      throw std::filesystem::filesystem_error( "open() failed", path, ec );
#else
      std::perror( "open() failed" );
      std::terminate();
#endif
   }

   struct mmap_file_open
   {
      explicit mmap_file_open( const std::filesystem::path& path )  // NOLINT(modernize-pass-by-value)
         : fd( file_open( path ) ),
           size( file_size( path ) )
      {}

      mmap_file_open( const mmap_file_open& ) = delete;
      mmap_file_open( mmap_file_open&& ) = delete;

      ~mmap_file_open()
      {
         ::close( fd );
      }

      mmap_file_open& operator=( const mmap_file_open& ) = delete;
      mmap_file_open& operator=( mmap_file_open&& ) = delete;

      const int fd;
      const std::size_t size;

   private:
      [[nodiscard]] std::size_t file_size( const std::filesystem::path& path ) const
      {
         struct stat st;
         errno = 0;
         if( ::fstat( fd, &st ) < 0 ) {
            // LCOV_EXCL_START
#if defined( __cpp_exceptions )
            const std::error_code ec( errno, std::system_category() );
            throw std::filesystem::filesystem_error( "fstat() failed", path, ec );
#else
            (void)path;
            std::perror( "fstat() failed" );
            std::terminate();
#endif
            // LCOV_EXCL_STOP
         }
         return static_cast< std::size_t >( st.st_size );
      }
   };

   class mmap_file_posix
   {
   public:
      explicit mmap_file_posix( const std::filesystem::path& path )
         : mmap_file_posix( path, mmap_file_open( path ) )
      {}

      mmap_file_posix( const std::filesystem::path& path, const mmap_file_open& file )
         : m_size( file.size ),
           m_data( static_cast< const char* >( ::mmap( nullptr, m_size, PROT_READ, MAP_PRIVATE, file.fd, 0 ) ) )
      {
         if( ( m_size != 0 ) && ( reinterpret_cast< intptr_t >( m_data ) == -1 ) ) {
            // LCOV_EXCL_START
#if defined( __cpp_exceptions )
            const std::error_code ec( errno, std::system_category() );
            throw std::filesystem::filesystem_error( "mmap() failed", path, ec );
#else
            (void)path;
            std::perror( "mmap() failed" );
            std::terminate();
#endif
            // LCOV_EXCL_STOP
         }
      }

      mmap_file_posix( const mmap_file_posix& ) = delete;
      mmap_file_posix( mmap_file_posix&& ) = delete;

      ~mmap_file_posix()
      {
         // Legacy C interface requires pointer-to-mutable but does not write through the pointer.
         ::munmap( const_cast< char* >( m_data ), m_size );
      }

      mmap_file_posix& operator=( const mmap_file_posix& ) = delete;
      mmap_file_posix& operator=( mmap_file_posix&& ) = delete;

      [[nodiscard]] bool empty() const noexcept
      {
         return m_size == 0;
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return m_size;
      }

      [[nodiscard]] const char* data() const noexcept
      {
         return m_data;
      }

   private:
      const std::size_t m_size;
      const char* const m_data;
   };

   using mmap_file_impl = mmap_file_posix;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
