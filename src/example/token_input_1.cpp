// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>

#include <tao/pegtl/debug/analyze_traits.hpp>

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
               in.template consume< token_type_rule >( 1 );
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
   using token_input = copy_input< void, std::vector< Token > >;

}  // namespace TAO_PEGTL_NAMESPACE

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   enum class my_type
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

   template< typename Rule >
   struct my_action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct my_action< pegtl::eof >
   {
      static void apply0()
      {
         std::cout << "We have eof." << std::endl;
      }
   };

   template<>
   struct my_action< pegtl::token_type_rule< my_type::beta > >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in )
      {
         assert( in.size() == 1 );
         std::cout << "We have a token of type 'beta' with data '" << in.peek().data << "'." << std::endl;
      }
   };

   struct my_grammar
      : pegtl::seq< pegtl::plus< pegtl::token_type_rule< my_type::beta > >, pegtl::eof >
   {};

}  // namespace example

int main()
{
   const std::vector< example::my_token > v{
      { example::my_type::beta, "first" },
      { example::my_type::beta, "second" }
   };
   pegtl::token_input< example::my_token > in( v );

   std::cout << "Example parsing " << v.size() << " token(s)." << std::endl;

   if( pegtl::parse< example::my_grammar, example::my_action >( in ) ) {
      std::cout << "Token example success." << std::endl;
      return 0;
   }
   std::cout << "Token example failure!" << std::endl;
   return 1;
}
