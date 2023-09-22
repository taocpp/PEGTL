// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
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

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>

namespace sexpr
{
   namespace pegtl = TAO_PEGTL_NAMESPACE;

   // clang-format off
   struct hash_comment : pegtl::until< pegtl::eolf > {};

   struct list;
   struct list_comment : pegtl::if_must< pegtl::at< pegtl::one< '(' > >, pegtl::disable< list > > {};

   struct read_include : pegtl::seq< pegtl::one< ' ' >, pegtl::one< '"' >, pegtl::plus< pegtl::not_one< '"' > >, pegtl::one< '"' > > {};
   struct hash_include : pegtl::if_must< pegtl::string< 'i', 'n', 'c', 'l', 'u', 'd', 'e' >, read_include > {};

   struct hashed : pegtl::if_must< pegtl::one< '#' >, pegtl::sor< hash_include, list_comment, hash_comment > > {};

   struct number : pegtl::plus< pegtl::digit > {};
   struct symbol : pegtl::identifier {};

   struct atom : pegtl::sor< number, symbol > {};

   struct anything;

   struct list : pegtl::if_must< pegtl::one< '(' >, pegtl::until< pegtl::one< ')' >, anything > > {};

   struct normal : pegtl::sor< atom, list > {};

   struct anything : pegtl::sor< pegtl::space, hashed, normal > {};

   struct main : pegtl::until< pegtl::eof, pegtl::must< anything > > {};
   // clang-format on

   template< typename Rule >
   struct action
   {};

   template<>
   struct action< pegtl::plus< pegtl::not_one< '"' > > >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& fn )
      {
         fn = in.string();
      }
   };

   template<>
   struct action< hash_include >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& fn )
      {
         std::string f2;
         // Here f2 is the state argument for the nested parsing
         // run (to store the value of the string literal like in
         // the upper-level parsing run), fn is the value of the
         // last string literal that we use as filename here, and
         // the input is passed on for chained error messages (as
         // in "error in line x file foo included from file bar...)
         pegtl::file_input i2( fn );
         pegtl::parse_nested< main, sexpr::action >( in, i2, f2 );
      }
   };

}  // namespace sexpr

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( TAO_PEGTL_NAMESPACE::analyze< sexpr::main >() != 0 ) {
      return 1;
   }
   for( int i = 1; i < argc; ++i ) {
      std::string fn;
      TAO_PEGTL_NAMESPACE::argv_input in( argv, i );
      try {
         TAO_PEGTL_NAMESPACE::parse< sexpr::main, sexpr::action >( in, fn );
      }
      catch( const TAO_PEGTL_NAMESPACE::parse_error& e ) {
         const auto& p = e.position_object();
         std::cerr << e.what() << '\n'
                   << in.line_at( p ) << '\n'
                   << std::setw( int( p.column ) ) << '^' << '\n';
      }
   }
   return 0;
}

#endif
