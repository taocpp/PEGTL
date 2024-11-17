// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ARGV_INPUT_HPP
#define TAO_PEGTL_INTERNAL_ARGV_INPUT_HPP

#include <cstddef>
#include <cstring>

#include "../config.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif

#include "input_with_start.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct argv_input
      : input_with_start< view_input< char > >
   {
      argv_input( char** argv, const int argn )
         : input_with_start< view_input< char > >( argv[ argn ], std::strlen( argv[ argn ] ) )
      {}

      using error_position_t = typename input_with_start< view_input< char > >::error_position_t;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      [[nodiscard]] const char* begin_of_line( const error_position_t& pos, const std::size_t max = 135 ) const noexcept
      {
         const data_t* p = previous( pos );
         return ( std::size_t( p - this->start() ) > max ) ? ( p - max ) : this->start();
      }

      [[nodiscard]] const char* end_of_line_or_file( const error_position_t& pos, const std::size_t max = 135 ) const noexcept
      {
         const data_t* p = previous( pos );
         return ( std::size_t( this->end() - p ) > max ) ? ( p + max ) : this->end();
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
