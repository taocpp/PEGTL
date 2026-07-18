// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <cstddef>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>
#include <tao/pegtl/example/json.hpp>
#include <tao/pegtl/extra/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace model
   {
      struct value;

      using array = std::vector< value >;
      using object = std::map< std::string, value >;

      struct value
      {
         std::variant< std::nullptr_t, bool, double, std::string, array, object > data;

         bool operator==( const value& ) const = default;
      };

   }  // namespace model

   namespace rules = json;

   using grammar = seq< rules::text, eof >;

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template< typename Rule >
   using json_value = create_for< Rule, model::value, test_action >;

   template<>
   struct test_action< rules::value >
      : variant_to< &model::value::data,
                    const_for< rules::null, nullptr >,
                    cases< case_< rules::false_, false >,
                           case_< rules::true_, true > >,
                    rules::number,
                    unescape_for< rules::string_content >,
                    repeat_for< rules::array, json_value< rules::array_element > >,
                    repeat_for< rules::object,
                                multi_for< rules::member,
                                           unescape_for< rules::key_content >,
                                           json_value< rules::member_value > > > >
   {};

   [[nodiscard]] bool parses_as( const std::string_view text, const model::value& expected )
   {
      text_view_input< scan::lf > in( text );
      model::value actual;

      return parse< grammar, test_action >( in, actual ) && in.empty() && ( actual == expected );
   }

   void unit_test()
   {
      using model::array;
      using model::object;
      using model::value;

      TAO_PEGTL_TEST_ASSERT( parses_as( " null ", value{ nullptr } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( "true", value{ true } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( "false", value{ false } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( "-1.5e2", value{ -150.0 } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( R"("a\n\u263a")", value{ std::string( "a\n\xe2\x98\xba" ) } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( "[]", value{ array{} } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( "{}", value{ object{} } ) );
      TAO_PEGTL_TEST_ASSERT( parses_as( R"({"array":[null,true,2.5,"text"],"object":{"answer":42}})",
                                        value{
                                           object{
                                                  { "array", value{ array{ value{ nullptr }, value{ true }, value{ 2.5 }, value{ std::string( "text" ) } } } },
                                                  { "object", value{ object{ { "answer", value{ 42.0 } } } } } }
      } ) );

      model::value duplicate;
      text_view_input< scan::lf > duplicate_input( R"({"same":1,"same":2})" );
      TAO_PEGTL_TEST_THROWS( parse< grammar, test_action >( duplicate_input, duplicate ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
