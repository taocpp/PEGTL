// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MMAP_FILE_POSIX_HPP
#define TAO_PEGTL_INTERNAL_MMAP_FILE_POSIX_HPP

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#if !defined( __cpp_exceptions )
#include <cstdio>
#include <exception>
#endif

#include <utility>

#include "filesystem.hpp"

namespace tao::pegtl::internal
{
   struct mmap_file_open
   {
      explicit mmap_file_open( const internal::filesystem::path& path )  // NOLINT(modernize-pass-by-value)
         : m_path( path ),
           m_fd( open() )
      {}

      mmap_file_open( const mmap_file_open& ) = delete;
      mmap_file_open( mmap_file_open&& ) = delete;

      ~mmap_file_open()
      {
         ::close( m_fd );
      }

      mmap_file_open& operator=( const mmap_file_open& ) = delete;
      mmap_file_open& operator=( mmap_file_open&& ) = delete;

      [[nodiscard]] std::size_t size() const
      {
         struct stat st;
         errno = 0;
         if( ::fstat( m_fd, &st ) < 0 ) {
            // LCOV_EXCL_START
#if defined( __cpp_exceptions )
            internal::error_code ec( errno, internal::system_category() );
            throw internal::filesystem::filesystem_error( "fstat() failed", m_path, ec );
#else
            std::perror( "fstat() failed" );
            std::terminate();
#endif
            // LCOV_EXCL_STOP
         }
         return std::size_t( st.st_size );
      }

      const internal::filesystem::path m_path;
      const int m_fd;

   private:
      [[nodiscard]] int open() const
      {
         errno = 0;
         const int fd = ::open( m_path.c_str(),
                                O_RDONLY
#if defined( O_CLOEXEC )
                                   | O_CLOEXEC
#endif
         );
         if( fd >= 0 ) {
            return fd;
         }
#if defined( __cpp_exceptions )
         internal::error_code ec( errno, internal::system_category() );
         throw internal::filesystem::filesystem_error( "open() failed", m_path, ec );
#else
         std::perror( "open() failed" );
         std::terminate();
#endif
      }
   };

   class mmap_file_posix
   {
   public:
      explicit mmap_file_posix( const internal::filesystem::path& path )
         : mmap_file_posix( mmap_file_open( path ) )
      {}

      explicit mmap_file_posix( const mmap_file_open& reader )
         : m_size( reader.size() ),
           m_data( static_cast< const char* >( ::mmap( nullptr, m_size, PROT_READ, MAP_PRIVATE, reader.m_fd, 0 ) ) )
      {
         if( ( m_size != 0 ) && ( intptr_t( m_data ) == -1 ) ) {
            // LCOV_EXCL_START
#if defined( __cpp_exceptions )
            internal::error_code ec( errno, internal::system_category() );
            throw internal::filesystem::filesystem_error( "mmap() failed", reader.m_path, ec );
#else
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

      using iterator = const char*;
      using const_iterator = const char*;

      [[nodiscard]] iterator data() const noexcept
      {
         return m_data;
      }

      [[nodiscard]] iterator begin() const noexcept
      {
         return m_data;
      }

      [[nodiscard]] iterator end() const noexcept
      {
         return m_data + m_size;
      }

   private:
      const std::size_t m_size;
      const char* const m_data;
   };

   using mmap_file_impl = mmap_file_posix;

}  // namespace tao::pegtl::internal

#endif
