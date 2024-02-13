// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/inputs.hpp>

#include <tao/pegtl/internal/has_start.hpp>

using namespace TAO_PEGTL_NAMESPACE;

int main()
{
   static_assert( internal::has_start< argv_input<> > );
   static_assert( internal::has_start< view_input<> > );
   static_assert( internal::has_start< file_input<> > );
   static_assert( internal::has_start< read_input<> > );
   static_assert( internal::has_start< mmap_input<> > );

   static_assert( !internal::has_start< base_input<> > );

   return 0;
}
