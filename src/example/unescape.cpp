// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/example/escaped.hpp>
#include <tao/pegtl/extra/unescape.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   // clang-format off
   struct escaped : pegtl::sor< pegtl::c_escaped_char,
                                pegtl::hex_escaped_byte,
                                pegtl::short_escaped_unicode,
                                pegtl::long_escaped_unicode > {};

   struct character : pegtl::if_then_else< pegtl::one< '\\' >,
                                           escaped,
                                           pegtl::utf8::range< 0x20, 0x10FFFF > > {};

   struct grammar : pegtl::seq< pegtl::until< pegtl::eof, character > > {};
   // clang-format on

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      std::string s;
      pegtl::argv_input< void > in( argv, i );
      if( pegtl::parse< example::grammar, pegtl::unescape >( in, s ) ) {
         std::cout << "argv[ " << i << " ] = " << s << std::endl;
      }
      else {
         std::cerr << "error parsing: " << argv[ i ] << std::endl;
      }
   }
   return 0;
}
