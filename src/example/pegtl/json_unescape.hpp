// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_UNESCAPE_HPP  // NOLINT
#define TAO_PEGTL_SRC_EXAMPLES_PEGTL_JSON_UNESCAPE_HPP

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/unescape.hpp>

namespace examples
{
   // State base class to store an unescaped string

   struct unescape_state_base
   {
      unescape_state_base() = default;

      unescape_state_base( const unescape_state_base& ) = delete;
      unescape_state_base( unescape_state_base&& ) = delete;

      ~unescape_state_base() = default;

      void operator=( const unescape_state_base& ) = delete;
      void operator=( unescape_state_base&& ) = delete;

      std::string unescaped;
   };

   // Action class for parsing literal strings, uses the PEGTL unescape utilities, cf. unescape.cpp.

   template< typename Rule, template< typename... > class Base = TAO_PEGTL_NAMESPACE::nothing >
   struct unescape_action : Base< Rule >
   {
   };

   // clang-format off
   template<> struct unescape_action< TAO_PEGTL_NAMESPACE::json::unicode > : TAO_PEGTL_NAMESPACE::unescape::unescape_j {};
   template<> struct unescape_action< TAO_PEGTL_NAMESPACE::json::escaped_char > : TAO_PEGTL_NAMESPACE::unescape::unescape_c< TAO_PEGTL_NAMESPACE::json::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
   template<> struct unescape_action< TAO_PEGTL_NAMESPACE::json::unescaped > : TAO_PEGTL_NAMESPACE::unescape::append_all {};
   // clang-format on

}  // namespace examples

#endif
