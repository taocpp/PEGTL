// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <string>

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using position = pegtl::position_with_source< std::string, pegtl::pointer_position< char > >;
#else
// include/tao/pegtl/position_with_source.hpp
// static_assert( std::is_same_v< Source, std::decay_t< Source > > );
using position = pegtl::position_with_source< const std::string, pegtl::pointer_position< char > >;
#endif

int main()
{
   (void)sizeof( position );
   return 0;
}
