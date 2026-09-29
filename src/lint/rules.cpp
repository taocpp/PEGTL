// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <cstdint>

#include <tao/pegtl.hpp>

#include <tao/pegtl/binary/uint16.hpp>
#include <tao/pegtl/unicode/utf16.hpp>
#include <tao/pegtl/unicode/utf32.hpp>
#include <tao/pegtl/unicode/utf8.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct text_grammar
      : pegtl::seq< pegtl::ascii::identifier, pegtl::one< ':' >, pegtl::plus< pegtl::ascii::digit >, pegtl::eof >
   {};

}  // namespace

int main()
{
   pegtl::text_view_input<> text( "answer:42" );
   const bool text_result = pegtl::parse< text_grammar >( text );

   const std::array< std::uint8_t, 2 > bytes{ 0x12, 0x34 };
   pegtl::view_input binary( bytes );
   const bool binary_result = pegtl::parse< pegtl::seq< pegtl::uint16_be::one< 0x1234 >, pegtl::eof > >( binary );

   pegtl::view_input utf8( "\xe2\x82\xac" );
   const bool utf8_result = pegtl::parse< pegtl::utf8::one< 0x20ac > >( utf8 );

   pegtl::view_input empty16( "" );
   const bool utf16_result = pegtl::parse< pegtl::utf16_le::any >( empty16 );

   pegtl::view_input empty32( "" );
   const bool utf32_result = pegtl::parse< pegtl::utf32_be::any >( empty32 );

   return ( text_result && binary_result && utf8_result && !utf16_result && !utf32_result ) ? 0 : 1;
}
