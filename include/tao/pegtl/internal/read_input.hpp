// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_READ_INPUT_HPP
#define TAO_PEGTL_INTERNAL_READ_INPUT_HPP

#include <cstdio>
#include <filesystem>
#include <string>

#include "../config.hpp"

#include "copy_input.hpp"
#include "read_file_stdio.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   class read_input
      : public copy_input< std::string >
   {
   public:
      explicit read_input( const std::filesystem::path& path )
         : copy_input< std::string >( read_file_stdio( path ).read_string() )
      {}

      read_input( FILE* file, const std::filesystem::path& path )
         : copy_input< std::string >( read_file_stdio( file, path ).read_string() )
      {}
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
