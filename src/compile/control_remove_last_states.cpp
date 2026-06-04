// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl.hpp>
#include <tao/pegtl/control/remove_last_states.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

template< typename Rule >
struct control
#if TAO_PEGTL_COMPILE_ACCEPT
   : pegtl::remove_last_states_r< 0, pegtl::normal, Rule >
#else
// include/tao/pegtl/control/remove_last_states.hpp
// static_assert( S >= N );
   : pegtl::remove_last_states_r< 1, pegtl::normal, Rule >
#endif
{};

int main()
{
   pegtl::text_view_input in( "" );
   return pegtl::parse< pegtl::eof, pegtl::nothing, control >( in ) ? 0 : 1;
}
