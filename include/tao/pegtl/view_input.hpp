// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_VIEW_INPUT_HPP
#define TAO_PEGTL_VIEW_INPUT_HPP

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "config.hpp"
#include "default_eol.hpp"

#include "internal/input_aliases.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = default_eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   struct view_input
      : internal::view_alias< Eol, Data, InputSource, ErrorSource >
   {
      using internal::view_alias< Eol, Data, InputSource, ErrorSource >::view_alias;
   };

   template< typename Data >
   view_input( const Data*, const Data* ) -> view_input< default_eol, Data, void, void >;

   template< typename Data >
   view_input( const Data*, const std::size_t ) -> view_input< default_eol, Data, void, void >;

   view_input( std::string& ) -> view_input< default_eol, char, void, void >;
   view_input( const std::string& ) -> view_input< default_eol, char, void, void >;
   view_input( const std::string_view ) -> view_input< default_eol, char, void, void >;

   template< typename Data, typename... Params >
   view_input( std::vector< Data, Params... >& ) -> view_input< default_eol, Data, void, void >;

   template< typename Data, typename... Params >
   view_input( const std::vector< Data, Params... >& ) -> view_input< default_eol, Data, void, void >;

   template< std::size_t Size >
   view_input( const char ( & )[ Size ] ) -> view_input< default_eol, char, void, void >;

   template< typename Data, std::size_t Size >
   view_input( const std::array< Data, Size >& ) -> view_input< default_eol, Data, void, void >;

   template< typename String, typename Data >
   view_input( String&&, const Data*, const Data* ) -> view_input< default_eol, Data, std::string, std::string >;

   template< typename String, typename Data >
   view_input( String&&, const Data*, const std::size_t ) -> view_input< default_eol, Data, std::string, std::string >;

   template< typename String >
   view_input( String&&, std::string& ) -> view_input< default_eol, char, std::string, std::string >;

   template< typename String >
   view_input( String&&, const std::string& ) -> view_input< default_eol, char, std::string, std::string >;

   template< typename String >
   view_input( String&&, const std::string_view ) -> view_input< default_eol, char, std::string, std::string >;

   template< typename String, typename Data, typename... Params >
   view_input( String&&, std::vector< Data, Params... >& ) -> view_input< default_eol, Data, std::string, std::string >;

   template< typename String, typename Data, typename... Params >
   view_input( String&&, const std::vector< Data, Params... >& ) -> view_input< default_eol, Data, std::string, std::string >;

   template< typename String, std::size_t Size >
   view_input( String&&, const char ( & )[ Size ] ) -> view_input< default_eol, char, std::string, std::string >;

   template< typename String, typename Data, std::size_t Size >
   view_input( String&&, const std::array< Data, Size >& ) -> view_input< default_eol, Data, std::string, std::string >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
