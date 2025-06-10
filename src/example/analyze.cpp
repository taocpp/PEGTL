// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl.hpp>

#include <tao/pegtl/debug/analyze.hpp>

using namespace TAO_PEGTL_NAMESPACE;

struct bar;

struct foo
   : sor< digit, bar >
{};

struct bar
   : plus< foo >  // seq< foo, opt< bar > >
{};

// We expect problems because if the next character in the input is not a
// digit then matching bar will attempt to match foo, which will attempt to
// match bar again (after failing digit), and so on ad infinitum. Which is
// of course exaclty the kind of problem the grammar analysis is supposed
// to detect and complain about...

int main()  // NOLINT(bugprone-exception-escape)
{
   if( analyze< foo >( 1 ) != 0 ) {
      std::cerr << "There are problems!" << std::endl;
      return 1;
   }
   std::cerr << "No problems found -- which is itself a problem because we were expecting some!" << std::endl;
   return 0;
}
