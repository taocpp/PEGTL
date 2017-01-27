// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_COUNT_DATA_HH
#define PEGTL_COUNT_DATA_HH

#include <cstdlib>

#include "config.hh"

namespace PEGTL_NAMESPACE
{
   struct count_data
   {
      count_data( const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line, const char * in_data ) noexcept
            : byte( in_byte ),
              line( in_line ),
              byte_in_line( in_byte_in_line ),
              data( in_data )
      { }

      std::size_t byte;
      std::size_t line;
      std::size_t byte_in_line;

      const char * data;
   };

} // namespace PEGTL_NAMESPACE

#endif
