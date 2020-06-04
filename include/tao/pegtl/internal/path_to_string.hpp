// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_PATH_TO_STRING_HPP
#define TAO_PEGTL_INTERNAL_PATH_TO_STRING_HPP

#include <filesystem>
#include <sstream>
#include <string>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline std::string path_to_string( const std::filesystem::path& path )
   {
      std::ostringstream os;
      os << path;
      return os.str();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
