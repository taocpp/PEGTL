// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_BUMP_IMPL_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_BUMP_IMPL_HPP

#include "../config.hpp"
#include "../count_data.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         inline void bump( count_data & data, const std::size_t count, const int ch )
         {
            for ( std::size_t i = 0; i < count; ++i ) {
               if ( data.data[ i ] == ch ) {
                  ++data.line;
                  data.byte_in_line = 0;
               }
               else {
                  ++data.byte_in_line;
               }
            }
            data.byte += count;
            data.data += count;
         }

         inline void bump_in_this_line( count_data & data, const std::size_t count )
         {
            data.data += count;
            data.byte += count;
            data.byte_in_line += count;
         }

         inline void bump_to_next_line( count_data & data, const std::size_t count )
         {
            ++data.line;
            data.byte += count;
            data.byte_in_line = 0;
            data.data += count;
         }

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
