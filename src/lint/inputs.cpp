// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <string_view>

#include <tao/pegtl.hpp>
#include <tao/pegtl/stream.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct grammar : pegtl::seq< pegtl::ascii::alpha, pegtl::eol, pegtl::eof > {};

}  // namespace

int main()  // NOLINT(bugprone-exception-escape) -- Deliberately instantiates throwing parse paths.
{
   pegtl::view_input plain( "a\n" );
   const bool plain_result = pegtl::parse< grammar >( plain );

   pegtl::text_view_input< pegtl::scan::lf > text( "a\n" );
   const bool text_result = pegtl::parse< grammar >( text );

   pegtl::text_view_input sourced( std::string( "lint" ), std::string_view( "a\n" ) );
   const bool sourced_result = pegtl::parse< grammar >( sourced );

   pegtl::alloc_text_cstring_input<> stream( 64, 16, "a\n" );
   const bool stream_result = pegtl::parse< grammar >( stream );

   return ( plain_result && text_result && sourced_result && stream_result ) ? 0 : 1;
}
