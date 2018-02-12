// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_file.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf >
      struct open_input
         : public read_input< P, Eol >
      {
         explicit open_input( const char* in_filename )
            : read_input< P, Eol >( internal::file_open( in_filename ), in_filename )
         {
         }

         explicit open_input( const std::string& in_filename )
            : open_input( in_filename.c_str() )
         {
         }
      };

      void unit_test()
      {
         verify_file< read_input<> >();
         verify_file< open_input<> >();
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
