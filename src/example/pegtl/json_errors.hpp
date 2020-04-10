// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_ERRORS_HPP
#define TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_ERRORS_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/raise_controller.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace examples
{
   // This file shows how to throw exceptions with
   // custom error messages for parse errors.
   // As the grammar contains must<>-rules,
   // the compiler will complain when a
   // specialization is missing.

   // clang-format off
   template< typename > constexpr const char* error_message = nullptr;
   template<> constexpr const char* error_message< pegtl::json::text > = "no valid JSON";

   template<> constexpr const char* error_message< pegtl::json::end_array > = "incomplete array, expected ']'";
   template<> constexpr const char* error_message< pegtl::json::end_object > = "incomplete object, expected '}'";
   template<> constexpr const char* error_message< pegtl::json::member > = "expected member";
   template<> constexpr const char* error_message< pegtl::json::name_separator > = "expected ':'";
   template<> constexpr const char* error_message< pegtl::json::array_element > = "expected value";
   template<> constexpr const char* error_message< pegtl::json::value > = "expected value";

   template<> constexpr const char* error_message< pegtl::json::digits > = "expected at least one digit";
   template<> constexpr const char* error_message< pegtl::json::xdigit > = "incomplete universal character name";
   template<> constexpr const char* error_message< pegtl::json::escaped > = "unknown escape sequence";
   template<> constexpr const char* error_message< pegtl::json::char_ > = "invalid character in string";
   template<> constexpr const char* error_message< pegtl::json::string::content > = "unterminated string";
   template<> constexpr const char* error_message< pegtl::json::key::content > = "unterminated key";

   template<> constexpr const char* error_message< pegtl::eof > = "unexpected character after JSON value";

   struct error { template< typename Rule > static constexpr const char* message = error_message< Rule >; };
   template< typename Rule > using control = pegtl::raise_controller< error >::control< Rule >;
   // clang-format on

}  // namespace examples

#endif
