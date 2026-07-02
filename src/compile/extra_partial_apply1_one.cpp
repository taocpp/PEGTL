// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>

#include <tao/pegtl/extra/partial_apply1.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using rule = pegtl::partial_apply1< pegtl::one< 'a' >, pegtl::one< 'b' > >;
#else
// include/tao/pegtl/extra/internal/partial_apply1.hpp
// static_assert( sizeof...( Rules ) > 0 );
using rule = pegtl::partial_apply1< pegtl::one< 'a' > >;
#endif

int main()
{
   (void)sizeof( rule );
   return 0;
}
