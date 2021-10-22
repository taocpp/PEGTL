// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

// this include gives us _POSIX_MAPPED_FILES to test and mmap_input<> if it is set
#include <tao/pegtl/file_input.hpp>

#if defined( _POSIX_MAPPED_FILES ) || defined( _WIN32 )

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_file< mmap_input<> >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#else

int main()
{
   return 0;
}

#endif
