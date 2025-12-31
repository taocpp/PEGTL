// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_COPY_INPUT_HPP
#define TAO_PEGTL_TEXT_COPY_INPUT_HPP

#include <array>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "config.hpp"
#include "default_eol.hpp"

#include "internal/input_aliases.hpp"
#include "internal/choose_container.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = default_eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   struct text_copy_input
      : internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >
   {
      using internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >::choose_alias;
   };

   template< typename Data >
   text_copy_input( const Data*, const Data* ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   text_copy_input( const Data*, const std::size_t ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Container >
   text_copy_input( Container&& ) -> text_copy_input< default_eol, std::decay_t< Container >, void, void >;

   template< typename Container >
   text_copy_input( const Container& ) -> text_copy_input< default_eol, std::decay_t< Container >, void, void >;

   template< typename Data, std::size_t Size >
   text_copy_input( const std::array< Data, Size >& ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   text_copy_input( const std::initializer_list< Data >& ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename String, typename Data >
   text_copy_input( String&&, const Data*, const Data* ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

   template< typename String, typename Data >
   text_copy_input( String&&, const Data*, const std::size_t ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

   template< typename String, typename Container >
   text_copy_input( String&&, Container&& ) -> text_copy_input< default_eol, std::decay_t< Container >, std::string, std::string >;

   template< typename String, typename Container >
   text_copy_input( String&&, const Container& ) -> text_copy_input< default_eol, std::decay_t< Container >, std::string, std::string >;

   template< typename String, typename Data, std::size_t Size >
   text_copy_input( String&&, const std::array< Data, Size >& ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

   template< typename String, typename Data >
   text_copy_input( String&&, const std::initializer_list< Data >& ) -> text_copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
