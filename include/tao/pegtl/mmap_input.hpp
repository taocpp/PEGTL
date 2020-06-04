// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_MMAP_INPUT_HPP
#define TAO_PEGTL_MMAP_INPUT_HPP

#include <filesystem>

#include "config.hpp"
#include "eol.hpp"
#include "memory_input.hpp"
#include "tracking_mode.hpp"

#if defined( __unix__ ) || ( defined( __APPLE__ ) && defined( __MACH__ ) )
#include <unistd.h>  // Required for _POSIX_MAPPED_FILES
#endif

#if defined( _POSIX_MAPPED_FILES )
#include "internal/file_mapper_posix.hpp"
#elif defined( _WIN32 )
#include "internal/file_mapper_win32.hpp"
#else
#endif

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct mmap_holder
      {
         const file_mapper data;

         explicit mmap_holder( const std::filesystem::path& path )
            : data( path )
         {}

         mmap_holder( const mmap_holder& ) = delete;
         mmap_holder( mmap_holder&& ) = delete;

         ~mmap_holder() = default;

         void operator=( const mmap_holder& ) = delete;
         void operator=( mmap_holder&& ) = delete;
      };

   }  // namespace internal

   template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
   struct mmap_input
      : private internal::mmap_holder,
        public memory_input< P, Eol >
   {
      explicit mmap_input( const std::filesystem::path& path )
         : internal::mmap_holder( path ),
           memory_input< P, Eol >( data.begin(), data.end(), path.string() )
      {}

      mmap_input( const mmap_input& ) = delete;
      mmap_input( mmap_input&& ) = delete;

      ~mmap_input() = default;

      void operator=( const mmap_input& ) = delete;
      void operator=( mmap_input&& ) = delete;
   };

   template< typename... Ts >
   explicit mmap_input( Ts&&... ) -> mmap_input<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
