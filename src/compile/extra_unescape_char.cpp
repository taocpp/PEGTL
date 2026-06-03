// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/extra/unescape.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using action = pegtl::unescape_char< '\n' >;
#else
// include/tao/pegtl/extra/unescape.hpp
// static_assert( sizeof...( Rs ) > 0 );
using action = pegtl::unescape_char<>;
#endif

int main()
{
   (void)sizeof( action );
   return 0;
}
