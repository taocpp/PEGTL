// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

int main()
{
#if TAO_PEGTL_COMPILE_ACCEPT
   pegtl::view_input in( "a" );
   return in.peek_char() == 'a' ? 0 : 1;
#else
   const char32_t data[] = { U'a' };
   pegtl::view_input< pegtl::default_eol, char32_t > in( data, data + 1 );
   // include/tao/pegtl/internal/input_with_funcs.hpp
   // static_assert( sizeof( data_t ) == sizeof( T ) );
   return in.peek_char() == 'a' ? 0 : 1;
#endif
}
