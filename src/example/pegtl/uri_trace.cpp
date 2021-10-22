// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
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
#include <tao/pegtl/contrib/trace.hpp>
#include <tao/pegtl/contrib/uri.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

using grammar = pegtl::must< pegtl::uri::URI >;

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      std::cout << "Parsing " << argv[ i ] << std::endl;
      pegtl::argv_input in( argv, i );
      pegtl::complete_trace< grammar >( in );
   }
   return 0;
}

#endif
