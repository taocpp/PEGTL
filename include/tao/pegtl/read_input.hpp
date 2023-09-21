// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_READ_INPUT_HPP
#define TAO_PEGTL_READ_INPUT_HPP

#include <filesystem>
#include <string>

#include "config.hpp"
#include "eol.hpp"
#include "string_input.hpp"
#include "tracking_mode.hpp"

#include "internal/path_to_string.hpp"
#include "internal/read_file_stdio.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
   struct read_input
      : string_input< P, Eol >
   {
      read_input( const std::filesystem::path& path, const std::string& source )
         : string_input< P, Eol >( internal::read_file_stdio( path ).read_string(), source )
      {}

      explicit read_input( const std::filesystem::path& path )
         : read_input( path, internal::path_to_string( path ) )
      {}

      read_input( FILE* file, const std::filesystem::path& path, const std::string& source )
         : string_input< P, Eol >( internal::read_file_stdio( file, path ).read_string(), source )
      {}

      read_input( FILE* file, const std::filesystem::path& path )
         : read_input( file, path, internal::path_to_string( path ) )
      {}

      read_input( const read_input& ) = delete;
      read_input( read_input&& ) = delete;

      ~read_input() = default;

      read_input& operator=( const read_input& ) = delete;
      read_input& operator=( read_input&& ) = delete;
   };

   template< typename... Ts >
   explicit read_input( Ts&&... ) -> read_input<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
