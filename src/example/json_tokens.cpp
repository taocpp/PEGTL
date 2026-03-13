// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/change_action_and_state.hpp>
#include <tao/pegtl/control/must_if.hpp>
#include <tao/pegtl/debug/analyze.hpp>
#include <tao/pegtl/example/json.hpp>
#include <tao/pegtl/extra/unescape.hpp>
#include <tao/pegtl/member.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

// This example shows a separate JSON lexer and parser.

// Part 1 -- Lexer.

namespace example
{
   struct json_one_rule
      : pegtl::one< '[', ']', '{', '}', ':', ',' >
   {};

   struct json_word_rule
      : pegtl::sor< pegtl::json::false_, pegtl::json::true_, pegtl::json::null >
   {};

   struct token_rule
      : pegtl::sor< json_one_rule, json_word_rule, pegtl::json::string, pegtl::json::number >
   {};

   struct lexer_rule
      : pegtl::seq< pegtl::star< pegtl::json::ws >, pegtl::until< pegtl::eof, pegtl::json::padr< token_rule > > >
   {};

   enum class token_type : char
   {
      null = 'n',
      true_ = 't',
      false_ = 'f',
      string = '"',
      number = '0',  // default
      begin_array = '[',
      end_array = ']',
      begin_object = '{',
      end_object = '}',
      name_separator = ':',
      value_separator = ','
   };

   struct json_token
   {
      explicit json_token( const token_type t ) noexcept
         : type( t )
      {}

      json_token( const token_type t, std::string&& d ) noexcept
         : type( t ),
           data( std::move( d ) )
      {}

      json_token( const token_type t, const std::string& d )
         : type( t ),
           data( d )
      {}

      token_type type;
      std::string data;
      // text_position pos;
   };

   template< typename Rule >
   struct lexer_action
      : pegtl::nothing< Rule >
   {};

   struct token_char_action
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::vector< json_token >& out )
      {
         assert( !in.empty() );
         out.emplace_back( token_type( in.peek_char() ) );
      }
   };

   template<>
   struct lexer_action< json_one_rule >
      : token_char_action
   {};

   template<>
   struct lexer_action< json_word_rule >
      : token_char_action
   {};


   template<>
   struct lexer_action< pegtl::json::string::content >
      : pegtl::change_action_and_state< pegtl::unescape, std::string >
   {
      template< typename ParseInput >
      static void success( const ParseInput& /*unused*/, std::string& unescaped, std::vector< json_token >& out )
      {
         out.emplace_back( token_type::string, unescaped );
      }

   };

   template<>
   struct lexer_action< pegtl::json::number >
   {
      template< typename ParseInput >
      static void apply( const ParseInput& in, std::vector< json_token >& out )
      {
         out.emplace_back( token_type::number, in.string() );  // Keep this simple, no conversion.
      }
   };

   // clang-format off
   template< typename > inline constexpr const char* lexer_errors = nullptr;

   template<> inline constexpr auto lexer_errors< token_rule > = "Expected a valid JSON token.";
   template<> inline constexpr auto lexer_errors< lexer_rule > = "Expected sequence of valid JSON.";
   // clang-format on

   struct lexer_error
   {
      template< typename Rule >
      static constexpr auto message = lexer_errors< Rule >;
   };

   template< typename Rule >
   using lexer_control = pegtl::must_if_n< lexer_error >::type< Rule >;

}  // namespace example

// Part 2 -- Parser (in the classical sense).

namespace example
{
   template< token_type T >
   struct token_type_rule
      : pegtl::member::one< &json_token::type, T >
   {};

   // clang-format off
   struct null_token : token_type_rule< token_type::null > {};
   struct true_token : token_type_rule< token_type::true_ > {};
   struct false_token : token_type_rule< token_type::false_ > {};

   struct number_token : token_type_rule< token_type::number > {};
   struct string_token : token_type_rule< token_type::string > {};

   struct begin_array : token_type_rule< token_type::begin_array > {};
   struct end_array : token_type_rule< token_type::end_array > {};
   struct begin_object : token_type_rule< token_type::begin_object > {};
   struct end_object : token_type_rule< token_type::end_object > {};
   struct name_separator : token_type_rule< token_type::name_separator > {};
   struct value_separator : token_type_rule< token_type::value_separator > {};

   struct value;

   struct array_tokens : pegtl::seq< begin_array, pegtl::opt< pegtl::list< value, value_separator > >, end_array > {};
   struct object_member : pegtl::seq< string_token, name_separator, value > {};
   struct object_tokens : pegtl::seq< begin_object, pegtl::opt< pegtl::list< object_member, value_separator > >, end_object > {};

   struct value : pegtl::sor< null_token, true_token, false_token, number_token, string_token, array_tokens, object_tokens > {};

   struct parser_rule : pegtl::until< pegtl::eof, value > {};

   template< typename > inline constexpr const char* parser_errors = nullptr;

   template<> inline constexpr auto parser_errors< name_separator > = "Expected ':' separator.";
   template<> inline constexpr auto parser_errors< end_array > = "Expected end of array (or another element).";
   template<> inline constexpr auto parser_errors< end_object > = "Expected end of object (or another member).";
   template<> inline constexpr auto parser_errors< parser_rule > = "Expected sequence of valid JSON.";
   // clang-format on

   struct parser_error
   {
      template< typename Rule >
      static constexpr auto message = parser_errors< Rule >;
   };

   template< typename Rule >
   using parser_control = pegtl::must_if_n< parser_error >::type< Rule >;

}  // namespace example

// Part 3 -- Combine Lexer and Parser.

namespace example
{
   [[nodiscard]] int main( const std::filesystem::path& file )
   {
      std::vector< json_token > tokens;

      try {
         pegtl::text_file_input in( file );
         pegtl::parse< lexer_rule, lexer_action, lexer_control >( in, tokens );
      }
      catch( const std::exception& e ) {
         std::cerr << "Parse error in lexer: " << e.what() << std::endl;
         return 1;
      }
      std::cout << "Lexer succeeded with " << tokens.size() << " tokens." << std::endl;

      try {
         pegtl::copy_input< void, std::vector< json_token > > in( tokens );
         pegtl::parse< parser_rule, pegtl::normal, parser_control >( in );
      }
      catch( const std::exception& e ) {
         std::cerr << "Parse error in parser: " << e.what() << std::endl;
         return 1;
      }
      std::cout << "Parser succeeded" << std::endl;
      return 0;
   }

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "usage: " << argv[ 0 ] << " <filename.json>\n";
      return 1;
   }
   return example::main( argv[ 1 ] );
}

#endif
