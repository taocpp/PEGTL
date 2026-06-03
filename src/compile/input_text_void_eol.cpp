// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/text_view_input.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using input = pegtl::text_view_input< pegtl::default_eol >;
#else
// include/tao/pegtl/internal/text_input.hpp
// static_assert( !std::is_same_v< Eol, void > );
using input = pegtl::text_view_input< void >;
#endif

int main()
{
   input in( "" );
   return in.empty() ? 0 : 1;
}
