// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#include <iostream>
int main()
{
   std::cout << "Exception and/or RTTI support disabled, skipping test..." << std::endl;
}
#else

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
   const std::string s1 = "foo";
   const std::string s2 = "bar";

}  // namespace

int main()
{
   try {
      try {
         throw std::string( s1 );
      }
      catch( ... ) {
         std::throw_with_nested( std::runtime_error( s2 ) );
      }
   }
   catch( ... ) {
   }
   return 0;
}

#endif
