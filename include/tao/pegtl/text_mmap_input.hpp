// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_MMAP_INPUT_HPP
#define TAO_PEGTL_TEXT_MMAP_INPUT_HPP

#include "config.hpp"
#include "default_eol.hpp"
#include "system.hpp"

#if defined( TAO_PEGTL_MMAP_AVAILABLE )

#include "internal/input_aliases.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = default_eol, typename Data = char >
   struct text_mmap_input
      : internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >
   {
      using internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >::choose_alias;
   };

   template< typename... Args >
   text_mmap_input( Args&&... ) -> text_mmap_input< default_eol >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
#endif
