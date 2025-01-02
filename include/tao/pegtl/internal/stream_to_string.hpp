// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_STREAM_TO_STRING_HPP
#define TAO_PEGTL_INTERNAL_STREAM_TO_STRING_HPP

#include <sstream>
#include <string>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Ts >
   [[nodiscard]] std::string stream_to_string( const Ts&... ts )
   {
      std::ostringstream oss;
      (void)( oss << ... << ts );
      return std::move( oss ).str();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
