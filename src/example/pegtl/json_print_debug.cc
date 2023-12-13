// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/print.hpp>

int main()  // NOLINT(bugprone-exception-escape)
{
   TAO_PEGTL_NAMESPACE::print_debug< TAO_PEGTL_NAMESPACE::json::text >( std::cout );
   return 0;
}
