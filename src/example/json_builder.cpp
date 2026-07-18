// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
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

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include <tao/pegtl.hpp>

#include <tao/pegtl/action/builders.hpp>
#include <tao/pegtl/example/json.hpp>
#include <tao/pegtl/extra/builders.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   namespace rules = pegtl::json;

   using grammar = pegtl::seq< rules::text, pegtl::eof >;

   // The data structure that will hold the parsed JSON.

   struct value;

   using array = std::vector< value >;
   using object = std::map< std::string, value >;

   struct value
   {
      std::variant< std::nullptr_t, bool, double, std::string, array, object > data;
   };

   // The builder-style action that will parse JSON in the data structure.

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   template< typename Rule >
   using json_value = pegtl::create_for< Rule, value, action >;

   template<>
   struct action< rules::value >
      : pegtl::variant_to<
         &value::data,
         pegtl::const_for< rules::null, nullptr >,
         pegtl::cases< pegtl::case_< rules::false_, false >,
                       pegtl::case_< rules::true_, true > >,
         rules::number,
         pegtl::unescape_for< rules::string_content >,
         pegtl::repeat_for< rules::array, json_value< rules::array_element > >,
         pegtl::repeat_for< rules::object, pegtl::multi_for< rules::member,
                                                             pegtl::unescape_for< rules::key_content >,
                                                             json_value< rules::member_value > > > >
   {};

   // A simple function to do ... something with the parsed JSON.

   [[nodiscard]] std::size_t count_values( const value& v )
   {
      return std::visit( []( const auto& data ) -> std::size_t {
         using data_t = std::decay_t< decltype( data ) >;

         if constexpr( std::is_same_v< data_t, array > ) {
            std::size_t result = 1;
            for( const auto& element : data ) {
               result += count_values( element );
            }
            return result;
         }
         else if constexpr( std::is_same_v< data_t, object > ) {
            std::size_t result = 1;
            for( const auto& member : data ) {
               result += count_values( member.second );
            }
            return result;
         }
         else {
            return 1;
         }
      }, v.data );
   }

}  // namespace example

int main( int argc, char** argv )
{
   int failed = 0;

   for( int i = 1; i < argc; ++i ) {
      example::value out;
      pegtl::text_file_input in( argv[ i ] );

      if( pegtl::parse< example::grammar, example::action >( in, out ) ) {
         std::cout << argv[ i ] << " has " << example::count_values( out ) << " values" << std::endl;
      }
      else {
         std::cerr << argv[ i ] << ": parse failed" << std::endl;
         ++failed;
      }
   }
   return failed;
}

#endif
