// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl/debug/print.hpp>
#include <tao/pegtl/example/json.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

int main()  // NOLINT(bugprone-exception-escape)
{
   pegtl::print_names< pegtl::json::text >( std::cout );
   return 0;
}
