// Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl.hpp>

#include <tao/pegtl/debug/analyze.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   struct bar;

   struct foo
      : pegtl::sor< pegtl::digit, bar >
   {};

   struct bar
      : pegtl::plus< foo >  // seq< foo, opt< bar > >
   {};

}  // namespace example

// We expect problems because if the next character in the input is not a
// digit then matching bar will attempt to match foo, which will attempt to
// match bar again (after failing digit), and so on ad infinitum. Which is
// of course exaclty the kind of problem the grammar analysis is supposed
// to detect and complain about...

int main()  // NOLINT(bugprone-exception-escape)
{
   if( pegtl::analyze< example::foo >( 1 ) != 0 ) {
      std::cout << "There are problems -- just as expected!" << std::endl;
      return 1;
   }
   std::cout << "No problems found -- which is itself a problem because we were expecting some!" << std::endl;
   return 0;
}
