// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_COUNT_DATA_HPP
#define TAOCPP_PEGTL_INCLUDE_COUNT_DATA_HPP

#include <cstdlib>

#include "config.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      struct count_data
      {
         count_data( const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line, const char* in_data ) noexcept
            : byte( in_byte ),
              line( in_line ),
              byte_in_line( in_byte_in_line ),
              data( in_data )
         {
         }

         std::size_t byte;
         std::size_t line;
         std::size_t byte_in_line;

         const char* data;
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
