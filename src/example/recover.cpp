// Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

// This is a small experiment with a grammar that can recover from errors.
//
// Motivated by https://github.com/taocpp/PEGTL/issues/55
//
// The grammar will recognize simple expressions terminated by semicolons.
// When an expression fails to parse, it skips to the next expression
// by looking for the terminator.
//
// Try: build/bin/example/recover '1+2*3;1+2*(3-)-4;-5;6/;7*(8+9)'

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <iostream>
#include <string>

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   // clang-format off
   template< typename T >
   struct skipping : pegtl::until< T > {};

   template< typename R, typename T >
   struct recoverable : pegtl::sor< pegtl::try_catch_return_false< pegtl::must< R >, T >, skipping< T > > {};

   struct expr_sum;
   struct expr_identifier : pegtl::identifier {};
   struct expr_number : pegtl::plus< pegtl::digit > {};
   struct expr_braced : pegtl::if_must< pegtl::one< '(' >, pegtl::pad< expr_sum, pegtl::space >, pegtl::one< ')' > > {};

   struct expr_value : pegtl::sor< expr_identifier, expr_number, expr_braced > {};

   struct expr_power : pegtl::list< pegtl::must< expr_value >, pegtl::one< '^' >, pegtl::space > {};
   struct expr_prod : pegtl::list_must< expr_power, pegtl::one< '*', '/', '%' >, pegtl::space > {};
   struct expr_sum : pegtl::list_must< expr_prod, pegtl::one< '+', '-' >, pegtl::space > {};

   struct term : pegtl::sor< pegtl::one< ';' >, pegtl::eof > {};
   struct expr : pegtl::pad< expr_sum, pegtl::space > {};
   struct recoverable_expr : recoverable< expr, term > {};

   struct my_grammar : pegtl::star< pegtl::not_at< pegtl::eof >, recoverable_expr > {};
   // clang-format on

   template< typename Rule >
   struct my_action
      : pegtl::nothing< Rule >
   {};

   template< typename T >
   struct my_action< skipping< T > >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, bool& error )
      {
         if( !error ) {
            std::cout << in.current_position() << ": Invalid expression \"" << in.string() << "\"" << std::endl;
         }
         error = true;
      }
   };

   template< typename R >
   struct found
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, bool& error )
      {
         if( !error ) {
            std::cout << in.current_position() << ": Found " << pegtl::demangle< R >() << ": \"" << in.string() << "\"" << std::endl;
         }
      }
   };

   // clang-format off
   // template<> struct my_action< expr_identifier > : found< expr_identifier > {};
   // template<> struct my_action< expr_number > : found< expr_number > {};
   // template<> struct my_action< expr_braced > : found< expr_braced > {};
   // template<> struct my_action< expr_value > : found< expr_value > {};
   // template<> struct my_action< expr_power > : found< expr_power > {};
   // template<> struct my_action< expr_prod > : found< expr_prod > {};
   // template<> struct my_action< expr_sum > : found< expr_sum > {};
   template<> struct my_action< expr > : found< expr > {};
   // clang-format on

   template<>
   struct my_action< recoverable_expr >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, bool& error )
      {
         error = false;
         std::cout << std::string( 79, '-' ) << std::endl;
      }
   };

   template< typename Rule >
   struct my_control
      : pegtl::normal< Rule >
   {
      template< typename ParseInput, typename... States >
      [[noreturn]] static void raise( const ParseInput& in, States&&... st )
      {
         std::cout << in.current_position() << ": Parse error matching " << pegtl::demangle< Rule >() << std::endl;
         pegtl::normal< Rule >::raise( in, st... );
      }
   };

}  // namespace example

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      pegtl::argv_input< pegtl::scan::lf_crlf > in( argv, i );
      bool error = false;
      const bool result = pegtl::parse< example::my_grammar, example::my_action, example::my_control >( in, error );
      std::cout << "Parsed argv[ " << i << " ] result " << result << " << error " << error << std::endl;
   }
   return 0;
}

#endif
