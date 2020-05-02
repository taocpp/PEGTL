// Copyright (c) 2015-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl/contrib/print.hpp>

#include "lua53.hpp"

int main()  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_names< lua53::grammar >( std::cout );
   return 0;
}
