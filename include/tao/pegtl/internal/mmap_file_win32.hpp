// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MMAP_FILE_WIN32_HPP
#define TAO_PEGTL_INTERNAL_MMAP_FILE_WIN32_HPP

#include "../config.hpp"

#if !defined( NOMINMAX )
#define NOMINMAX
#define TAO_PEGTL_NOMINMAX_WAS_DEFINED
#endif

#if !defined( WIN32_LEAN_AND_MEAN )
#define WIN32_LEAN_AND_MEAN
#define TAO_PEGTL_WIN32_LEAN_AND_MEAN_WAS_DEFINED
#endif

#include <windows.h>

#if defined( TAO_PEGTL_NOMINMAX_WAS_DEFINED )
#undef NOMINMAX
#undef TAO_PEGTL_NOMINMAX_WAS_DEFINED
#endif

#if defined( TAO_PEGTL_WIN32_LEAN_AND_MEAN_WAS_DEFINED )
#undef WIN32_LEAN_AND_MEAN
#undef TAO_PEGTL_WIN32_LEAN_AND_MEAN_WAS_DEFINED
#endif

#if !defined( __cpp_exceptions )
#include <cstdio>
#include <exception>
#endif

#include <filesystem>

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline HANDLE file_open( const std::filesystem::path& path )
   {
      SetLastError( 0 );
#if( _WIN32_WINNT >= 0x0602 )
      const HANDLE h = ::CreateFile2( path.c_str(),
                                      GENERIC_READ,
                                      FILE_SHARE_READ,
                                      OPEN_EXISTING,
                                      nullptr );
      if( h != INVALID_HANDLE_VALUE ) {
         return h;
      }
#if defined( __cpp_exceptions )
      std::error_code ec( ::GetLastError(), std::system_category() );
      throw std::filesystem::filesystem_error( "CreateFile2() failed", path, ec );
#else
      std::perror( "CreateFile2() failed" );
      std::terminate();
#endif
#else
      const HANDLE h = ::CreateFileW( path.c_str(),
                                      GENERIC_READ,
                                      FILE_SHARE_READ,
                                      nullptr,
                                      OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL,
                                      nullptr );
      if( h != INVALID_HANDLE_VALUE ) {
         return h;
      }
#if defined( __cpp_exceptions )
      std::error_code ec( ::GetLastError(), std::system_category() );
      throw std::filesystem::filesystem_error( "CreateFileW()", path, ec );
#else
      std::perror( "CreateFileW() failed" );
      std::terminate();
#endif
#endif
   }

   struct mmap_file_open
   {
      explicit mmap_file_open( const std::filesystem::path& path )
         : handle( file_open( path ) ),
           size( file_size( path ) )
      {}

      mmap_file_open( const mmap_file_open& ) = delete;
      mmap_file_open( mmap_file_open&& ) = delete;

      ~mmap_file_open()
      {
         ::CloseHandle( handle );
      }

      mmap_file_open& operator=( const mmap_file_open& ) = delete;
      mmap_file_open& operator=( mmap_file_open&& ) = delete;

      const HANDLE handle;
      const std::size_t size;

   private:
      [[nodiscard]] std::size_t file_size( const std::filesystem::path& path ) const
      {
         LARGE_INTEGER s;
         if( !::GetFileSizeEx( handle, &s ) ) {
#if defined( __cpp_exceptions )
            std::error_code ec( ::GetLastError(), std::system_category() );
            throw std::filesystem::filesystem_error( "GetFileSizeEx() failed", path, ec );
#else
            (void)path;
            std::perror( "GetFileSizeEx() failed" );
            std::terminate();
#endif
         }
         return std::size_t( s.QuadPart );
      }
   };

   struct mmap_file_mmap
   {
      explicit mmap_file_mmap( const std::filesystem::path& path )
         : mmap_file_mmap( path, mmap_file_open( path ) )
      {}

      mmap_file_mmap( const std::filesystem::path& path, const mmap_file_open& file )
         : size( file.size ),
           handle( open( path, file ) )
      {}

      mmap_file_mmap( const mmap_file_mmap& ) = delete;
      mmap_file_mmap( mmap_file_mmap&& ) = delete;

      ~mmap_file_mmap()
      {
         ::CloseHandle( handle );
      }

      mmap_file_mmap& operator=( const mmap_file_mmap& ) = delete;
      mmap_file_mmap& operator=( mmap_file_mmap&& ) = delete;

      const size_t size;
      const HANDLE handle;

   private:
      [[nodiscard]] HANDLE open( const std::filesystem::path& path, const mmap_file_open& file ) const
      {
         SetLastError( 0 );
         // Use `CreateFileMappingW` because a) we're not specifying a
         // mapping name, so the character type is of no consequence, and
         // b) it's defined in `memoryapi.h`, unlike
         // `CreateFileMappingA`(?!)
         const HANDLE h = ::CreateFileMappingW( file.handle,
                                                nullptr,
                                                PAGE_READONLY,
                                                DWORD( file.size >> 32 ),
                                                DWORD( file.size & 0xffffffff ),
                                                nullptr );
         if( ( h != NULL ) || ( file.size == 0 ) ) {
            return h;
         }
#if defined( __cpp_exceptions )
         std::error_code ec( ::GetLastError(), std::system_category() );
         throw std::filesystem::filesystem_error( "CreateFileMappingW() failed", path, ec );
#else
         (void)path;
         std::perror( "CreateFileMappingW() failed" );
         std::terminate();
#endif
      }
   };

   class mmap_file_win32
   {
   public:
      explicit mmap_file_win32( const std::filesystem::path& path )
         : mmap_file_win32( mmap_file_mmap( path ) )
      {}

      explicit mmap_file_win32( const mmap_file_mmap& file )
         : m_size( file.size ),
           m_data( static_cast< const char* >( ::MapViewOfFile( file.handle,
                                                                FILE_MAP_READ,
                                                                0,
                                                                0,
                                                                0 ) ) )
      {
         if( ( m_size != 0 ) && ( intptr_t( m_data ) == 0 ) ) {
#if defined( __cpp_exceptions )
            std::error_code ec( ::GetLastError(), std::system_category() );
            throw std::filesystem::filesystem_error( "MapViewOfFile() failed", ec );
#else
            std::perror( "MapViewOfFile() failed" );
            std::terminate();
#endif
         }
      }

      mmap_file_win32( const mmap_file_win32& ) = delete;
      mmap_file_win32( mmap_file_win32&& ) = delete;

      ~mmap_file_win32()
      {
         ::UnmapViewOfFile( LPCVOID( m_data ) );
      }

      mmap_file_win32& operator=( const mmap_file_win32& ) = delete;
      mmap_file_win32& operator=( mmap_file_win32&& ) = delete;

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

   using mmap_file_impl = mmap_file_win32;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
