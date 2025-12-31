// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_COPY_INPUT_HPP
#define TAO_PEGTL_COPY_INPUT_HPP

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
   struct copy_input
      : internal::copy_alias< Eol, Container, InputSource, ErrorSource >
   {
      using internal::copy_alias< Eol, Container, InputSource, ErrorSource >::copy_alias;
   };

   template< typename Data >
   copy_input( const Data*, const Data* ) -> copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   copy_input( const Data*, const std::size_t ) -> copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Container >
   copy_input( Container&& ) -> copy_input< default_eol, std::decay_t< Container >, void, void >;

   template< typename Container >
   copy_input( const Container& ) -> copy_input< default_eol, std::decay_t< Container >, void, void >;

   template< typename Data, std::size_t Size >
   copy_input( const std::array< Data, Size >& ) -> copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   copy_input( const std::initializer_list< Data >& ) -> copy_input< default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename String, typename Data >
   copy_input( String&&, const Data*, const Data* ) -> copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

   template< typename String, typename Data >
   copy_input( String&&, const Data*, const std::size_t ) -> copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

   template< typename String, typename Container >
   copy_input( String&&, Container&& ) -> copy_input< default_eol, std::decay_t< Container >, std::string, std::string >;

   template< typename String, typename Container >
   copy_input( String&&, const Container& ) -> copy_input< default_eol, std::decay_t< Container >, std::string, std::string >;

   template< typename String, typename Data, std::size_t Size >
   copy_input( String&&, const std::array< Data, Size >& ) -> copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

   template< typename String, typename Data >
   copy_input( String&&, const std::initializer_list< Data >& ) -> copy_input< default_eol, internal::choose_container_t< Data >, std::string, std::string >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
