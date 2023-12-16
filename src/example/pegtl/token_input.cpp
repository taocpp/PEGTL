// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>

#include <tao/pegtl/analyze_traits.hpp>

// This file contains some experiments towards generalising inputs to
// represent sequences of arbitrary objects; it's not very complete, but
// it does get a minimal example up-and-running. One main limitation is
// that nothing that throws a parse_error can be used because positions
// aren't supported by the token_parse_input.

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Type, Type Value >
      struct token_type
      {
         using rule_t = token_type;
         using subs_t = empty_list;

         template< typename ParseInput >
         static bool match( ParseInput& in )
         {
            if( ( !in.empty() ) && ( in.current()->type == Value ) ) {
               in.template consume< internal::eol_exclude_tag >( 1 );
               return true;
            }
            return false;
         }
      };

   }  // namespace internal

   template< auto Value >
   using token_type = internal::token_type< decltype( Value ), Value >;

   template< typename Name, typename Type, Type Value >
   struct analyze_traits< Name, internal::token_type< Type, Value > >
      : analyze_any_traits<>
   {};

   template< typename Token >
   using token_input = internal::input_with_fakes< internal::copy_input< std::vector< Token > > >;

}  // namespace TAO_PEGTL_NAMESPACE

using namespace TAO_PEGTL_NAMESPACE;

enum my_type
{
   a,
   b,
   c,
   d,
   e,
   f
};

struct my_token
{
   my_type type;
   std::string data;
};

template< typename Rule >
struct my_action
   : nothing< Rule >
{};

template<>
struct my_action< eof >
{
   static void apply0()
   {
      std::cout << "We have eof!" << std::endl;
   }
};

template<>
struct my_action< token_type< my_type::b > >
{
   template< typename ActionInput >
   static void apply( const ActionInput& /*unused*/ )
   {
      std::cout << "We have a token of type 'b'!" << std::endl;
   }
};

struct my_grammar
   : seq< plus< token_type< my_type::b > >, eof >
{};

int main()
{
   const std::vector< my_token > v{
      { my_type::b, "" },
      { my_type::b, "" }
   };
   token_input< my_token > in( v );

   if( !parse< my_grammar, my_action >( in ) ) {
      return 1;
   }
   return 0;
}
