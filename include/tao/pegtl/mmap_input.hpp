// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_MMAP_INPUT_HPP
#define TAO_PEGTL_MMAP_INPUT_HPP

#include <string>

#include "eol.hpp"
#include "memory_input.hpp"
#include "tracking_mode.hpp"

#include "internal/filesystem.hpp"
#include "internal/mmap_file.hpp"
#include "internal/path_to_string.hpp"

namespace tao::pegtl
{
   template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
   struct mmap_input
      : private internal::mmap_file,
        public memory_input< P, Eol >
   {
      mmap_input( const internal::filesystem::path& path, const std::string& source )
         : internal::mmap_file( path ),
           memory_input< P, Eol >( data.begin(), data.end(), source )
      {}

      explicit mmap_input( const internal::filesystem::path& path )
         : mmap_input( path, internal::path_to_string( path ) )
      {}

      mmap_input( const mmap_input& ) = delete;
      mmap_input( mmap_input&& ) = delete;

      ~mmap_input() = default;

      mmap_input& operator=( const mmap_input& ) = delete;
      mmap_input& operator=( mmap_input&& ) = delete;
   };

   template< typename... Ts >
   explicit mmap_input( Ts&&... ) -> mmap_input<>;

}  // namespace tao::pegtl

#endif
