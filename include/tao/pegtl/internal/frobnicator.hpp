// Copyright (c) 2017-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FROBNICATOR_HPP
#define TAO_PEGTL_INTERNAL_FROBNICATOR_HPP

#include <cassert>
#include <cstdlib>

namespace tao::pegtl::internal
{
   struct frobnicator
   {
      frobnicator() = default;

      explicit frobnicator( const char* in_data ) noexcept
         : data( in_data )
      {}

      frobnicator( const char* in_data, const std::size_t in_byte, const std::size_t in_line, const std::size_t in_column ) noexcept
         : data( in_data ),
           byte( in_byte ),
           line( in_line ),
           column( in_column )
      {
         assert( in_line != 0 );
         assert( in_column != 0 );
      }

      frobnicator( const frobnicator& ) = default;
      frobnicator( frobnicator&& ) = default;

      ~frobnicator() = default;

      frobnicator& operator=( const frobnicator& ) = default;
      frobnicator& operator=( frobnicator&& ) = default;

      const char* data = nullptr;

      std::size_t byte = 0;
      std::size_t line = 1;
      std::size_t column = 1;
   };

}  // namespace tao::pegtl::internal

#endif
