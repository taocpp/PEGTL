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
   struct inputerator
   {
      inputerator() noexcept = default;

      explicit inputerator( const char* in_data ) noexcept
         : data( in_data )
      {}

      inputerator( const char* in_data, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_column ) noexcept
         : data( in_data ),
           byte( in_byte ),
           line( in_line ),
           column( in_column )
      {
         assert( in_line != 0 );
         assert( in_column != 0 );
      }

      inputerator( const inputerator& ) = default;
      inputerator( inputerator&& ) = default;

      ~inputerator() = default;

      inputerator& operator=( const inputerator& ) = default;
      inputerator& operator=( inputerator&& ) = default;

      const char* data = nullptr;

      std::size_t byte = 0;
      std::size_t line = 1;
      std::size_t column = 1;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
