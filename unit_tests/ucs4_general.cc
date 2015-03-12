// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   namespace
   {
      std::string ucs4string( const uint32_t u )
      {
         return std::string( reinterpret_cast< const char * >( & u ), 4 );
      }

   } //

   void unit_test()
   {
      verify_rule< ucs4::any >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, "\xff", result_type::LOCAL_FAILURE, 1 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, "\xff\xff", result_type::LOCAL_FAILURE, 2 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, "\xff\xff\xff", result_type::LOCAL_FAILURE, 3 );

      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0 ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 1 ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x00ff ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x0100 ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x0fff ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x1000 ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0xfffe ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0xffff ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x100000 ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x10fffe ), result_type::SUCCESS, 0 );
      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x10ffff ), result_type::SUCCESS, 0 );

      verify_rule< ucs4::any >( __LINE__, __FILE__, ucs4string( 0x110000 ), result_type::LOCAL_FAILURE, 4 );
   }

} // pegtl

#include "main.hh"
