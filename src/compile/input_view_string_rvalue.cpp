// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <string>

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

int main()
{
#if TAO_PEGTL_COMPILE_ACCEPT
   std::string data = "a";
   pegtl::view_input in( data );
#else
   // include/tao/pegtl/internal/view_input.hpp
   // view_input( std::string&& ) = delete;
   pegtl::view_input in{ std::string( "a" ) };
#endif
   return in.size() == 1 ? 0 : 1;
}
