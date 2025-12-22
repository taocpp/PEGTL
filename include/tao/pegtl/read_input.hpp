// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_READ_INPUT_HPP
#define TAO_PEGTL_READ_INPUT_HPP

#include "config.hpp"
#include "default_eol.hpp"

#include "internal/input_aliases.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = default_eol >
   struct read_input
      : internal::read_alias< Eol >
   {
      using internal::read_alias< Eol >::read_alias;
   };

   template< typename... Args >
   read_input( Args&&... ) -> read_input< default_eol >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
