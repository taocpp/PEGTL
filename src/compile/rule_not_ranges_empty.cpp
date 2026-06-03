// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using rule = pegtl::not_ranges< 'a' >;
#else
// include/tao/pegtl/internal/not_ranges.hpp
// static_assert( dependent_false< Peek > );
using rule = pegtl::not_ranges<>;
#endif

int main()
{
   (void)sizeof( rule );
   return 0;
}
