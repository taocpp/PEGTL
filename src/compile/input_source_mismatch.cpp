// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <string>

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using input = pegtl::text_view_input< pegtl::default_eol, char, std::string, std::string >;
#else
// include/tao/pegtl/internal/text_input_with_source.hpp
// static_assert( std::is_same_v< InputSource, ErrorSource >, "TODO?" );
using input = pegtl::text_view_input< pegtl::default_eol, char, std::string, int >;
#endif

int main()
{
   (void)sizeof( input );
   return 0;
}
