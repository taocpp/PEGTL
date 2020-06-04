// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_PATH_TO_STRING_HPP
#define TAO_PEGTL_INTERNAL_PATH_TO_STRING_HPP

#include <filesystem>
#include <string>
#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline std::string path_to_string( const std::filesystem::path& path )
   {
      const auto s = path.u8string();
      if constexpr( std::is_same_v< decltype( s ), const std::string > ) {
         return s;
      }
      else {
         return { reinterpret_cast< const char* >( s.data() ), s.size() };
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
