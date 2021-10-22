// Copyright (c) 2017-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/analyze.hpp>

using namespace TAO_PEGTL_NAMESPACE;

struct bar;

struct foo
   : sor< digit, bar >
{};

struct bar
   : plus< foo >  // seq< foo, opt< bar > >
{};

int main()  // NOLINT(bugprone-exception-escape)
{
   if( analyze< foo >( 1 ) != 0 ) {
      std::cerr << "there are problems" << std::endl;
      return 1;
   }
   return 0;
}
