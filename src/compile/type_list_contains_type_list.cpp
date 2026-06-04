// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/type_list.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

int main()
{
#if TAO_PEGTL_COMPILE_ACCEPT
   static_assert( pegtl::type_list_contains_v< int, pegtl::type_list< int > > );
#else
   // include/tao/pegtl/type_list.hpp
   // static_assert( internal::dependent_false< Ts..., Us... > );
   static_assert( pegtl::type_list_contains_v< pegtl::type_list< int >, int > );
#endif
   return 0;
}
