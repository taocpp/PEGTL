// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_UNESCAPE_HPP  // NOLINT
#define TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_UNESCAPE_HPP

#include <string>

#include <tao/pegtl/contrib/change_action_and_states.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/unescape.hpp>

namespace examples
{
   // Action class for parsing literal strings, uses the PEGTL unescape utilities, cf. unescape.cpp.

   // clang-format off
   template< typename Rule > struct json_unescape_action {};

   template<> struct json_unescape_action< tao::TAO_PEGTL_NAMESPACE::json::unicode > : tao::TAO_PEGTL_NAMESPACE::unescape::unescape_j {};
   template<> struct json_unescape_action< tao::TAO_PEGTL_NAMESPACE::json::escaped_char > : tao::TAO_PEGTL_NAMESPACE::unescape::unescape_c< tao::TAO_PEGTL_NAMESPACE::json::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
   template<> struct json_unescape_action< tao::TAO_PEGTL_NAMESPACE::json::unescaped > : tao::TAO_PEGTL_NAMESPACE::unescape::append_all {};
   // clang-format on

   using json_unescape = tao::pegtl::change_action_and_states< json_unescape_action, std::string >;

}  // namespace examples

#endif
