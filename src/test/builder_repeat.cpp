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

#include <list>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct bag
   {
      std::vector< int > values;

      bool operator==( const bag& ) const = default;
   };

   template<>
   struct repeat_traits< bag >
   {
      using element_t = int;

      template< typename ParseInput >
      static void add( const ParseInput& /*unused*/, bag& out, int&& value )
      {
         out.values.emplace_back( value * 10 );
      }
   };

   struct test_target
   {
      std::vector< int > numbers;
      std::list< int > number_list;
      std::set< int > number_set;
      std::multiset< int > number_multiset;
      std::map< std::string, int > entry_map;
      std::multimap< std::string, int > entry_multimap;
      std::vector< std::pair< std::string, int > > entries;
      std::vector< std::vector< int > > groups;
      bag custom;
   };

   struct number
      : plus< digit >
   {};

   struct numbers
      : list< number, one< ',' > >
   {};

   struct maybe_numbers
      : opt< numbers >
   {};

   struct bad_numbers
      : seq< number, one< ',' >, one< '!' > >
   {};

   struct list_numbers
      : numbers
   {};

   struct set_numbers
      : numbers
   {};

   struct multiset_numbers
      : numbers
   {};

   struct bag_numbers
      : numbers
   {};

   struct name
      : plus< alpha >
   {};

   struct entry_number
      : plus< digit >
   {};

   struct entry
      : seq< name, one< '=' >, entry_number >
   {};

   struct entries_rule
      : list< entry, one< ',' > >
   {};

   struct map_entries
      : entries_rule
   {};

   struct multimap_entries
      : entries_rule
   {};

   struct group_number
      : plus< digit >
   {};

   struct group_numbers
      : list< group_number, one< ',' > >
   {};

   struct group
      : seq< one< '[' >, opt< group_numbers >, one< ']' > >
   {};

   struct groups_rule
      : list< group, one< ';' > >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< numbers >
      : repeat_to< &test_target::numbers, number >
   {};

   template<>
   struct test_action< maybe_numbers >
      : repeat_to< &test_target::number_list, number >
   {};

   template<>
   struct test_action< bad_numbers >
      : repeat_to< &test_target::numbers, number >
   {};

   template<>
   struct test_action< list_numbers >
      : repeat_to< &test_target::number_list, number >
   {};

   template<>
   struct test_action< set_numbers >
      : repeat_to< &test_target::number_set, number >
   {};

   template<>
   struct test_action< multiset_numbers >
      : repeat_to< &test_target::number_multiset, number >
   {};

   template<>
   struct test_action< bag_numbers >
      : repeat_to< &test_target::custom, number >
   {};

   template<>
   struct test_action< entries_rule >
      : repeat_to< &test_target::entries,
                   multi_for< entry, name, entry_number > >
   {};

   template<>
   struct test_action< map_entries >
      : repeat_to< &test_target::entry_map,
                   multi_for< entry, name, entry_number > >
   {};

   template<>
   struct test_action< multimap_entries >
      : repeat_to< &test_target::entry_multimap,
                   multi_for< entry, name, entry_number > >
   {};

   template<>
   struct test_action< groups_rule >
      : repeat_to< &test_target::groups,
                   repeat_for< group, group_number > >
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

      parse_into< numbers >( "1,2,3", target );
      parse_into< maybe_numbers >( "", target );
      parse_into< list_numbers >( "4,5,6", target );
      parse_into< set_numbers >( "3,1,2", target );
      parse_into< multiset_numbers >( "1,2,1", target );
      parse_into< bag_numbers >( "7,8", target );
      parse_into< entries_rule >( "alpha=1,beta=2", target );
      parse_into< map_entries >( "alpha=1,beta=2", target );
      parse_into< multimap_entries >( "alpha=1,alpha=2", target );
      parse_into< groups_rule >( "[1,2];[];[3]", target );

      TAO_PEGTL_TEST_ASSERT( target.numbers == std::vector< int >( { 1, 2, 3 } ) );
      TAO_PEGTL_TEST_ASSERT( target.number_list == std::list< int >( { 4, 5, 6 } ) );
      TAO_PEGTL_TEST_ASSERT( target.number_set == std::set< int >( { 1, 2, 3 } ) );
      TAO_PEGTL_TEST_ASSERT( target.number_multiset == std::multiset< int >( { 1, 1, 2 } ) );
      TAO_PEGTL_TEST_ASSERT( target.custom == bag{
                                { 70, 80 }
      } );
      TAO_PEGTL_TEST_ASSERT( target.entries == std::vector< std::pair< std::string, int > >( {
                                { "alpha", 1 },
                                { "beta",  2 }
      } ) );
      TAO_PEGTL_TEST_ASSERT( target.entry_map == std::map< std::string, int >( {
                                { "alpha", 1 },
                                { "beta",  2 }
      } ) );
      TAO_PEGTL_TEST_ASSERT( target.entry_multimap == std::multimap< std::string, int >( {
                                { "alpha", 1 },
                                { "alpha", 2 }
      } ) );
      TAO_PEGTL_TEST_ASSERT( target.groups == std::vector< std::vector< int > >( {
                                { 1, 2 },
                                {},
                                { 3 }
      } ) );

      test_target failed;
      failed.numbers = { 9 };
      text_view_input< scan::lf > bad_input( "1," );

      TAO_PEGTL_TEST_ASSERT( !parse< bad_numbers, test_action >( bad_input, failed ) );
      TAO_PEGTL_TEST_ASSERT( failed.numbers == std::vector< int >( { 9 } ) );

      test_target duplicate_set;
      text_view_input< scan::lf > duplicate_set_input( "1,2,1" );
      TAO_PEGTL_TEST_THROWS( parse< set_numbers, test_action >( duplicate_set_input, duplicate_set ) );

      test_target duplicate_map;
      text_view_input< scan::lf > duplicate_map_input( "alpha=1,alpha=2" );
      TAO_PEGTL_TEST_THROWS( parse< map_entries, test_action >( duplicate_map_input, duplicate_map ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
