// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MMAP_FILE_BASE_HPP
#define TAO_PEGTL_INTERNAL_MMAP_FILE_BASE_HPP

#if defined( __unix__ ) || ( defined( __APPLE__ ) && defined( __MACH__ ) )
#include <unistd.h>  // Required for _POSIX_MAPPED_FILES
#endif

#if defined( _POSIX_MAPPED_FILES )
#include "mmap_file_posix.hpp"
#define TAO_PEGTL_MMAP_AVAILABLE 1
#elif defined( _WIN32 )
#include "mmap_file_win32.hpp"
#define TAO_PEGTL_MMAP_AVAILABLE 1
#else
#undef TAO_PEGTL_MMAP_AVAILABLE
#endif

#if defined( TAO_PEGTL_MMAP_AVAILABLE )

#if !defined( __cpp_exceptions )
#include <cstdio>
#include <exception>
#else
#include <stdexcept>
#endif

#include <filesystem>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   class mmap_file_base
   {
   public:
      explicit mmap_file_base( const std::filesystem::path& path )
         : m_mmap_data( path )
      {}

      mmap_file_base( mmap_file_base&& ) = delete;
      mmap_file_base( const mmap_file_base& ) = delete;

      ~mmap_file_base() = default;

      void operator=( mmap_file_base&& ) = delete;
      void operator=( const mmap_file_base& ) = delete;

      // TODO: Use std::span with C++20.

      template< typename Data >
      [[nodiscard]] std::size_t mmap_file_size() const
      {
         if( ( sizeof( Data ) == 1 ) || ( m_mmap_data.size() % sizeof( Data ) == 0 ) ) {
            return m_mmap_data.size() / sizeof( Data );
         }
#if defined( __cpp_exceptions )
         throw std::runtime_error( "mmap() file size is not multiple of data size" );
#else
         std::perror( "mmap() file size is not multiple of data size" );
         std::terminate();
#endif
      }

      template< typename Data >
      [[nodiscard]] const Data* mmap_file_data() const noexcept
      {
         return reinterpret_cast< const Data* >( m_mmap_data.data() );
      }

   protected:
      const mmap_file_impl m_mmap_data;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
