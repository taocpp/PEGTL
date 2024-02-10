// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INPUTS_HPP
#define TAO_PEGTL_INPUTS_HPP

#include <filesystem>
#include <string>

#include "config.hpp"

#include "internal/inputs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = void, typename Data = char >
   using base_input = internal::base_alias< Eol, Data >;

   template< typename Eol = void, typename Source = std::string >
   using argv_input = internal::argv_alias< Eol, Source >;

   template< typename Eol = void, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   using view_input = internal::view_alias< Eol, Data, InputSource, ErrorSource >;

   template< typename Eol = void, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   using copy_input = internal::copy_alias< Eol, Container, InputSource, ErrorSource >;

   template< typename Eol = void >
   using file_input = internal::file_alias< Eol >;

   template< typename Eol = void >
      using read_input = internal::read_alias< Eol >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol = void, typename Data = char >
   using mmap_input = internal::mmap_alias< Eol, Data >;
#endif

   template< typename Eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   using text_view_input = internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >;

   template< typename Eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   using text_copy_input = internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >;

   template< typename Eol >
   using text_file_input = internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >;

   template< typename Eol >
   using text_read_input = internal::choose_alias< Eol, internal::lazy_read_alias< Eol >, internal::text_read_alias< Eol > >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data = char >
   using text_mmap_input = internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >;
#endif

}  // namespace TAO_PEGTL_NAMESPACE

#endif
