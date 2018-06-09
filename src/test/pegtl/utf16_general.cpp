// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_rule.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace
      {
         std::string u16s( const char16_t u )
         {
            return std::string( static_cast< const char* >( static_cast< const void* >( &u ) ), sizeof( u ) );
         }

         std::string u16s_be( const char16_t v )
         {
            const std::uint16_t u = internal::h_to_be( std::uint16_t( v ) );
            return std::string( static_cast< const char* >( static_cast< const void* >( &u ) ), sizeof( u ) );
         }

         std::string u16s_le( const char16_t v )
         {
            const std::uint16_t u = internal::h_to_le( std::uint16_t( v ) );
            return std::string( static_cast< const char* >( static_cast< const void* >( &u ) ), sizeof( u ) );
         }

      }  // namespace

      void test_utf16()
      {
         verify_rule< utf16::any >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, "\x01", result_type::LOCAL_FAILURE, 1 );
         verify_rule< utf16::any >( __LINE__, __FILE__, "\xff", result_type::LOCAL_FAILURE, 1 );

         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 1 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, "  ", result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x00ff ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x0100 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x0fff ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0x1000 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd900 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xde00 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xfffe ) + " ", result_type::SUCCESS, 1 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xffff ) + "  ", result_type::SUCCESS, 2 );

         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd7ff ) + u16s( 0xdfff ), result_type::SUCCESS, 2 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xdc00 ) + u16s( 0xdfff ), result_type::LOCAL_FAILURE, 4 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0x0020 ), result_type::LOCAL_FAILURE, 4 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0xff20 ), result_type::LOCAL_FAILURE, 4 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0xdf00 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xd800 ) + u16s( 0xdfff ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xdbff ) + u16s( 0xdc00 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::any >( __LINE__, __FILE__, u16s( 0xdbff ) + u16s( 0xdfff ), result_type::SUCCESS, 0 );

         verify_rule< utf16::one< 0x20 > >( __LINE__, __FILE__, u16s( 0x20 ), result_type::SUCCESS, 0 );
         verify_rule< utf16::one< 0x20ac > >( __LINE__, __FILE__, u16s( 0x20ac ), result_type::SUCCESS, 0 );
         verify_rule< utf16::one< 0x10437 > >( __LINE__, __FILE__, u16s( 0xd801 ) + u16s( 0xdc37 ), result_type::SUCCESS, 0 );

         verify_rule< utf16::bom >( __LINE__, __FILE__, u16s( 0xfeff ), result_type::SUCCESS, 0 );
         verify_rule< utf16::bom >( __LINE__, __FILE__, u16s( 0xfffe ), result_type::LOCAL_FAILURE, 2 );

         verify_rule< utf16::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s( 0x20 ) + u16s( 0x20ac ) + u16s( 0xd801 ) + u16s( 0xdc37 ) + u16s( 0x20 ), result_type::SUCCESS, 2 );
      }

      void test_utf16_be()
      {
         verify_rule< utf16_be::any >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, "\x01", result_type::LOCAL_FAILURE, 1 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, "\xff", result_type::LOCAL_FAILURE, 1 );

         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 1 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, "  ", result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x00ff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x0100 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x0fff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0x1000 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd900 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xde00 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xfffe ) + " ", result_type::SUCCESS, 1 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xffff ) + "  ", result_type::SUCCESS, 2 );

         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd7ff ) + u16s_be( 0xdfff ), result_type::SUCCESS, 2 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xdc00 ) + u16s_be( 0xdfff ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0x0020 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0xff20 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0xdf00 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xd800 ) + u16s_be( 0xdfff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xdbff ) + u16s_be( 0xdc00 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::any >( __LINE__, __FILE__, u16s_be( 0xdbff ) + u16s_be( 0xdfff ), result_type::SUCCESS, 0 );

         verify_rule< utf16_be::one< 0x20 > >( __LINE__, __FILE__, u16s_be( 0x20 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::one< 0x20ac > >( __LINE__, __FILE__, u16s_be( 0x20ac ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::one< 0x10437 > >( __LINE__, __FILE__, u16s_be( 0xd801 ) + u16s_be( 0xdc37 ), result_type::SUCCESS, 0 );

         verify_rule< utf16_be::bom >( __LINE__, __FILE__, u16s_be( 0xfeff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_be::bom >( __LINE__, __FILE__, u16s_be( 0xfffe ), result_type::LOCAL_FAILURE, 2 );

         verify_rule< utf16_be::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s_be( 0x20 ) + u16s_be( 0x20ac ) + u16s_be( 0xd801 ) + u16s_be( 0xdc37 ) + u16s_be( 0x20 ), result_type::SUCCESS, 2 );
      }

      void test_utf16_le()
      {
         verify_rule< utf16_le::any >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, "\x01", result_type::LOCAL_FAILURE, 1 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, "\xff", result_type::LOCAL_FAILURE, 1 );

         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 1 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, "  ", result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x00ff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x0100 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x0fff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0x1000 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd900 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xde00 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xfffe ) + " ", result_type::SUCCESS, 1 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xffff ) + "  ", result_type::SUCCESS, 2 );

         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd7ff ) + u16s_le( 0xdfff ), result_type::SUCCESS, 2 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xdc00 ) + u16s_le( 0xdfff ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0x0020 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0xff20 ), result_type::LOCAL_FAILURE, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0xdf00 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xd800 ) + u16s_le( 0xdfff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xdbff ) + u16s_le( 0xdc00 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::any >( __LINE__, __FILE__, u16s_le( 0xdbff ) + u16s_le( 0xdfff ), result_type::SUCCESS, 0 );

         verify_rule< utf16_le::one< 0x20 > >( __LINE__, __FILE__, u16s_le( 0x20 ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::one< 0x20ac > >( __LINE__, __FILE__, u16s_le( 0x20ac ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::one< 0x10437 > >( __LINE__, __FILE__, u16s_le( 0xd801 ) + u16s_le( 0xdc37 ), result_type::SUCCESS, 0 );

         verify_rule< utf16_le::bom >( __LINE__, __FILE__, u16s_le( 0xfeff ), result_type::SUCCESS, 0 );
         verify_rule< utf16_le::bom >( __LINE__, __FILE__, u16s_le( 0xfffe ), result_type::LOCAL_FAILURE, 2 );

         verify_rule< utf16_le::string< 0x20, 0x20ac, 0x10437 > >( __LINE__, __FILE__, u16s_le( 0x20 ) + u16s_le( 0x20ac ) + u16s_le( 0xd801 ) + u16s_le( 0xdc37 ) + u16s_le( 0x20 ), result_type::SUCCESS, 2 );
      }

      void unit_test()
      {
         test_utf16();
         test_utf16_be();
         test_utf16_le();
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
