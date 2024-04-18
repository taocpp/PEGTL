// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <optional>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/debug/analyze.hpp>

namespace TAO_PEGTL_NAMESPACE::json
{
   struct token_rule : padr< sor< one< '[' >, one< ']' >, one< '{' >, one< '}' >, one< ':' >, one< ',' >, string, number, false_, true_, null > >
   {};

   struct token_file : until< eof, token_rule >
   {};

   enum class token_type
   {
      null,
      true_,
      false_,
      string,
      number,
      begin_array,
      end_array,
      begin_object,
      end_object,
      name_separator,
      value_separator
   };

   struct token_data
   {
      explicit token_data( const token_type t ) noexcept
         : type( t )
      {}

      token_type type;
      std::string data;
      // text_position pos;
   };

   template< typename Rule >
   struct token_action
      : nothing< Rule >
   {};

   template< token_type Type >
   struct token_simple_action
   {
      template< typename State >
      static void apply0( State& st )
      {
         st.emplace( Type );
      }
   };

   template<>
   struct token_action< one< '[' > >
      : token_simple_action< token_type::begin_array >
   {};

   template<>
   struct token_action< one< ']' > >
      : token_simple_action< token_type::end_array >
   {};

   template<>
   struct token_action< one< '{' > >
      : token_simple_action< token_type::begin_array >
   {};

   template<>
   struct token_action< one< '}' > >
      : token_simple_action< token_type::end_array >
   {};

   template<>
   struct token_action< null >
      : token_simple_action< token_type::null >
   {};

   template<>
   struct token_action< true_ >
      : token_simple_action< token_type::true_ >
   {};

   template<>
   struct token_action< false_ >
      : token_simple_action< token_type::false_ >
   {};

   struct token_vector
   {
      void emplace( const token_type t )
      {
         tokens.emplace_back( t );
      }

      std::vector< token_data > tokens;
   };

   template< typename ParseInput >
   [[nodiscard]] std::optional< token_data > parse_token( ParseInput& in )
   {
      std::optional< token_data > result;
      parse< token_rule, token_action >( in, result );
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE::json

int main()  // NOLINT(bugprone-exception-escape)
{
   return 0;
}
