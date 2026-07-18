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

#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct aggregate_entry
   {
      std::string name;
      int value = 0;

      bool operator==( const aggregate_entry& ) const = default;
   };

   struct constructed_entry
   {
      std::string name;
      int value = 0;

      constructed_entry() = default;

      constructed_entry( std::string in_name, const int in_value )
         : name( std::move( in_name ) ),
           value( in_value )
      {}

      bool operator==( const constructed_entry& ) const = default;
   };

   struct made_entry
   {
      std::string name;
      int value = 0;

      bool operator==( const made_entry& ) const = default;
   };

   template<>
   struct multi_traits< aggregate_entry >
   {
      using fields_t = type_list< std::string, int >;
   };

   template<>
   struct multi_traits< constructed_entry >
   {
      using fields_t = type_list< std::string, int >;
   };

   template<>
   struct multi_traits< made_entry >
   {
      using fields_t = type_list< std::string, int >;

      [[nodiscard]] static made_entry make( std::string name, const int value )
      {
         return { std::move( name ), value + 100 };
      }
   };

   struct test_target
   {
      std::string name;
      int value = 0;
      std::optional< int > maybe_number;
      std::optional< std::string > maybe_name;
      std::pair< std::string, int > pair;
      std::tuple< std::string, int > tuple;
      std::tuple< int > single_tuple;
      aggregate_entry aggregate;
      constructed_entry constructed;
      made_entry made;
      std::vector< std::pair< std::string, int > > entries;
      std::string group_name;
      std::vector< int > group_values;
      std::string reverse_name;
      int reverse_value = 0;

      void set( std::string in_name, const int in_value )
      {
         name = std::move( in_name );
         value = in_value;
      }

      void set_optional( std::optional< int > in_number, std::optional< std::string > in_name )
      {
         maybe_number = in_number;
         maybe_name = std::move( in_name );
      }

      void set_group( std::string in_name, std::vector< int > in_values )
      {
         group_name = std::move( in_name );
         group_values = std::move( in_values );
      }

      void set_reverse( const int in_value, std::string in_name )
      {
         reverse_name = std::move( in_name );
         reverse_value = in_value;
      }
   };

   struct name
      : plus< alpha >
   {};

   struct number
      : plus< digit >
   {};

   struct method_rule
      : seq< name, one< '=' >, number, eof >
   {};

   struct pair_rule
      : seq< name, one< '=' >, number, eof >
   {};

   struct tuple_rule
      : seq< name, one< '=' >, number, eof >
   {};

   struct single_tuple_rule
      : seq< number, eof >
   {};

   struct aggregate_rule
      : seq< name, one< '=' >, number, eof >
   {};

   struct constructed_rule
      : seq< name, one< '=' >, number, eof >
   {};

   struct made_rule
      : seq< name, one< '=' >, number, eof >
   {};

   struct entry
      : seq< name, one< '=' >, number >
   {};

   struct entries_rule
      : list< entry, one< ',' > >
   {};

   struct optional_number
      : plus< digit >
   {};

   struct optional_name
      : plus< alpha >
   {};

   struct optional_rule
      : seq< opt< optional_number >, one< ':' >, opt< optional_name >, eof >
   {};

   struct group_name
      : plus< alpha >
   {};

   struct group_number
      : plus< digit >
   {};

   struct group_numbers
      : list< group_number, one< ',' > >
   {};

   struct bracketed_numbers
      : seq< one< '[' >, opt< group_numbers >, one< ']' > >
   {};

   struct group_rule
      : seq< group_name, one< ':' >, bracketed_numbers, eof >
   {};

   struct reverse_name
      : plus< alpha >
   {};

   struct reverse_number
      : plus< digit >
   {};

   struct reverse_rule
      : seq< reverse_name, one< ':' >, reverse_number, eof >
   {};

   struct bad_name
      : plus< alpha >
   {};

   struct bad_number
      : plus< digit >
   {};

   struct bad_rule
      : seq< bad_name, one< '=' >, bad_number, one< '!' > >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< method_rule >
      : multi_to< &test_target::set, name, number >
   {};

   template<>
   struct test_action< pair_rule >
      : multi_to< &test_target::pair, name, number >
   {};

   template<>
   struct test_action< tuple_rule >
      : multi_to< &test_target::tuple, name, number >
   {};

   template<>
   struct test_action< single_tuple_rule >
      : multi_to< &test_target::single_tuple, number >
   {};

   template<>
   struct test_action< aggregate_rule >
      : multi_to< &test_target::aggregate, name, number >
   {};

   template<>
   struct test_action< constructed_rule >
      : multi_to< &test_target::constructed, name, number >
   {};

   template<>
   struct test_action< made_rule >
      : multi_to< &test_target::made, name, number >
   {};

   template<>
   struct test_action< entries_rule >
      : repeat_to< &test_target::entries,
                   multi_for< entry, name, number > >
   {};

   template<>
   struct test_action< optional_rule >
      : multi_to< &test_target::set_optional, optional_number, optional_name >
   {};

   template<>
   struct test_action< group_rule >
      : multi_to< &test_target::set_group,
                  group_name,
                  repeat_for< bracketed_numbers, group_number > >
   {};

   template<>
   struct test_action< reverse_rule >
      : multi_to< &test_target::set_reverse,
                  reverse_number,
                  reverse_name >
   {};

   template<>
   struct test_action< bad_rule >
      : multi_to< &test_target::set, bad_name, bad_number >
   {};

   template< typename Rule >
   void parse_into( const std::string_view text, test_target& target )
   {
      text_view_input< scan::lf > in( text );
      TAO_PEGTL_TEST_ASSERT( parse< Rule, test_action >( in, target ) );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

   void unit_test()
   {
      test_target target;

      parse_into< method_rule >( "alpha=1", target );
      parse_into< pair_rule >( "beta=2", target );
      parse_into< tuple_rule >( "gamma=3", target );
      parse_into< single_tuple_rule >( "4", target );
      parse_into< aggregate_rule >( "delta=5", target );
      parse_into< constructed_rule >( "epsilon=6", target );
      parse_into< made_rule >( "zeta=7", target );
      parse_into< entries_rule >( "one=1,two=2", target );
      parse_into< optional_rule >( "8:maybe", target );
      parse_into< group_rule >( "group:[1,2,3]", target );
      parse_into< reverse_rule >( "reverse:9", target );

      TAO_PEGTL_TEST_ASSERT( target.name == "alpha" );
      TAO_PEGTL_TEST_ASSERT( target.value == 1 );
      TAO_PEGTL_TEST_ASSERT( target.pair == std::pair< std::string, int >( "beta", 2 ) );
      TAO_PEGTL_TEST_ASSERT( target.tuple == std::tuple< std::string, int >( "gamma", 3 ) );
      TAO_PEGTL_TEST_ASSERT( target.single_tuple == std::tuple< int >( 4 ) );
      TAO_PEGTL_TEST_ASSERT( target.aggregate == aggregate_entry{ "delta", 5 } );
      TAO_PEGTL_TEST_ASSERT( target.constructed == constructed_entry( "epsilon", 6 ) );
      TAO_PEGTL_TEST_ASSERT( target.made == made_entry{ "zeta", 107 } );
      TAO_PEGTL_TEST_ASSERT( target.entries == std::vector< std::pair< std::string, int > >( {
                                { "one", 1 },
                                { "two", 2 }
      } ) );
      TAO_PEGTL_TEST_ASSERT( target.maybe_number == 8 );
      TAO_PEGTL_TEST_ASSERT( target.maybe_name == "maybe" );
      TAO_PEGTL_TEST_ASSERT( target.group_name == "group" );
      TAO_PEGTL_TEST_ASSERT( target.group_values == std::vector< int >( { 1, 2, 3 } ) );
      TAO_PEGTL_TEST_ASSERT( target.reverse_name == "reverse" );
      TAO_PEGTL_TEST_ASSERT( target.reverse_value == 9 );

      test_target absent;
      parse_into< optional_rule >( ":", absent );
      TAO_PEGTL_TEST_ASSERT( !absent.maybe_number );
      TAO_PEGTL_TEST_ASSERT( !absent.maybe_name );

      test_target failed;
      failed.name = "before";
      failed.value = 42;
      text_view_input< scan::lf > bad_input( "changed=7" );

      TAO_PEGTL_TEST_ASSERT( !parse< bad_rule, test_action >( bad_input, failed ) );
      TAO_PEGTL_TEST_ASSERT( failed.name == "before" );
      TAO_PEGTL_TEST_ASSERT( failed.value == 42 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
