// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_UNESCAPE_HPP
#define TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_UNESCAPE_HPP

#include <string>

#include <tao/pegtl/change_action_and_states.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/unescape.hpp>

namespace example
{
   namespace pegtl = TAO_PEGTL_NAMESPACE;

   // Action class for parsing literal strings, uses the PEGTL unescape utilities, cf. unescape.cpp.

   // clang-format off
   template< typename Rule > struct json_unescape_action {};

   template<> struct json_unescape_action< pegtl::json::unicode > : pegtl::unescape::unescape_j {};
   template<> struct json_unescape_action< pegtl::json::escaped_char > : pegtl::unescape::unescape_c< pegtl::json::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
   template<> struct json_unescape_action< pegtl::json::unescaped > : pegtl::unescape::append_all {};
   // clang-format on

   using json_unescape = pegtl::change_action_and_states< json_unescape_action, std::string >;

}  // namespace example

#endif
