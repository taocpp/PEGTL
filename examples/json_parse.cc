// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <pegtl.hh>
#include <pegtl/contrib/json.hh>

// This example shows how to throw exceptions with
// custom error messages for parse errors. A custom
// control class is created that delegates everything
// to the PEGTL default control class pegtl::normal<>
// except for the throwing of exceptions:

template< typename Rule >
struct normal
      : pegtl::normal< Rule >
{
   static const std::string error_message;

   template< typename Input, typename ... States >
   static void raise( const Input & in, States && ... )
   {
      throw pegtl::parse_error( error_message, in );
   }
};

// The following specialisations of the static string
// member are then used in the exception messages:

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

// The raise()-function-template is instantiated exactly
// for the specialisations of normal< Rule > for which a
// parse error can be generated, therefore the string
// error_message needs to be supplied only for these rules
// (and the compiler will complain if one is missing).

int main( int argc, char ** argv )
{
   // Individually parse each command line
   // argument as JSON value.

   for ( int i = 1; i < argc; ++i ) {
      pegtl::parse< pegtl::json::text, pegtl::nothing, normal >( i, argv );
   }
   return 0;
}
