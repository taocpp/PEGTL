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

   struct escaped_x : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< 'x' >, tao::TAOCPP_PEGTL_NAMESPACE::rep< 2, tao::TAOCPP_PEGTL_NAMESPACE::must< tao::TAOCPP_PEGTL_NAMESPACE::xdigit > > > {};
   struct escaped_u : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< 'u' >, tao::TAOCPP_PEGTL_NAMESPACE::rep< 4, tao::TAOCPP_PEGTL_NAMESPACE::must< tao::TAOCPP_PEGTL_NAMESPACE::xdigit > > > {};
   struct escaped_U : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< 'U' >, tao::TAOCPP_PEGTL_NAMESPACE::rep< 8, tao::TAOCPP_PEGTL_NAMESPACE::must< tao::TAOCPP_PEGTL_NAMESPACE::xdigit > > > {};
   struct escaped_c : tao::TAOCPP_PEGTL_NAMESPACE::one< '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v' > {};

   struct escaped : tao::TAOCPP_PEGTL_NAMESPACE::sor< escaped_x,
                                     escaped_u,
                                     escaped_U,
                                     escaped_c > {};

   struct character : tao::TAOCPP_PEGTL_NAMESPACE::if_must_else< tao::TAOCPP_PEGTL_NAMESPACE::one< '\\' >, escaped, tao::TAOCPP_PEGTL_NAMESPACE::utf8::range< 0x20, 0x10FFFF > > {};
   struct literal : tao::TAOCPP_PEGTL_NAMESPACE::if_must< tao::TAOCPP_PEGTL_NAMESPACE::one< '"' >, tao::TAOCPP_PEGTL_NAMESPACE::until< tao::TAOCPP_PEGTL_NAMESPACE::one< '"' >, character > > {};

   struct padded : tao::TAOCPP_PEGTL_NAMESPACE::must< tao::TAOCPP_PEGTL_NAMESPACE::pad< literal, tao::TAOCPP_PEGTL_NAMESPACE::blank >, tao::TAOCPP_PEGTL_NAMESPACE::eof > {};

   // Action class that uses the actions from tao/pegtl/contrib/unescape.hh to
   // produce a UTF-8 encoded result string where all escape sequences are
   // replaced with their intended meaning.

   template< typename Rule > struct action : tao::TAOCPP_PEGTL_NAMESPACE::nothing< Rule > {};

   template<> struct action< tao::TAOCPP_PEGTL_NAMESPACE::utf8::range< 0x20, 0x10FFFF > > : tao::TAOCPP_PEGTL_NAMESPACE::unescape::append_all {};
   template<> struct action< escaped_x > : tao::TAOCPP_PEGTL_NAMESPACE::unescape::unescape_x {};
   template<> struct action< escaped_u > : tao::TAOCPP_PEGTL_NAMESPACE::unescape::unescape_u {};
   template<> struct action< escaped_U > : tao::TAOCPP_PEGTL_NAMESPACE::unescape::unescape_u {};
   template<> struct action< escaped_c > : tao::TAOCPP_PEGTL_NAMESPACE::unescape::unescape_c< escaped_c, '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v' > {};

} // namespace unescape

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      tao::TAOCPP_PEGTL_NAMESPACE::unescape::state s;
      tao::TAOCPP_PEGTL_NAMESPACE::parse_arg< unescape::padded, unescape::action >( i, argv, s );
      std::cout << "argv[ " << i << " ] = " << s.unescaped << std::endl;
   }
   return 0;
}
