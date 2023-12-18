// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>

#include <tao/pegtl/analyze_traits.hpp>

enum my_type
{
   alpha,
   beta,
   gamma,
   delta
};

struct my_token
{
   my_type type;
   std::string data;
};

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Type, Type Value >
      struct token_type_rule
      {
         using rule_t = token_type_rule;
         using subs_t = empty_list;

         template< typename ParseInput >
         static bool match( ParseInput& in )
         {
            if( ( !in.empty() ) && ( in.current()->type == Value ) ) {
               in.template consume< eol_exclude_tag >( 1 );
               return true;
            }
            return false;
         }
      };

   }  // namespace internal

   template< auto Value >
   using token_type_rule = internal::token_type_rule< decltype( Value ), Value >;

   template< typename Name, typename Type, Type Value >
   struct analyze_traits< Name, internal::token_type_rule< Type, Value > >
      : analyze_any_traits<>
   {};

   template< typename Token >
   using token_input = internal::input_with_fakes< internal::copy_input< std::vector< Token > > >;

}  // namespace TAO_PEGTL_NAMESPACE

using namespace TAO_PEGTL_NAMESPACE;

template< typename Rule >
struct my_action
   : nothing< Rule >
{};

template<>
struct my_action< eof >
{
   static void apply0()
   {
      std::cout << "We have eof." << std::endl;
   }
};

template<>
struct my_action< token_type_rule< my_type::beta > >
{
   template< typename ActionInput >
   static void apply( const ActionInput& in )
   {
      assert( in.size() == 1 );
      std::cout << "We have a token of type 'beta' with data '" << in.peek().data << "'." << std::endl;
   }
};

struct my_grammar
   : seq< plus< token_type_rule< my_type::beta > >, eof >
{};

int main()
{
   const std::vector< my_token > v{
      { my_type::beta, "first" },
      { my_type::beta, "second" }
   };
   token_input< my_token > in( v );

   if( !parse< my_grammar, my_action >( in ) ) {
      return 1;
   }
   return 0;
}
