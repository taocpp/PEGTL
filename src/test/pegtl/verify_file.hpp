// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_FILE_HPP
#define TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_FILE_HPP

#include <tao/pegtl.hpp>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      struct file_content : seq< TAOCPP_PEGTL_STRING( "dummy content" ), eol, discard >
      {
      };

      struct file_grammar : seq< rep_min_max< 11, 11, file_content >, eof >
      {
      };

      template< typename Rule >
      struct file_action
         : nothing< Rule >
      {
      };

      template<>
      struct file_action< eof >
      {
         template< typename Input >
         static void apply( const Input&, bool& flag )
         {
            flag = true;
         }
      };

      template< typename Rule >
      struct file_control
         : normal< Rule >
      {
      };

      template<>
      struct file_control< eof >
         : normal< eof >
      {
         template< typename Input >
         static void success( const Input&, bool& flag )
         {
            flag = true;
         }
      };

      template< typename T >
      void verify_file()
      {
         {
            const std::string f{ "src/test/pegtl/no_such_file.txt" };
            try {
               T p{ f };
               TAOCPP_PEGTL_TEST_ASSERT( !"no error on opening non-existing file" );
            }
            catch( const input_error& ) {
            }
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            TAOCPP_PEGTL_TEST_ASSERT( p.template parse< file_grammar >() );
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = true;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            TAOCPP_PEGTL_TEST_ASSERT( p.template parse< file_grammar >( flag ) );
            TAOCPP_PEGTL_TEST_ASSERT( flag == true );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = false;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            TAOCPP_PEGTL_TEST_ASSERT( p.template parse< file_grammar >( flag ) );
            TAOCPP_PEGTL_TEST_ASSERT( flag == false );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = false;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            const bool result = p.template parse< file_grammar, file_action >( flag );
            TAOCPP_PEGTL_TEST_ASSERT( result );
            TAOCPP_PEGTL_TEST_ASSERT( flag == true );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = false;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            const bool result = p.template parse< file_grammar, nothing, file_control >( flag );
            TAOCPP_PEGTL_TEST_ASSERT( result );
            TAOCPP_PEGTL_TEST_ASSERT( flag == true );
         }
         const char * foo = "foo";
         const memory_input m{ foo, foo + 3, foo };
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            TAOCPP_PEGTL_TEST_ASSERT( p.template parse_nested< file_grammar >( m ) );
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = true;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            TAOCPP_PEGTL_TEST_ASSERT( p.template parse_nested< file_grammar >( m, flag ) );
            TAOCPP_PEGTL_TEST_ASSERT( flag == true );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = false;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            TAOCPP_PEGTL_TEST_ASSERT( p.template parse_nested< file_grammar >( m, flag ) );
            TAOCPP_PEGTL_TEST_ASSERT( flag == false );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = false;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            const bool result = p.template parse_nested< file_grammar, file_action >( m, flag );
            TAOCPP_PEGTL_TEST_ASSERT( result );
            TAOCPP_PEGTL_TEST_ASSERT( flag == true );
         }
         {
            const std::string f{ "src/test/pegtl/file_data.txt" };
            T p{ f };
            bool flag = false;
            TAOCPP_PEGTL_TEST_ASSERT( p.source() == f );
            const bool result = p.template parse_nested< file_grammar, nothing, file_control >( m, flag );
            TAOCPP_PEGTL_TEST_ASSERT( result );
            TAOCPP_PEGTL_TEST_ASSERT( flag == true );
         }
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
