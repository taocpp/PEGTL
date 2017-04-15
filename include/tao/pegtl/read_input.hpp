// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_READ_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_READ_INPUT_HPP

#include <string>

#include "config.hpp"
#include "eol.hpp"
#include "string_input.hpp"
#include "tracking_mode.hpp"

#include "internal/file_reader.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         struct filename_holder
         {
            const std::string data;

            template< typename T >
            explicit filename_holder( T&& in_data )
               : data( std::forward< T >( in_data ) )
            {
            }
         };

      }  // namespace internal

      template< typename Eol = lf_crlf_eol, tracking_mode P = tracking_mode::IMMEDIATE >
      struct read_input
         : private internal::filename_holder,
           public string_input< Eol, P >
      {
         template< typename T >
         explicit read_input( T&& filename )
            : internal::filename_holder( std::forward< T >( filename ) ),
              string_input< Eol, P >( internal::file_reader( data.c_str() ).read(), data.c_str() )
         {
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
