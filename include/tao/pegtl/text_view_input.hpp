// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_VIEW_INPUT_HPP
#define TAO_PEGTL_TEXT_VIEW_INPUT_HPP

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
   struct text_view_input
      : internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >
   {
      using internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >::choose_alias;
   };

   template< typename Data >
   text_view_input( const Data*, const Data* ) -> text_view_input< default_eol, Data, void, void >;

   template< typename Data >
   text_view_input( const Data*, const std::size_t ) -> text_view_input< default_eol, Data, void, void >;

   text_view_input( std::string& ) -> text_view_input< default_eol, char, void, void >;
   text_view_input( const std::string& ) -> text_view_input< default_eol, char, void, void >;
   text_view_input( const std::string_view ) -> text_view_input< default_eol, char, void, void >;

   template< typename Data, typename... Params >
   text_view_input( std::vector< Data, Params... >& ) -> text_view_input< default_eol, Data, void, void >;

   template< typename Data, typename... Params >
   text_view_input( const std::vector< Data, Params... >& ) -> text_view_input< default_eol, Data, void, void >;

   template< std::size_t Size >
   text_view_input( const char ( & )[ Size ] ) -> text_view_input< default_eol, char, void, void >;

   template< typename Data, std::size_t Size >
   text_view_input( const std::array< Data, Size >& ) -> text_view_input< default_eol, Data, void, void >;

   template< typename String, typename Data >
   text_view_input( String&&, const Data*, const Data* ) -> text_view_input< default_eol, Data, std::string, std::string >;

   template< typename String, typename Data >
   text_view_input( String&&, const Data*, const std::size_t ) -> text_view_input< default_eol, Data, std::string, std::string >;

   template< typename String >
   text_view_input( String&&, std::string& ) -> text_view_input< default_eol, char, std::string, std::string >;
   template< typename String >
   text_view_input( String&&, const std::string& ) -> text_view_input< default_eol, char, std::string, std::string >;
   template< typename String >
   text_view_input( String&&, const std::string_view ) -> text_view_input< default_eol, char, std::string, std::string >;

   template< typename String, typename Data, typename... Params >
   text_view_input( String&&, std::vector< Data, Params... >& ) -> text_view_input< default_eol, Data, std::string, std::string >;

   template< typename String, typename Data, typename... Params >
   text_view_input( String&&, const std::vector< Data, Params... >& ) -> text_view_input< default_eol, Data, std::string, std::string >;

   template< typename String, std::size_t Size >
   text_view_input( String&&, const char ( & )[ Size ] ) -> text_view_input< default_eol, char, std::string, std::string >;

   template< typename String, typename Data, std::size_t Size >
   text_view_input( String&&, const std::array< Data, Size >& ) -> text_view_input< default_eol, Data, std::string, std::string >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
