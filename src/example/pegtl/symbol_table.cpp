// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/integer.hpp>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace example
{
   // Simple example for parsing with a symbol table; here the custom
   // logic is in the semantic actions and an exception is thrown on
   // error -- other possibilities are to let the actions' apply()
   // function return a bool instead, or to use a custom rule for the
   // matching of name within assignment that uses the symbol table
   // in its match() function.

   struct state
   {
      unsigned converted = 0;
      std::string temporary;
      std::map< std::string, unsigned > symbol_table;
   };

   // clang-format off
   struct semi : pegtl::one< ';' > {};
   struct blank0 : pegtl::star< pegtl::blank > {};
   struct blanks : pegtl::plus< pegtl::blank > {};
   struct name : pegtl::plus< pegtl::alpha > {};
   struct value : pegtl::plus< pegtl::digit > {};
   struct equals : pegtl::pad< pegtl::one< '=' >, pegtl::blank > {};
   struct definition : pegtl::if_must< pegtl::string< 'd', 'e', 'f' >, blanks, name, blank0, semi > {};
   struct assignment : pegtl::if_must< name, equals, value, blank0, semi > {};
   struct something : pegtl::sor< pegtl::space, definition, assignment > {};
   struct grammar : pegtl::until< pegtl::eof, pegtl::must< something > > {};
   // clang-format on

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {
   };

   template<>
   struct action< value >
      : public pegtl::integer::unsigned_action
   {
      // Sets st.converted to the integer value of the matched string.
   };

   template<>
   struct action< name >
   {
      template< typename Input >
      static void apply( const Input& in, state& st )
      {
         st.temporary = in.string();
      }
   };

   template<>
   struct action< definition >
   {
      template< typename Input >
      static void apply( const Input& in, state& st )
      {
         if( !st.symbol_table.insert( { st.temporary, 0 } ).second ) {
            throw pegtl::parse_error( "duplicate symbol " + st.temporary, in );  // NOLINT
         }
      }
   };

   template<>
   struct action< assignment >
   {
      template< typename Input >
      static void apply( const Input& in, state& st )
      {
         const auto i = st.symbol_table.find( st.temporary );
         if( i == st.symbol_table.end() ) {
            throw pegtl::parse_error( "unknown symbol " + st.temporary, in );  // NOLINT
         }
         i->second = st.converted;
      }
   };

}  // namespace example

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      pegtl::file_input<> in( argv[ i ] );
      example::state st;
      pegtl::parse< example::grammar, example::action >( in, st );
      for( const auto& j : st.symbol_table ) {
         std::cout << j.first << " = " << j.second << std::endl;
      }
   }
   return 0;
}
