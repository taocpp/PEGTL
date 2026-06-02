// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <string>
#include <string_view>

#include <tao/pegtl/copy_input.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using input = pegtl::copy_input< pegtl::default_eol, std::string >;
#else
using input = pegtl::copy_input< pegtl::default_eol, std::string_view >;
#endif

int main()
{
#if TAO_PEGTL_COMPILE_ACCEPT
   input in{ std::string() };
#else
   input in{ std::string_view() };
#endif
   return in.empty() ? 0 : 1;
}
