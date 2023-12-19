// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_INPUT_TRAITS_HPP

#include <string>

#include "../config.hpp"

#include "inputs.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input, typename = void >
   struct input_traits;

   // TODO: Find a better structure.

   template< typename Data >
   struct input_traits< view_input< Data > >
   {
      static constexpr bool is_argv = false;
      static constexpr bool is_copy = false;
      static constexpr bool is_file = false;
      static constexpr bool is_lazy = false;
      static constexpr bool is_mmap = false;
      static constexpr bool is_read = false;
      static constexpr bool is_text = false;
      static constexpr bool is_view = true;
      static constexpr bool has_start = false;
   };

   template< typename Container >
   struct input_traits< copy_input< Container > >
   {
      static constexpr bool is_argv = false;
      static constexpr bool is_copy = true;
      static constexpr bool is_file = false;
      static constexpr bool is_lazy = false;
      static constexpr bool is_mmap = false;
      static constexpr bool is_read = false;
      static constexpr bool is_text = false;
      static constexpr bool is_view = false;
      static constexpr bool has_start = true;
   };

   template<>
   struct input_traits< argv_input >
      : input_traits< view_input< char > >
   {
      static constexpr bool is_argv = true;
   };

   template<>
   struct input_traits< read_input >
      : input_traits< copy_input< std::string > >
   {
      static constexpr bool is_read = true;
   };

#if defined( TAO_PEGTL_MMAP_AVAILABLE )

   template< typename Data >
   struct input_traits< mmap_input< Data > >
   {
      static constexpr bool is_mmap = true;
   };

   template<>
   struct input_traits< file_input >
      : input_traits< mmap_input< char > >
   {
      static constexpr bool is_file = true;
   };

#else

   template<>
   struct input_traits< file_input >
      : input_traits< read_input >
   {
      static constexpr bool is_file = true;
   };

#endif

   template< typename Eol, typename Input >
   struct input_traits< lazy_input< Eol, Input > >
      : input_traits< input_with_lines< Eol, Input > >
   {
      static constexpr bool is_lazy = true;
   };

   template< typename Eol, typename Input >
   struct input_traits< text_input< Eol, Input > >
      : input_traits< input_with_lines< Eol, Input > >
   {
      static constexpr bool is_text = true;
   };

   template< typename Input >
   struct input_traits< input_with_fakes< Input > >
      : input_traits< Input >
   {
      static constexpr bool has_fakes = true;
   };

   template< typename Eol, typename Input >
   struct input_traits< input_with_lines< Eol, Input > >
      : input_traits< Input >
   {
      using eol_rule = Eol;

      static constexpr bool has_lines = true;
   };

   template< typename Input >
   struct input_traits< input_with_peeks< Input > >
      : input_traits< Input >
   {
      static constexpr bool has_peeks = true;
   };

   template< typename Source, typename Input >
   struct input_traits< input_with_source< Source, Input > >
      : input_traits< Input >
   {
      using source_t = Source;

      static constexpr bool has_source = true;
   };

   template< typename Input >
   struct input_traits< input_with_start< Input > >
      : input_traits< Input >
   {
      static constexpr bool has_start = true;
   };

   template< typename Input >
   struct input_traits< input_double_path< Input > >
      : input_traits< Input >
   {};

   template<>
   struct input_traits< argv_input_with_source >
      : input_traits< input_with_source< std::string, argv_input > >
   {};

   template< typename Eol, typename Source, typename Input >
   struct input_traits< text_input_with_source< Eol, Source, Input > >
      : input_traits< input_with_lines< Eol, Input > >
   {
      using source_t = Source;

      static constexpr bool has_source = true;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
