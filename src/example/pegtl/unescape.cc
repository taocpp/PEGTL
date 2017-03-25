// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hh>

#include <tao/pegtl/contrib/unescape.hh>

namespace unescape
{
   // Grammar for string literals with some escape sequences from the C language:
   // - \x followed by two hex-digits to insert any byte value.
   // - \u followed by four hex-digits to insert a Unicode code point.
   // - \U followed by eight hex-digits to insert any Unicdoe code points.
   // - A backslash followed by one of the characters listed in the grammar below.

   struct escaped_x : tao::pegtl::seq< tao::pegtl::one< 'x' >, tao::pegtl::rep< 2, tao::pegtl::must< tao::pegtl::xdigit > > > {};
   struct escaped_u : tao::pegtl::seq< tao::pegtl::one< 'u' >, tao::pegtl::rep< 4, tao::pegtl::must< tao::pegtl::xdigit > > > {};
   struct escaped_U : tao::pegtl::seq< tao::pegtl::one< 'U' >, tao::pegtl::rep< 8, tao::pegtl::must< tao::pegtl::xdigit > > > {};
   struct escaped_c : tao::pegtl::one< '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v' > {};

   struct escaped : tao::pegtl::sor< escaped_x,
                                     escaped_u,
                                     escaped_U,
                                     escaped_c > {};

   struct character : tao::pegtl::if_must_else< tao::pegtl::one< '\\' >, escaped, tao::pegtl::utf8::range< 0x20, 0x10FFFF > > {};
   struct literal : tao::pegtl::if_must< tao::pegtl::one< '"' >, tao::pegtl::until< tao::pegtl::one< '"' >, character > > {};

   struct padded : tao::pegtl::must< tao::pegtl::pad< literal, tao::pegtl::blank >, tao::pegtl::eof > {};

   // Action class that uses the actions from tao/pegtl/contrib/unescape.hh to
   // produce a UTF-8 encoded result string where all escape sequences are
   // replaced with their intended meaning.

   template< typename Rule > struct action : tao::pegtl::nothing< Rule > {};

   template<> struct action< tao::pegtl::utf8::range< 0x20, 0x10FFFF > > : tao::pegtl::unescape::append_all {};
   template<> struct action< escaped_x > : tao::pegtl::unescape::unescape_x {};
   template<> struct action< escaped_u > : tao::pegtl::unescape::unescape_u {};
   template<> struct action< escaped_U > : tao::pegtl::unescape::unescape_u {};
   template<> struct action< escaped_c > : tao::pegtl::unescape::unescape_c< escaped_c, '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v' > {};

} // namespace unescape

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      tao::pegtl::unescape::state s;
      tao::pegtl::parse_arg< unescape::padded, unescape::action >( i, argv, s );
      std::cout << "argv[ " << i << " ] = " << s.unescaped << std::endl;
   }
   return 0;
}
