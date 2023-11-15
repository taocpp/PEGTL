// Copyright (c) 2017-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUTERATOR_HPP
#define TAO_PEGTL_INTERNAL_INPUTERATOR_HPP

#include <cassert>
#include <cstdlib>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   struct basic_small_position
   {
      basic_small_position() noexcept = default;

      explicit basic_small_position( const T* in_data ) noexcept
         : data( in_data )
      {}

      basic_small_position( basic_small_position&& ) noexcept = default;
      basic_small_position( const basic_small_position& ) noexcept = default;

      ~basic_small_position() = default;

      basic_small_position& operator=( basic_small_position&& ) noexcept = default;
      basic_small_position& operator=( const basic_small_position& ) noexcept = default;

      const T* data = nullptr;
   };

   using small_position = basic_small_position< char >;

   struct large_position
   {
      large_position() noexcept = default;

      explicit large_position( const char* in_data ) noexcept
         : data( in_data )
      {}

      explicit large_position( const small_position& in_small ) noexcept
         : large_position( in_small.data )
      {}

      large_position( const char* in_data, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_column ) noexcept
         : data( in_data ),
           byte( in_byte ),
           line( in_line ),
           column( in_column )
      {
         assert( in_line != 0 );
         assert( in_column != 0 );
      }

      large_position( large_position&& ) noexcept = default;
      large_position( const large_position& ) noexcept = default;

      ~large_position() = default;

      large_position& operator=( large_position&& ) noexcept = default;
      large_position& operator=( const large_position& ) noexcept = default;

      const char* data = nullptr;

      std::size_t byte = 0;
      std::size_t line = 1;
      std::size_t column = 1;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
