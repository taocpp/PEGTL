// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using rule = pegtl::utf8::string< 0x10ffff >;
#else
// include/tao/pegtl/unicode/internal/utf32_to_utf8.hpp
// template< template< char... > class String, char32_t C, typename = void >
// struct utf32_to_utf8_char;
using rule = pegtl::utf8::string< 0x110000 >;
#endif

int main()
{
   (void)sizeof( rule );
   return 0;
}
