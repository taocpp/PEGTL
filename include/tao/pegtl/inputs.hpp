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
   template< typename Eol, typename Data = char >
   using base_input = internal::base_combination< Eol, internal::view_input< Data > >;
   template< typename Eol = void, typename Source = std::string >
   using argv_input = internal::base_combination< Eol, internal::argv_input_with_source< Source > >;
   template< typename Eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   using view_input = internal::base_combination< Eol, internal::input_with_source< InputSource, ErrorSource, internal::input_with_start< internal::view_input< Data > > > >;
   template< typename Eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   using copy_input = internal::base_combination< Eol, internal::input_with_source< InputSource, ErrorSource, internal::copy_input< Container > > >;
   template< typename Eol >
   using file_input = internal::base_combination< Eol, internal::path_combination< internal::file_input > >;
   template< typename Eol >
   using read_input = internal::base_combination< Eol, internal::path_combination< internal::read_input > >;
#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data = char >
   using mmap_input = internal::base_combination< Eol, internal::path_combination< internal::mmap_input< Data > > >;
#endif
   template< typename Eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   using text_view_input = internal::input_with_funcs< internal::text_input_with_source< Eol, InputSource, ErrorSource, internal::input_with_start< internal::view_input< Data > > > >;
   template< typename Eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   using text_copy_input = internal::input_with_funcs< internal::text_input_with_source< Eol, InputSource, ErrorSource, internal::copy_input< Container > > >;
   template< typename Eol >
   using text_file_input = internal::input_with_funcs< internal::text_combination< Eol, internal::file_input > >;
   template< typename Eol >
   using text_read_input = internal::input_with_funcs< internal::text_combination< Eol, internal::read_input > >;
#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data = char >
   using text_mmap_input = internal::input_with_funcs< internal::text_combination< Eol, internal::mmap_input< Data > > >;
#endif
   template< typename Eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   using lazy_copy_input = internal::input_with_funcs< internal::input_with_source< InputSource, ErrorSource, internal::lazy_input< Eol, internal::copy_input< Container > > > >;
   template< typename Eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   using lazy_view_input = internal::input_with_funcs< internal::input_with_source< InputSource, ErrorSource, internal::lazy_input< Eol, internal::input_with_start< internal::view_input< Data > > > > >;
   template< typename Eol >
   using lazy_file_input = internal::input_with_funcs< internal::path_combination< internal::lazy_input< Eol, internal::file_input > > >;
   template< typename Eol >
   using lazy_read_input = internal::input_with_funcs< internal::path_combination< internal::lazy_input< Eol, internal::read_input > > >;
#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data = char >
   using lazy_mmap_input = internal::input_with_funcs< internal::path_combination< internal::lazy_input< Eol, internal::mmap_input< Data > > > >;
#endif

}  // namespace TAO_PEGTL_NAMESPACE

#endif
