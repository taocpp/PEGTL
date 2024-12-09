// Copyright (c) 2015-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#if defined( __cpp_exceptions ) && defined( _POSIX_MAPPED_FILES )

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      TAO_PEGTL_TEST_THROWS( internal::mmap_file_impl( "include" ) );

      const std::string s = "dummy content\n";
      const std::string dummy_content = s + s + s + s + s + s + s + s + s + s + s;

      internal::mmap_file_impl mapper( "src/test/pegtl/data/test_data.txt" );
      TAO_PEGTL_TEST_ASSERT( mapper.size() == 154 );
      TAO_PEGTL_TEST_ASSERT( std::string_view( mapper.data(), mapper.size() ) == dummy_content );
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
