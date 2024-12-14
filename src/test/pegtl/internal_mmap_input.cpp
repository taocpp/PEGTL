// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#if defined( __cpp_exceptions ) && ( defined( _POSIX_MAPPED_FILES ) || defined( _WIN32 ) )

#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_THROWS( internal::mmap_input< std::size_t >( "src/test/pegtl/data/duseltronik.txt" ) );
      const internal::mmap_input< char > in( "src/test/pegtl/data/duseltronik.txt" );
      const std::string data( in.start(), in.end() );
      TAO_PEGTL_TEST_ASSERT( data == "duseltronik" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#else

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::cerr << "Skipping mmap test..." << std::endl;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif

#include "main.hpp"
