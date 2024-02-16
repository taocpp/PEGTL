// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUTS_HPP
#define TAO_PEGTL_INTERNAL_INPUTS_HPP

#include <filesystem>
#include <type_traits>

#include "../config.hpp"
#include "../system.hpp"

#include "argv_input.hpp"
#include "copy_input.hpp"
#include "file_input.hpp"
#include "lazy_input.hpp"
// mmap_input.hpp via file_input.hpp
#include "read_input.hpp"
#include "text_input.hpp"
#include "view_input.hpp"

#include "input_with_fakes.hpp"
#include "input_with_funcs.hpp"
#include "input_with_lines.hpp"
#include "input_with_source.hpp"
#include "input_with_start.hpp"

#include "input_double_path.hpp"

#include "argv_input_with_source.hpp"
#include "text_input_with_source.hpp"

#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   using func_short = input_with_fakes< input_with_funcs< Input > >;

   template< typename Eol, typename Input >
   using base_short = input_with_fakes< input_with_funcs< input_with_lines< Eol, Input > > >;

   template< typename Eol, typename Input >
   using text_short = input_double_path< text_input_with_source< Eol, std::filesystem::path, std::filesystem::path, Input > >;

   template< typename Input >
   using path_short = input_double_path< input_with_source< std::filesystem::path, std::filesystem::path, Input > >;

   template< typename Eol, typename Data >
   using base_alias = base_short< Eol, view_input< Data > >;

   template< typename Eol, typename Source >
   using argv_alias = base_short< Eol, argv_input_with_source< Source > >;

   template< typename Eol, typename Data, typename InputSource, typename ErrorSource >
   using view_alias = base_short< Eol, input_with_source< InputSource, ErrorSource, input_with_start< view_input< Data > > > >;

   template< typename Eol, typename Container, typename InputSource, typename ErrorSource >
   using copy_alias = base_short< Eol, input_with_source< InputSource, ErrorSource, copy_input< Container > > >;

   template< typename Eol >
   using file_alias = base_short< Eol, path_short< file_input > >;

   template< typename Eol >
   using read_alias = base_short< Eol, path_short< read_input > >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data >
   using mmap_alias = base_short< Eol, path_short< mmap_input< Data > > >;
#endif

   template< typename Eol, typename Data, typename InputSource, typename ErrorSource >
   using text_view_alias = func_short< text_input_with_source< Eol, InputSource, ErrorSource, input_with_start< view_input< Data > > > >;

   template< typename Eol, typename Container, typename InputSource, typename ErrorSource >
   using text_copy_alias = func_short< text_input_with_source< Eol, InputSource, ErrorSource, copy_input< Container > > >;

   template< typename Eol >
   using text_file_alias = func_short< text_short< Eol, file_input > >;

   template< typename Eol >
   using text_read_alias = func_short< text_short< Eol, read_input > >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data >
   using text_mmap_alias = func_short< text_short< Eol, mmap_input< Data > > >;
#endif

   template< typename Eol, typename Data, typename InputSource, typename ErrorSource >
   using lazy_view_alias = func_short< input_with_source< InputSource, ErrorSource, lazy_input< Eol, input_with_start< view_input< Data > > > > >;

   template< typename Eol, typename Container, typename InputSource, typename ErrorSource >
   using lazy_copy_alias = func_short< input_with_source< InputSource, ErrorSource, lazy_input< Eol, copy_input< Container > > > >;

   template< typename Eol >
   using lazy_file_alias = func_short< path_short< lazy_input< Eol, file_input > > >;

   template< typename Eol >
   using lazy_read_alias = func_short< path_short< lazy_input< Eol, read_input > > >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol, typename Data >
   using lazy_mmap_alias = func_short< path_short< lazy_input< Eol, mmap_input< Data > > > >;
#endif

   template< typename Eol, typename Lazy, typename Text >
   using choose_alias = std::conditional_t< has_eol_lazy_peek< Eol >, Lazy, Text >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
