// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/coverage.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/print_coverage.hpp>

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      std::cout << tao::pegtl::coverage< tao::pegtl::json::text >( tao::pegtl::file_input( argv[ i ] ) );
   }
   return 0;
}
