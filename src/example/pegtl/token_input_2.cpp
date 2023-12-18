// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/member.hpp>

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

using namespace TAO_PEGTL_NAMESPACE;

template< my_type T >
struct token_type_rule
   : member::one< &my_token::type, T >
{};

using token_input = internal::input_with_fakes< internal::copy_input< std::vector< my_token > > >;

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
   token_input in( v );

   if( !parse< my_grammar, my_action >( in ) ) {
      return 1;
   }
   return 0;
}
