// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <pegtl.hh>
#include <pegtl/contrib/json.hh>

// define dedicated error messages:
template< typename Rule >
struct normal : pegtl::normal< Rule >
{
   static const std::string error_message;

   template< typename Input, typename ... States >
   static void raise( const Input & in, States && ... )
   {
      throw pegtl::parse_error( error_message, in );
   }
};

// if there is no default, the compiler will complain
// about missing error messages for a given grammar

template<> const std::string normal< pegtl::json::end_array >::error_message = "incomplete array, expected ']'";
template<> const std::string normal< pegtl::json::end_object >::error_message = "incomplete object, expected '}'";
template<> const std::string normal< pegtl::json::member >::error_message = "expected member";
template<> const std::string normal< pegtl::json::name_separator >::error_message = "expected ':'";
template<> const std::string normal< pegtl::json::value >::error_message = "expected value";

template<> const std::string normal< pegtl::json::required_digits >::error_message = "expected at least one digit";
template<> const std::string normal< pegtl::json::required_xdigit >::error_message = "incomplete universal character name";
template<> const std::string normal< pegtl::json::escaped >::error_message = "unknown escape sequence";
template<> const std::string normal< pegtl::json::char_ >::error_message = "invalid character in string";
template<> const std::string normal< pegtl::json::string_cont >::error_message = "unterminated string";

template<> const std::string normal< pegtl::json::text >::error_message = "no JSON object could be decoded";

// parse the command line arguments, one-by-one
int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      pegtl::parse< pegtl::json::text, pegtl::nothing, normal >( i, argv );
   }
   return 0;
}
