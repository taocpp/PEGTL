// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_READ_INPUT_HPP
#define TAO_PEGTL_READ_INPUT_HPP

#include <filesystem>

#include "config.hpp"
#include "eol.hpp"
#include "string_input.hpp"
#include "tracking_mode.hpp"

#include "internal/file_reader.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
   struct read_input
      : string_input< P, Eol >
   {
      explicit read_input( const std::filesystem::path& path )
         : string_input< P, Eol >( internal::file_reader( path ).read(), path.string() )
      {}

      read_input( FILE* file, const std::filesystem::path& path )
         : string_input< P, Eol >( internal::file_reader( file, path ).read(), path.string() )
      {}

      read_input( const read_input& ) = delete;
      read_input( read_input&& ) = delete;

      ~read_input() = default;

      void operator=( const read_input& ) = delete;
      void operator=( read_input&& ) = delete;
   };

   template< typename... Ts >
   explicit read_input( Ts&&... ) -> read_input<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
