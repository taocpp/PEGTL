// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/extra/parse_tree.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct selected_rule
   : pegtl::one< 'a' >
{};

#if TAO_PEGTL_COMPILE_ACCEPT
using selector = pegtl::parse_tree::selector< selected_rule, pegtl::parse_tree::store_content::on< selected_rule > >;
#else
// include/tao/pegtl/extra/parse_tree.hpp
// static_assert( sizeof...( Ts ) == 0, "multiple matches found" );
using selector = pegtl::parse_tree::selector< selected_rule, pegtl::parse_tree::store_content::on< selected_rule >, pegtl::parse_tree::remove_content::on< selected_rule > >;
#endif

int main()
{
#if TAO_PEGTL_COMPILE_ACCEPT
   (void)sizeof( selector );
#endif
   return 0;
}
