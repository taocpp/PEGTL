// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ARGV_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_ARGV_INPUT_WITH_SOURCE_HPP

#include <string>

#include "../config.hpp"

#include "argv_input.hpp"
#include "input_with_source.hpp"
#include "stream_to_string.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Source >
   struct argv_input_with_source;

   template<>
   struct argv_input_with_source< std::string >
      : input_with_source< std::string, std::string, argv_input >
   {
      argv_input_with_source( char** argv, const int argn )
         : input_with_source< std::string, std::string, argv_input >( stream_to_string( "argv[", argn, ']' ), argv, argn )
      {}
   };

   template<>
   struct argv_input_with_source< void >
      : argv_input
   {
      using argv_input::argv_input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
