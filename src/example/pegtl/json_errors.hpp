// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_ERRORS_HPP  // NOLINT
#define TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_ERRORS_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace examples
{
   // This file shows how to throw exceptions with
   // custom error messages for parse errors. A custom
   // control class is created that delegates everything
   // to the PEGTL default control class TAO_PEGTL_NAMESPACE::normal<>
   // except for the throwing of exceptions:

   template< typename Rule >
   struct errors
      : public pegtl::normal< Rule >
   {
      static const std::string error_message;

      template< typename Input, typename... States >
      static void raise( const Input& in, States&&... /*unused*/ )
      {
         throw pegtl::parse_error( error_message, in );
      }
   };

   // The following specialisations of the static string
   // member are then used in the exception messages:

   // clang-format off
   template<> inline const std::string errors< pegtl::json::text >::error_message = "no valid JSON";  // NOLINT

   template<> inline const std::string errors< pegtl::json::end_array >::error_message = "incomplete array, expected ']'";  // NOLINT
   template<> inline const std::string errors< pegtl::json::end_object >::error_message = "incomplete object, expected '}'";  // NOLINT
   template<> inline const std::string errors< pegtl::json::member >::error_message = "expected member";  // NOLINT
   template<> inline const std::string errors< pegtl::json::name_separator >::error_message = "expected ':'";  // NOLINT
   template<> inline const std::string errors< pegtl::json::array_element >::error_message = "expected value";  // NOLINT
   template<> inline const std::string errors< pegtl::json::value >::error_message = "expected value";  // NOLINT

   template<> inline const std::string errors< pegtl::json::digits >::error_message = "expected at least one digit";  // NOLINT
   template<> inline const std::string errors< pegtl::json::xdigit >::error_message = "incomplete universal character name";  // NOLINT
   template<> inline const std::string errors< pegtl::json::escaped >::error_message = "unknown escape sequence";  // NOLINT
   template<> inline const std::string errors< pegtl::json::char_ >::error_message = "invalid character in string";  // NOLINT
   template<> inline const std::string errors< pegtl::json::string::content >::error_message = "unterminated string";  // NOLINT
   template<> inline const std::string errors< pegtl::json::key::content >::error_message = "unterminated key";  // NOLINT

   template<> inline const std::string errors< pegtl::eof >::error_message = "unexpected character after JSON value";  // NOLINT
   // clang-format on

   // The raise()-function-template is instantiated exactly
   // for the specialisations of errors< Rule > for which a
   // parse error can be generated, therefore the string
   // error_message needs to be supplied only for these rules
   // (and the compiler will complain if one is missing).

}  // namespace examples

#endif
