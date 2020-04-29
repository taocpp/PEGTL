// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_PRINT_UTILITY_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_PRINT_UTILITY_HPP

#include <ostream>
#include <string_view>

#include "../../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      inline constexpr const std::string_view reset_style = "\033[m";

      struct styled
      {
         const std::string_view style;
         const std::string_view string;
      };

      inline std::ostream& operator<<( std::ostream& os, const styled ss )
      {
         os << ss.style << ss.string << reset_style;
         return os;
      }

   }  // namespace internal

}  // namespace TAO_PEGTL_NAMESPACE

#endif
