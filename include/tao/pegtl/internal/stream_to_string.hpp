// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
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
   template< typename T >
   [[nodiscard]] std::string stream_to_string( const T& t )
   {
      std::ostringstream oss;
      oss << t;
      return std::move( oss ).str();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
