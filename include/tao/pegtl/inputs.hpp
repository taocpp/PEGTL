// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INPUTS_HPP
#define TAO_PEGTL_INPUTS_HPP

#include <string>

#include "ascii.hpp"
#include "config.hpp"
#include "system.hpp"

#include "internal/inputs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = void, typename Source = std::string >
   struct argv_input
      : internal::argv_alias< Eol, Source >
   {
      using internal::argv_alias< Eol, Source >::argv_alias;
   };

   template< typename Eol = void, typename Data = char >
   struct base_input
      : internal::base_alias< Eol, Data >
   {
      using internal::base_alias< Eol, Data >::base_alias;
   };

   template< typename Eol = void, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   struct view_input
      : internal::view_alias< Eol, Data, InputSource, ErrorSource >
   {
      using internal::view_alias< Eol, Data, InputSource, ErrorSource >::view_alias;
   };

   template< typename Eol = void, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   struct copy_input
      : internal::copy_alias< Eol, Container, InputSource, ErrorSource >
   {
      using internal::copy_alias< Eol, Container, InputSource, ErrorSource >::copy_alias;
   };

   template< typename Eol = void >
   struct file_input
      : internal::file_alias< Eol >
   {
      using internal::file_alias< Eol >::file_alias;
   };

   template< typename Eol = void >
   struct read_input
      : internal::read_alias< Eol >
   {
      using internal::read_alias< Eol >::read_alias;
   };

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol = void, typename Data = char >
   struct mmap_input
      : internal::mmap_alias< Eol, Data >
   {
      using internal::mmap_alias< Eol, Data >::mmap_alias;
   };
#endif

   using tao_default_eol = TAO_PEGTL_DEFAULT_EOL;

   template< typename Eol = tao_default_eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   struct text_view_input
      : internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >
   {
      using internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >::choose_alias;
   };

   template< typename Eol = tao_default_eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   struct text_copy_input
      : internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >
   {
      using internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >::choose_alias;
   };

   template< typename Eol = tao_default_eol >
   struct text_file_input
      : internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >
   {
      using internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >::choose_alias;
   };

   template< typename Eol = tao_default_eol >
   struct text_read_input
      : internal::choose_alias< Eol, internal::lazy_read_alias< Eol >, internal::text_read_alias< Eol > >
   {
      using internal::choose_alias< Eol, internal::lazy_read_alias< Eol >, internal::text_read_alias< Eol > >::choose_alias;
   };

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol = tao_default_eol, typename Data = char >
   struct text_mmap_input
      : internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >
   {
      using internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >::choose_alias;
   };
#endif

}  // namespace TAO_PEGTL_NAMESPACE

#endif
