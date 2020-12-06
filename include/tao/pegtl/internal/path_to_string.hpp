// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_PATH_TO_STRING_HPP
#define TAO_PEGTL_INTERNAL_PATH_TO_STRING_HPP

#include <string>

#include "../config.hpp"
#include "filesystem.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline std::string path_to_string( const internal::filesystem::path& path )
   {
      return path.string();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
