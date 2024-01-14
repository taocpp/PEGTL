// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if defined( _POSIX_MAPPED_FILES ) || defined( _WIN32 )

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const internal::mmap_input in( "src/test/pegtl/data/duseltronik.txt" );
      const std::string data( in.start(), in.end() );
      TAO_PEGTL_TEST_ASSERT( data == "duseltronik" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#else

int main()
{
   return 0;
}

#endif
