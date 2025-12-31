// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_FILE_INPUT_HPP
#define TAO_PEGTL_FILE_INPUT_HPP

#include "config.hpp"
#include "default_eol.hpp"

#include "internal/input_aliases.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = default_eol >
   struct file_input
      : internal::file_alias< Eol >
   {
      using internal::file_alias< Eol >::file_alias;
   };

   template< typename... Args >
   file_input( Args&&... ) -> file_input< default_eol >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
