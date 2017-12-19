// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_INPUT_PAIR_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_INPUT_PAIR_HPP

#include "../config.hpp"

namespace tao
{
   namespace pegtl
   {
      namespace internal
      {
         template< typename Data >
         struct input_pair
         {
            Data data;
            unsigned char size;

            using data_t = Data;

            explicit operator bool() const noexcept
            {
               return size > 0;
            }
         };

      }  // namespace internal

   }  // namespace pegtl

}  // namespace tao

#endif
