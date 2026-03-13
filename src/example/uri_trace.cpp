// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/debug/trace.hpp>
#include <tao/pegtl/example/uri.hpp>

// Shorten the namespace for this example.
namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   // Type alias for the top-level rule for the parsing run.
   using grammar = pegtl::must< pegtl::uri::URI >;

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      std::cout << "Parsing " << argv[ i ] << std::endl;
      // Set up an input that represents argv[ i ].
      pegtl::argv_input in( argv, i );
      // Start a parsing run that prints trace information for all rules,
      // including the internal PEGTL ones that have control disabled.
      pegtl::complete_trace< example::grammar >( in );
   }
   return 0;
}

#endif
