// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ARGV_INPUT_HPP
#define TAO_PEGTL_ARGV_INPUT_HPP

#include <string>

#include "config.hpp"

#include "internal/input_aliases.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = void, typename Source = std::string >
   struct argv_input
      : internal::argv_alias< Eol, Source >
   {
      using internal::argv_alias< Eol, Source >::argv_alias;
   };

   argv_input( char**, const int ) -> argv_input<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
