// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BASE_INPUT_HPP
#define TAO_PEGTL_BASE_INPUT_HPP

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
   template< typename Eol = default_eol, typename Data = char >
   struct base_input
      : internal::base_alias< Eol, Data >
   {
      using internal::base_alias< Eol, Data >::base_alias;
   };

   template< typename Data >
   base_input( const Data*, const Data* ) -> base_input< default_eol, Data >;

   template< typename Data >
   base_input( const Data*, const std::size_t ) -> base_input< default_eol, Data >;

   base_input( std::string& ) -> base_input< default_eol, char >;
   base_input( const std::string& ) -> base_input< default_eol, char >;
   base_input( const std::string_view ) -> base_input< default_eol, char >;

   template< typename Data, typename... Params >
   base_input( std::vector< Data, Params... >& ) -> base_input< default_eol, Data >;

   template< typename Data, typename... Params >
   base_input( const std::vector< Data, Params... >& ) -> base_input< default_eol, Data >;

   template< std::size_t Size >
   base_input( const char ( & )[ Size ] ) -> base_input< default_eol, char >;

   template< typename Data, std::size_t Size >
   base_input( const std::array< Data, Size >& ) -> base_input< default_eol, Data >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
