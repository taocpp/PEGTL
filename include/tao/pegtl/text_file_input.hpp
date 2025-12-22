// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_FILE_INPUT_HPP
#define TAO_PEGTL_TEXT_FILE_INPUT_HPP

#include "config.hpp"
#include "default_eol.hpp"

#include "internal/input_aliases.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = default_eol >
   struct text_file_input
      : internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >
   {
      using internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >::choose_alias;
   };

   template< typename... Args >
   text_file_input( Args&&... ) -> text_file_input< default_eol >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
