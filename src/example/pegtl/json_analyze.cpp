// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/debug/analyze.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

int main()  // NOLINT(bugprone-exception-escape)
{
   if( pegtl::analyze< grammar >() != 0 ) {
      std::cerr << "cycles without progress detected!" << std::endl;
      return 1;
   }
   return 0;
}
