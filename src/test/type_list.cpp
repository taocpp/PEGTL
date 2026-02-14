// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <type_traits>

#include <tao/pegtl/type_list.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static_assert( std::is_same_v< empty_list, type_list<> > );

   static_assert( is_type_list_v< empty_list > );
   static_assert( is_type_list_v< type_list< int > > );
   static_assert( is_type_list_v< type_list< int, char > > );
   static_assert( !is_type_list_v< std::tuple<> > );
   static_assert( !is_type_list_v< std::tuple< int > > );
   static_assert( !is_type_list_v< std::tuple< int, char > > );

   static_assert( empty_list::size == 0 );
   static_assert( type_list< char >::size == 1 );
   static_assert( type_list< char, int >::size == 2 );
   static_assert( type_list< char, int, void >::size == 3 );

   static_assert( type_list_contains_v< void, void > );
   static_assert( type_list_contains_v< int, int, void, char > );
   static_assert( type_list_contains_v< void, int, void, char > );
   static_assert( type_list_contains_v< char, int, void, char > );
   static_assert( type_list_contains_v< void, type_list< void > > );
   static_assert( type_list_contains_v< int, type_list< int, void, char > > );
   static_assert( type_list_contains_v< void, type_list< int, void, char > > );
   static_assert( type_list_contains_v< char, type_list< int, void, char > > );
   static_assert( !type_list_contains_v< int, empty_list > );
   static_assert( !type_list_contains_v< void, empty_list > );
   static_assert( !type_list_contains_v< int, type_list< char, unsigned, void > > );

   // static_assert( !type_list_contains_v< type_list< int >, void > );
   // static_assert( !type_list_contains_v< type_list< void >, int > );
   // static_assert( !type_list_contains_v< type_list< unsigned >, int > );

   // static_assert( !type_list_contains_v< void, type_list< type_list< void > > > );

   // static_assert( !type_list_contains_v< type_list< void >, type_list< type_list< void > > > );

   static_assert( std::is_same_v< type_list_append_t< empty_list >, empty_list > );
   static_assert( std::is_same_v< type_list_append_t< empty_list, int >, type_list< int > > );
   static_assert( std::is_same_v< type_list_append_t< empty_list, int, char >, type_list< int, char > > );
   static_assert( std::is_same_v< type_list_append_t< empty_list, empty_list >, type_list< empty_list > > );
   static_assert( std::is_same_v< type_list_append_t< type_list< int >, char >, type_list< int, char > > );
   static_assert( std::is_same_v< type_list_append_t< type_list< int, double >, char >, type_list< int, double, char > > );
   static_assert( std::is_same_v< type_list_append_t< type_list< int >, double, char >, type_list< int, double, char > > );
   static_assert( std::is_same_v< type_list_append_t< type_list< int, bool >, double, char >, type_list< int, bool, double, char > > );

   static_assert( std::is_same_v< type_list_concat_t< empty_list >, empty_list > );
   static_assert( std::is_same_v< type_list_concat_t< empty_list, empty_list >, empty_list > );
   static_assert( std::is_same_v< type_list_concat_t< type_list< void >, empty_list, empty_list >, type_list< void > > );
   static_assert( std::is_same_v< type_list_concat_t< empty_list, type_list< void >, empty_list >, type_list< void > > );
   static_assert( std::is_same_v< type_list_concat_t< empty_list, empty_list, type_list< void > >, type_list< void > > );
   static_assert( std::is_same_v< type_list_concat_t< type_list< int >, type_list< char > >, type_list< int, char > > );
   static_assert( std::is_same_v< type_list_concat_t< type_list< int >, type_list< char, void > >, type_list< int, char, void > > );
   static_assert( std::is_same_v< type_list_concat_t< type_list< int, unsigned >, type_list< char, void > >, type_list< int, unsigned, char, void > > );
   static_assert( std::is_same_v< type_list_concat_t< type_list< int >, type_list< char, void >, type_list< unsigned > >, type_list< int, char, void, unsigned > > );

}  // namespace TAO_PEGTL_NAMESPACE

int main()
{
   return 0;
}
